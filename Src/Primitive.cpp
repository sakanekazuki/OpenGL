/**
* @file Primitive.cpp
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include "Primitive.h"
#include "GLContext.h"
#include "GameEngine.h"

/**
* データをGPUメモリにコピーする
* 
* @param writeBuffer	コピー先のバッファオブジェクト
* @param unitSize		要素のバイト数
* @param offsetount		コピー先オフセット(要素単位)
* @param count			コピーする要素数
* @param data			コピーするデータのアドレス
* 
* @retval true  コピー成功
* @retval false コピー失敗
*/
bool CopyData(GLuint writeBuffer, GLsizei unitSize,
	GLsizei offsetCount, size_t count, const void* data)
{
	const GLsizei size = static_cast<GLsizei>(unitSize * count);
	const GLuint readBuffer = GLContext::CreateBuffer(size,data);
	if (!readBuffer)
	{
		std::cerr << "[エラー]" << __func__ << ": コピー元バッファの作成に失敗(size="
			<< size << ")\n";
		return false;
	}
	const GLsizei offset = static_cast<GLsizei>(unitSize*offsetCount);
	glCopyNamedBufferSubData(readBuffer, writeBuffer, 0, offset, size);
	glDeleteBuffers(1, &readBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[エラー]" << __func__ << ": データのコピーに失敗(size=" <<
			size << ", offset=" << offset << ")\n";
		return false;
	}
	return true;
}

/**
* プリミティブを描画する
*/
void Primitive::Draw()const
{
	glDrawElementsBaseVertex(mode, count, GL_UNSIGNED_SHORT, indices, baseVertex);
}

/**
* プリミティブ用のメモリを確保する
* 
* @param maxVertexCount	格納可能な最大頂点数
* @oaram maxIndexCount  格納可能な最大インデックス数
*/
PrimitiveBuffer::PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIndexCount)
{
	// VBO, IBO, VAOを作成
	vboPosition = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboColor = GLContext::CreateBuffer(sizeof(glm::vec4) * maxVertexCount, nullptr);
	vboTexcoord = GLContext::CreateBuffer(sizeof(glm::vec2) * maxVertexCount, nullptr);
	vboNormal = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	ibo = GLContext::CreateBuffer(sizeof(GLushort) * maxIndexCount, nullptr);
	vao = GLContext::CreateVertexArray(vboPosition, vboColor, vboTexcoord, vboNormal, ibo);
	if (!vboPosition || !vboColor || !vboTexcoord || !vboNormal || !ibo || !vao) 
	{
		std::cerr << "[エラー]" << __func__ << ": VAOの作成に失敗.\n";
	}
	
	// 描画データ用のメモリを1000個分予約.
	primitives.reserve(1000);
	
	// 管理予定の頂点数とインデックス数を設定.
	this->maxVertexCount = maxVertexCount;
	this->maxIndexCount = maxIndexCount;
}

/**
* デストラクタ
*/
PrimitiveBuffer::~PrimitiveBuffer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vboNormal);
	glDeleteBuffers(1, &vboTexcoord);
	glDeleteBuffers(1, &vboColor);
	glDeleteBuffers(1, &vboPosition);
}

/**
* 描画データを追加する
* 
* @param vertexCount	追加する頂点データの数
* @param pPosition		座標データへのポインタ
* @param pColor			色データへのポインタ
* @param pTexcoors		テクスチャ座標データへのポインタ
* @oaram pNormal		法線データへのポインタ
* @param indexCount		追加するインデックスデータの数
* @param pIndex			インデックスデータへのポインタ
* @param name			プリミティブの名前
* 
* @retval true  追加に成功
* @retval false 追加に失敗
*/
bool PrimitiveBuffer::Add(size_t vertexCount, const glm::vec3* pPosition,
	const glm::vec4* pColor, const glm::vec2* pTexcoord,
	const glm::vec3* pNormal, 
	size_t indexCount, const GLushort* pIndex,
	const char* name,GLenum type)
{
	// エラーチェック
	if (!vao)
	{
		std::cerr << "[エラー]" << __func__ << ": VAOの作成に失敗しています\n";
		return false;
	}
	else if (vertexCount > static_cast<size_t>(maxVertexCount) - curVertexCount)
	{
		std::cerr << "[警告]" << __func__ <<
			": IBOが満杯です(max=" << maxVertexCount <<
			",cur=" << curVertexCount <<
			",add=" << vertexCount << ")\n";
		return false;
	}
	else if (indexCount > static_cast<size_t>(maxIndexCount) - curIndexCount)
	{
		std::cerr << "[警告]" << __func__ << 
			": IBOが満杯です(max=" << maxIndexCount << 
			",cur=" << curIndexCount << 
			",add=" << indexCount << ")\n";
		return false;
	}

	// GPUメモリに座標データをコピー
	if (!CopyData(vboPosition, sizeof(glm::vec3), curVertexCount, vertexCount, pPosition))
	{
		return false;
	}

	// GPUメモリに色データをコピー
	if (!CopyData(vboColor, sizeof(glm::vec4), curVertexCount, vertexCount, pColor))
	{
		return false;
	}

	// GPUメモリにテクスチャ座標データをコピー
	if (!CopyData(vboTexcoord, sizeof(glm::vec2), curVertexCount, vertexCount, pTexcoord))
	{
		return false;
	}

	// GPUメモリに法線データをコピー
	if (!CopyData(vboNormal, sizeof(glm::vec3), curVertexCount, vertexCount, pNormal))
	{
		return false;
	}

	// GPUメモリにインデックスデータをコピー
	if (!CopyData(ibo, sizeof(GLshort), curIndexCount, indexCount, pIndex))
	{
		return false;
	}

	// 描画データを作成
	const Primitive prim(name,type,static_cast<GLsizei>(indexCount),
		sizeof(GLushort) * curIndexCount, curVertexCount);

	// 描画データを配列に追加
	primitives.push_back(prim);

	// 現在のデータ数を、追加したデータ数だけ増やす
	curVertexCount += static_cast<GLsizei>(vertexCount);
	curIndexCount += static_cast<GLsizei>(indexCount);

	return true;
}

/**
* OBJファイルからプリミティブを追加する
* 
* @param filename ロードするOBJファイル名
* 
* @retval true	追加成功
* @retval false 追加失敗
*/
bool PrimitiveBuffer::AddFromObjFile(const char* filename)
{
	// ファイルを開く
	std::ifstream ifs(filename);
	if (!ifs)
	{
		std::cerr << "[エラー]" << __func__ << ":`" << filename << "を開けません\n";
		return false;
	}

	// データ読み取り用の配列を準備

	// OBJファイルの頂点座標
	std::vector<glm::vec3> objPositions;
	// OBJファイルのテクスチャ座標
	std::vector<glm::vec2> objTexcoords;
	// OBJファイルの法線
	std::vector<glm::vec3> objNormals;
	struct Index
	{
		int v = 0;
		int vt = 0;
		int vn = 0;
	};
	// OBJファイルのインデックス
	std::vector<Index> objIndices;

	// 配列のメモリを予約
	objPositions.reserve(10'000);
	objTexcoords.reserve(10'000);
	objNormals.reserve(10'000);
	objIndices.reserve(10'000);

	//ファイルからモデルのデータを読み込む

	 // 読み込んだ行数
	size_t lineNo = 0;
	while (!ifs.eof())
	{
		std::string line;
		// ファイルから１行読み込む
		std::getline(ifs, line);
		++lineNo;

		// 行の先頭にある空白を読み飛ばす
		const size_t posData = line.find_first_not_of(' ');
		if (posData != std::string::npos)
		{
			line = line.substr(posData);
		}

		// 空行またはコメント行なら無視して次の行へ進む
		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		// データの種類を取得
		const size_t posEndOfType = line.find(' ');
		const std::string type = line.substr(0, posEndOfType);
		// データ部分を指すポインタ
		const char* p = line.c_str() + posEndOfType;
	
		// タイプ別のデータ読み込み処理

		 // 頂点座標
		if (type == "v")
		{
			glm::vec3 v(0);
			if (sscanf(p, "%f %f %f", &v.x, &v.y, &v.z) != 3)
			{
				std::cerr << "[警告]" << __func__ << 
					":頂点座標の読み取りに失敗\n" << " " << filename << 
					"(" << lineNo << "行目): " << line << "\n";
			}
			objPositions.push_back(v);
		}
		// テクスチャ座標
		else if (type == "vt")
		{
			glm::vec2 vt(0);
			if (sscanf(p, "%f %f", &vt.x, &vt.y) != 2)
			{
				std::cerr << "[警告]" << __func__ <<
					":テクスチャ座標の読み取りに失敗\n" <<
					" " << filename << "(" << lineNo << "行目): " << 
					line << "\n";
			}
			objTexcoords.push_back(vt);
		}
		// 法線
		else if (type == "vn")
		{
			glm::vec3 vn(0);
			if (sscanf(p, "%f %f %f", &vn.x, &vn.y, &vn.z) != 3)
			{
				std::cerr << "[警告]" << __func__ <<
					":法線の読み取りに失敗\n" << " " <<
					filename << "(" << lineNo << "行目): "
					<< line << "\n";
			}
			objNormals.push_back(vn);
		}
		// 面
		else if (type == "f")
		{
			// 面の頂点データの配列
			std::vector<Index> f;
			for (size_t i = 0; ; ++i)
			{
				// 読み込んだ文字数
				int readBytes = 0;
				Index tmp;
				// 「頂点/テクスチャ座標/法線」バージョンの解析
				if (sscanf(p, "%d/%d/%d %n", &tmp.v, &tmp.vt, &tmp.vn, &readBytes) == 3)
				{
					f.push_back(tmp);
					p += readBytes;
				}
				// 「頂点/テクスチャ座標」バージョンの解析
				else if (sscanf(p, "%d/%d %n", &tmp.v, &tmp.vt, &readBytes) == 2)
				{
					f.push_back(tmp);
					p += readBytes;
				}
				else
				{
					break;
				}
			}

			if (f.size() >= 3)
			{
				for (size_t i = 2; i < f.size(); ++i)
				{
					// 一つ目は0番で固定
					objIndices.push_back(f[0]);
					objIndices.push_back(f[i - 1]);
					objIndices.push_back(f[i]);
				}
			}
			else
			{
				std::cerr << "[警告]" << __func__ <<
					"面データの読み取りに失敗\n" << "  "
					<< filename << "(" << lineNo <<
					"行目): " << line << "\n";
			}
		}
		else
		{
			std::cerr << "[警告]" << __func__ <<
				":未対応の形式です\n" << " " <<
				filename << "(" << lineNo << "行目): "
				<< line << "\n";
		}
	}

	// データ変換用の配列を準備

	// OpenGL用の頂点座標
	std::vector<glm::vec3> positions;
	// OpenGL用の色
	std::vector<glm::vec4> colors;
	// OpenGL用のテクスチャ座標
	std::vector<glm::vec2> texcoords;
	// OpenGL用の法線
	std::vector<glm::vec3> normals;
	// OpenGL用のインデックス
	std::vector<GLushort> indices;

	// データ変換用のメモリを確保
	const size_t indexCount = objIndices.size();
	positions.reserve(indexCount);
	texcoords.reserve(indexCount);
	normals.reserve(indexCount);
	indices.reserve(indexCount);

	// OBJファイルのデータをOpenGLのデータに変換
	for (int i = 0; i < indexCount; ++i)
	{
		// インデックスデータを追加
		indices.push_back(static_cast<GLushort>(i));

		// 頂点座標を変換
		const int v = objIndices[i].v - 1;
		if (v < static_cast<int>(objPositions.size()))
		{
			positions.push_back(objPositions[v]);
		}
		else
		{
			std::cerr << "[警告]" << __func__ <<
				":頂点座標インデックス" << v <<
				"は範囲(0," << objPositions.size() <<
				")の外を指定しています\n" << " " <<
				filename << "\n";
			positions.push_back(glm::vec3(0));
		}

		// テクスチャ座標を変換
		const int vt = objIndices[i].vt - 1;
		if (vt < static_cast<int>(objTexcoords.size()))
		{
			texcoords.push_back(objTexcoords[vt]);
		}
		else
		{
			std::cerr << "[警告]" << __func__ <<
				"テクスチャ座標インデックス\n" << vt <<
				"は範囲(0," << objTexcoords.size() <<
				")の外を指しています\n" << " " << filename << "\n";
			texcoords.push_back(glm::vec2(0));
		}

		// 法線インデックスがないデータの場合、面の頂点座標から法線を計算する
		if (objIndices[i].vn == 0)
		{
			// 面の頂点座標を配列pに取得
			glm::vec3 p[3];
			const size_t n = (i / 3);
			for (size_t j = 0; j < 3; ++j)
			{
				const int v = objIndices[n * 3 + j].v - 1;
				p[j] = objPositions[v];
			}

			// 辺aと辺bを計算
			const glm::vec3 a = p[1] - p[0];
			const glm::vec3 b = p[2] - p[0];

			// aとbに垂直なベクトルを計算
			glm::vec3 normal = glm::cross(a,b);

			// 垂直ベクトルの長さを計算
			const float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

			// 垂直ベクトルを単位ベクトルに変換
			normal = normal / length;

			normals.push_back(normal);
		}
		else
		{
			// 法線を変換
			const int vn = objIndices[i].vn - 1;
			if (vn < static_cast<int>(objNormals.size()))
			{
				normals.push_back(objNormals[vn]);
			}
			else
			{
				std::cerr << "[警告]" << __func__ <<
					":法線インデックス" << vn <<
					"は範囲(0," << objNormals.size() <<
					")の外を指しています\n" <<
					" " << filename << "\n";
				normals.push_back(glm::vec3(0, 1, 0));
			}
		}
	}

	// 色データを設定
	colors.resize(positions.size(), glm::vec4(1));

	// 頂点データとインデックスデータをGPUメモリにコピーする
	const bool result = Add(positions.size(), positions.data(), colors.data(),
		texcoords.data(), normals.data(), indices.size(), indices.data(),filename);
	if (result)
	{
		std::cout << "[情報]" << __func__ <<
			":" << filename << "(頂点数=" <<
			positions.size() << "インデックス数=" <<
			indices.size() << ")\n";
	}
	else
	{
		std::cerr << "[エラー]" << __func__ <<
			":" << filename << "読み込みに失敗\n";
	}

	return result;
}

/**
* プリミティブを取得する
* 
* @param n プリミティブのインデックス
* 
* @retval nに対応するプリミティブ
*/
const Primitive& PrimitiveBuffer::Get(size_t n)const
{
	if (n > static_cast<int>(primitives.size()))
	{
		std::cerr << "[警告]" << __func__ << ":" << n <<
			"は無効なインデックスです(size=" << primitives.size() << ")\n";
		// 仮の描画データを返す
		static const Primitive dummy;
		return dummy;
	}
	return primitives[n];
}

/*
* プリミティブを取得する
* 
* @param name プリミティブの名前
* 
* @return 名前がnameと一致するプリミティブ
*/
const Primitive& PrimitiveBuffer::Find(const char* name)const
{
	for (int i = 0; i < primitives.size(); ++i)
	{
		if (primitives[i].GetName() == name)
		{
			// 名前が一致するデータを見つけた
			return primitives[i];
		}
	}

	// 名前が一致する描画データは見つからなかった
	static const Primitive dummy;
	return dummy;
}

/**
* VAOをグラフィックスパイプラインにバインドする
*/
void PrimitiveBuffer::BindVertexArray()const
{
	glBindVertexArray(vao);
}

/**
* VAOのバインドを解除する
*/
void PrimitiveBuffer::UnbindVectexArray()const
{
	glBindVertexArray(0);
}