﻿/**
* @file GLContext.cpp
*/
#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "GLContext.h"

/**
* OpenGLコンテキストに関する機能を格納する名前空間
*/
namespace GLContext 
{
	/**
	* バッファオブジェクトを作成する
	*
	* @param size データのサイズ
	* @param data データへのポインタ
	*
	* @return 作成したバッファオブジェクト
	*/
	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data)
	{
		GLuint id = 0;
		// OpenGLの管理下にあるメモリ領域を扱うためのオブジェクトを作成
		glCreateBuffers(1, &id);
		// メモリを確保しデータを転送する
		glNamedBufferStorage(id, size, data, 0);
		return id;
	}

	/**
	* Vertex Array Objectを作成する
	*
	* @param vboPosition VAOに関連付けられる座標データ
	* @param vboColor    VAOに関連付けられるカラーデータ
	* @param vboTexcoord VAOに関連付けられるテクスチャ座標データ
	* @param vboNormal	 VBOに関連付けられる法線データ
	* @param ibo         VAOに関連付けられるインデックスデータ
	*
	* @return 作成したVAO
	*/
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor,
		GLuint vboTexcoord,GLuint vboNormal, GLuint ibo)
	{
		if (!vboPosition || !vboColor || !vboTexcoord || !vboNormal || !ibo) {
			std::cerr
				<< "[エラー]"
				<< __func__
				<< ":バッファオブジェクトが0です。\n";
			return 0;
		}

		GLuint id = 0;
		/*OVAを作成*/
		glCreateVertexArrays(1, &id);
		//有効にする頂点アトリビュートの番号
		const GLuint positionIndex = 0;
		//割り当てるバインディング・ポイントの番号
		const GLuint positionBindingIndex = 0;
		//------------------------
		//頂点アトリビュートを設定
		glEnableVertexArrayAttrib(id, positionIndex);
		/*(VAOのID, 頂点アトリビュートの番号,
		要素に含まれるデータ数, データの型,
		正規化の有無, 相対オフセット値)*/
		glVertexArrayAttribFormat(id, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
		/*(VAOのID, 頂点アトリビュートの番号,  
		バインディング・ポイントの番号)*/
		glVertexArrayAttribBinding(id, positionIndex, positionBindingIndex);
		//------------------------
		/*頂点データをバインディング・ポイントに割り当てる
		(VAOのID, バインディング・ポイントの番号, 
		バッファ・オブジェクトのID, 最初の要素の位置, 
		要素間の距離)*/
		glVertexArrayVertexBuffer(id, positionBindingIndex, vboPosition, 0, sizeof(Position));

		const GLuint colorIndex = 1;
		const GLuint colorBindingIndex = 1;
		glEnableVertexArrayAttrib(id, colorIndex);
		glVertexArrayAttribFormat(id, colorIndex, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, colorIndex, colorBindingIndex);
		glVertexArrayVertexBuffer(id, colorBindingIndex, vboColor, 0, sizeof(Color));

		//テクスチャ座標データをバインディングポイントに割り当てる
		const GLuint texcoordIndex = 2;
		const GLuint texcoordBindingIndex = 2;
		glEnableVertexArrayAttrib(id, texcoordIndex);
		glVertexArrayAttribFormat(id, texcoordIndex, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, texcoordIndex, texcoordBindingIndex);
		glVertexArrayVertexBuffer(id, texcoordBindingIndex, vboTexcoord, 0, sizeof(glm::vec2));

		const GLuint nomalIndex = 3;
		const GLuint nomalBindingIndex = 3;
		glEnableVertexArrayAttrib(id, nomalIndex);
		glVertexArrayAttribFormat(id, nomalIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, nomalIndex, nomalBindingIndex);
		glVertexArrayVertexBuffer(id, nomalBindingIndex, vboNormal, 0, sizeof(glm::vec3));

		glVertexArrayElementBuffer(id, ibo);

		return id;
	}

	/**
	* シェーダー・プログラムをビルドする
	*
	* @param type シェーダーの種類
	* @param code シェーダー・プログラムへのポインタ
	*
	* @retval 0より大きい 作成したプログラム・オブジェクト
	* @retval 0          プログラム・オブジェクトの作成に失敗
	*/
	GLuint CreateProgram(GLenum type, const GLchar* code)
	{
		GLuint program = glCreateShaderProgramv(type, 1, &code);

		GLint status = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				std::vector<char> buf;
				buf.resize(infoLen);
				if ((int)buf.size() >= infoLen)
				{
					glGetProgramInfoLog(program, infoLen, nullptr, buf.data());
					std::cerr << "[エラー]" << __func__ <<
					":シェーダーのビルドに失敗.\n" << buf.data() << "\n";
				}
			}
			glDeleteProgram(program);
			return 0;
		}
		return program;
	}

	/**
	* ファイルからシェーダー・プログラムを作成する
	* 
	* @param type		シェーダーの種類
	* @param filename	シェーダーファイル名
	* 
	* @retval 0より大きい	作成したプログラム・オブジェクト
	* @retval 0				プログラム・オブジェクトの作成に失敗
	*/
	GLuint CreateProgramFromFile(GLenum type, const char* filename)
	{
		std::ifstream ifs(filename);
		if (!ifs)
		{
			std::cerr << "[エラー]" << __func__ <<
				":" << filename << "を開けません\n";
		}
		std::stringstream ss;
		ss << ifs.rdbuf();
		return GLContext::CreateProgram(type, ss.str().c_str());
	}

	/**
	* パイプライン・オブジェクトを作成する
	*
	* @param vp  頂点シェーダー・プログラム
	* @param fp  フラグメントシェーダー・プログラム
	*
	* @retval 0より大きい 作成したパイプライン・オブジェクト
	* @retval 0         パイプライン・オブジェクトの作成に失敗
	*/
	GLuint CreatePipeline(GLuint vp, GLuint fp)
	{
		// エラー状態をリセット
		glGetError();

		GLuint id = 0;
		glCreateProgramPipelines(1, &id);
		glUseProgramStages(id, GL_VERTEX_SHADER_BIT, vp);
		glUseProgramStages(id, GL_FRAGMENT_SHADER_BIT, fp);

		if (glGetError() != GL_NO_ERROR) 
		{
			std::cerr << "[エラー]" << __func__ << ":プログラムパイプラインの作成に失敗.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}

		GLint testVp = 0;
		glGetProgramPipelineiv(id, GL_VERTEX_SHADER, &testVp);
		
		if (testVp != vp) 
		{
			std::cerr << "[エラー]" << __func__ << ":頂点シェーダの設定に失敗.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
			
		}

		GLint testFp = 0;
		glGetProgramPipelineiv(id, GL_FRAGMENT_SHADER, &testFp);
		
		if (testFp != fp) 
		{
			std::cerr << "[エラー]" << __func__ << ":フラグメントシェーダの設定に失敗.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}

		return id;
	}

	/**
	* サンプラ・オブジェクトを作成する
	*
	* @param wrapMode	ラップモード
	*
	* @retval 0以外		作成したサンプラ・オブジェクト
	* @retval 0			サンプラ・オブジェクトの作成に失敗
	*/
	GLuint CreateSampler(GLenum wrapMode)
	{
		glGetError();	//エラー状態をリセット

		//サンプラ・オブジェクトを作成する
		GLuint id;
		glCreateSamplers(1, &id);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ << "サンプラの作成に失敗\n";
			glDeleteSamplers(1, &id);
			return 0;
		}

		//ラップ・モードを設定する
		glSamplerParameteri(id, GL_TEXTURE_WRAP_S, wrapMode);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_T, wrapMode);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ << "ラップ・モードではない値が指定されました\n";
			glDeleteSamplers(1, &id);
			return 0;
		}

		//フィルターを設定する
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ << ":フィルタではない値が指定されました\n";
			glDeleteSamplers(1, &id);
			return 0;
		}

		return id;
	}

	/**
	* 2Dテクスチャを作成する
	* 
	* @param width			画像の幅(ピクセル数)
	* @param height			画像の高さ(ピクセル数)
	* @param data			画像のデータのアドレス
	* @param pixelFormat	画像のピクセルデータ(GL_BGRAなど)
	* @param type			画像データの型
	* 
	* @retval 0以外　作成したテクスチャ・オブジェクトのID
	* @retval 0		 テクスチャの作成に失敗
	*/
	GLuint CreateImage2D(GLsizei width, GLsizei height, const void* data, GLuint pixelFormat, GLenum type)
	{
		glGetError();

		// 画像データ形式に対応する内部データ形式を選択する
		// ※ここに書いてない形式を使うには対応するcase文を追加する
		GLenum internalFormat = GL_RGBA8;
		switch (pixelFormat)
		{
		case GL_RED: internalFormat = GL_R8; break;
		case GL_RGB: internalFormat = GL_RGB8; break;
		case GL_BGR: internalFormat = GL_RGB8; break;
		case GL_DEPTH_COMPONENT16:
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32F:
			internalFormat = pixelFormat;
			break;
		}

		// テクスチャ・オブジェクトを作成し、GPUメモリを確保する
		GLuint id;
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, internalFormat, width, height);

		// 画像データがある場合、GPUメモリにデータを転送する
		GLint alignment;
		if (data)
		{
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// GPUメモリデータを転送する
			glTextureSubImage2D(id, 0, 0, 0, width, height, pixelFormat, type, data);
			// アラインメントを元に戻す
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		}

		const GLenum result = glGetError();
		if (result != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ << "テクスチャの作成に失敗\n";
			glDeleteTextures(1, &id);
			return 0;
		}

		//白黒画像の場合、(R,R,R,1)として読み取られるように設定する
		if (pixelFormat == GL_RED)
		{
			glTextureParameteri(id, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTextureParameteri(id, GL_TEXTURE_SWIZZLE_B, GL_RED);
		}


		return id;
	}

	/**
	* ファイルから2Dテクスチャを読み込む
	* 
	* @param filename 2Dテクスチャとして読み込むファイル名
	* 
	* @retval	0以外　作成したテクスチャ・オブジェクトのID
	*			0	   テクスチャの作成に失敗
	*/
	GLuint CreateImage2D(const char* filename)
	{
		//ファイルを開く
		std::ifstream ifs;
		ifs.open(filename, std::ios_base::binary);
		if (!ifs)
		{
			std::cerr << "[エラー]" << __func__ << ":`" << filename << "を開けません\n";
			return 0;
		}

		//TGAヘッダを読み込む
		uint8_t tgaHeader[18]; //TGAヘッダは18バイト
		ifs.read(reinterpret_cast<char*>(tgaHeader), 18);

		//イメージIDを読み飛ばす
		ifs.ignore(tgaHeader[0]);

		//カラーマップを読み飛ばす
		if (tgaHeader[1])
		{
			const int colorMapLength = tgaHeader[5] + tgaHeader[6] * 0x100;
			const int colorMapEntrySize = tgaHeader[7];
			//エントリーサイズはビット数なので、8で割ってバイト数に変換する
			const int colorMapSize = (colorMapLength * colorMapEntrySize + 7) / 8;
			ifs.ignore(colorMapSize);
		}

		//画像データを読み込む
		const int width = tgaHeader[12] + tgaHeader[13] * 0x100;
		const int height = tgaHeader[14] + tgaHeader[15] * 0x100;
		const int pixelDepth = tgaHeader[16];
		const int imageSize = width * height * pixelDepth / 8;
		std::vector<uint8_t> buf(imageSize);
		ifs.read(reinterpret_cast<char*>(buf.data()), imageSize);

		// TGAヘッダ18バイト目の第5ビットは、画像データの格納方向を表す
		// 0の場合: 下から上
		// 1の場合: 上から下
		// OpenGLは画像データを「下から上」に格納するルールになっているので、
		// TGAが「上から下」に格納されている場合は画像を上下反転する
		if (tgaHeader[17] & 0x20)
		{
			const size_t lineSize = width * pixelDepth / 8;
			std::vector<uint8_t> tmp(imageSize);
			std::vector<uint8_t>::iterator source = buf.begin();
			std::vector<uint8_t>::iterator destination = tmp.end();
			for (size_t i = 0; i < height; ++i)
			{
				destination -= lineSize;
				std::copy(source, source + lineSize, destination);
				source += lineSize;
			}
			buf.swap(tmp);
		}

		//データの型を選ぶ
		GLenum type = GL_UNSIGNED_BYTE;
		if (tgaHeader[16] == 16)
		{
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		}

		//ピクセル形式を選ぶ
		GLenum pixelFormat = GL_BGRA;
		if (tgaHeader[2] == 3)//白黒データ
		{
			pixelFormat = GL_RED;
		}

		if (tgaHeader[16] == 24)
		{
			pixelFormat = GL_BGR;
		}

		//読み込んだ画像データからテクスチャを作成する
		return CreateImage2D(width, height, buf.data(), pixelFormat, type);
	}

} // namespace GLContext