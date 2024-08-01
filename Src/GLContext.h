/**
* @file GLContext.h
*/
#ifndef GLCONTEXT_H_INCLUDED
#define GLCONTEXT_H_INCLUDED

#include <glad/glad.h>

/// 三次元座標型
struct Position
{
	float x, y, z;

	/**
	* コンストラクタ
	*/
	Position()
		:x(0)
		,y(0)
		,z(0)
	{}

	Position(Position& pos)
		:x(pos.x)
		,y(pos.y)
		,z(pos.z)
	{}

	/**
	* 演算(+ - * /)
	*/
	Position operator +(const Position& pos)
	{
		Position p;
		p.x = x+pos.x;
		p.y = y+pos.y;
		p.z =z+ pos.z;
		return p;
	}
	Position operator -(const Position& pos)
	{
		Position p;
		p.x=x - pos.x;
		p.y=y - pos.y;
		p.z=z - pos.z;
		return p;
	}
	Position operator *(const Position& pos)
	{
		Position p;
		p.x=x * pos.x;
		p.y=y * pos.y;
		p.z=z * pos.z;
		return p;
	}
	Position operator /(const Position& pos)
	{
		Position p;
		p.x=x / pos.x;
		p.y=y / pos.y;
		p.z=z / pos.z;
		return p;
	}
	/**
	* 代入演算子(+= -= *= /= =)
	*/
	Position& operator +=(const Position& pos)
	{
		x += pos.x;
		y += pos.y;
		z += pos.z;
		return *this;
	}
	Position& operator -=(const Position& pos)
	{
		x -= pos.x;
		y -= pos.y;
		z -= pos.z;
		return *this;
	}
	Position& operator *=(const Position& pos)
	{
		x *= pos.x;
		y *= pos.y;
		z *= pos.z;
		return *this;
	}
	Position& operator /=(const Position& pos)
	{
		x /= pos.x;
		y /= pos.y;
		z /= pos.z;
		return *this;
	}
	Position& operator =(const Position& pos)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;
		return *this;
	}
	/**
	* インクリメント、デクリメント(Position++ ++Position Position-- --Position)
	*/
	Position& operator ++()
	{
		x++;
		y++;
		z++;
		return *this;
	}
	Position& operator ++(int)
	{
		++x;
		++y;
		++z;
		return *this;
	}
	Position& operator --()
	{
		x--;
		y--;
		z--;
		return *this;
	}
	Position& operator --(int)
	{
		--x;
		--y;
		--z;
		return *this;
	}
	/**
	* 比較演算子(==, <, <=, >, >=, !=)
	*/
	bool operator ==(const Position& pos)
	{
		return x == pos.x && y == pos.y && z == pos.z;
	}
	bool operator <(const Position& pos)
	{
		return x < pos.x && y < pos.y && z < pos.z;
	}
	bool operator <=(const Position& pos)
	{
		return x <= pos.x && y <= pos.y && z <= pos.z;
	}
	bool operator >(const Position& pos)
	{
		return x > pos.x && y > pos.y && z > pos.z;
	}
	bool operator >=(const Position& pos)
	{
		return x >= pos.x && y >= pos.y && z >= pos.z;
	}
	bool operator !=(const Position& pos)
	{
		return x != pos.x && y != pos.y && z != pos.z;
	}
	/**
	* 論理演算子(&&, ||, !)
	*/
	bool operator &&(const Position& pos)
	{
		return (x && pos.x) && (y && pos.y) && (z && pos.z);
	}
	bool operator ||(const Position& pos)
	{
		return (x || pos.x) && (y || pos.y) && (z || pos.z);
	}
	bool operator !()
	{
		return !x && !y && !z;
	}
};

/// RGBAカラー型
struct Color
{
	float r, g, b, a;
};

namespace GLContext 
{

	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data);
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor,
		GLuint vboTexcoord,GLuint vboNormal, GLuint ibo);
	GLuint CreateProgram(GLenum type, const GLchar* code);
	GLuint CreateProgramFromFile(GLenum type, const char* filename);
	GLuint CreatePipeline(GLuint vp, GLuint fp);
	GLuint CreateSampler(GLenum wrapMode);
	GLuint CreateImage2D(GLsizei width, GLsizei height, const void* data,GLuint pixelFormat, GLenum type);
	GLuint CreateImage2D(const char* filename);

} // namespace GLContext

#endif // GLCONTEXT_H_INCLUDED