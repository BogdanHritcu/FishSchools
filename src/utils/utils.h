#pragma once

#include <string>
#include <GL/freeglut.h>

#define M_PI 3.14159265358979

/************************************************************************************************************
* Vector cu 4 componente de tip float.
* Metodele/functiile care lucreaza cu acet tip de vector iau in considerare doar primele trei componente.
*************************************************************************************************************/
struct Vec2f;

struct Vec4f
{
	float x;
	float y;
	float z;
	float w;

	Vec4f(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	Vec4f(const Vec2f& v2, float z = 0.0f, float w = 1.0f);

	static float dot(const Vec4f& a, const Vec4f& b);
	static Vec4f cross(const Vec4f& a, const Vec4f& b);
	static float length(const Vec4f& v);
	static float length2(const Vec4f& v);
	static Vec4f normalize(const Vec4f& v);
	static Vec4f reflect(const Vec4f& v, const Vec4f& n);
};

Vec4f operator*(const Vec4f& v, float s);
Vec4f operator*(float s, const Vec4f& v);
Vec4f operator/(const Vec4f& v, float s);
Vec4f operator+(const Vec4f& lhs, const Vec4f& rhs);
Vec4f operator-(const Vec4f& lhs, const Vec4f& rhs);
std::ostream& operator<<(std::ostream& out, const Vec4f& v);

struct Vec2f
{
	float x;
	float y;

	Vec2f(float x = 0.0f, float y = 0.0f);
	Vec2f(const Vec4f& v4);

	static float dot(const Vec2f& a, const Vec2f& b);
	static float det(const Vec2f& a, const Vec2f& b);
	static Vec4f cross(const Vec2f& a, const Vec2f& b);
	static float length(const Vec2f& v);
	static float length2(const Vec2f& v);
	static Vec2f normalize(const Vec2f& v);
	static Vec2f reflect(const Vec2f& v, const Vec2f& n);
	static float angleRad(const Vec2f& a, const Vec2f& b);
	static float angleDeg(const Vec2f& a, const Vec2f& b);
};

Vec2f operator*(const Vec2f& v, float s);
Vec2f operator*(float s, const Vec2f& v);
Vec2f operator/(const Vec2f& v, float s);
Vec2f operator+(const Vec2f& lhs, const Vec2f& rhs);
Vec2f operator-(const Vec2f& lhs, const Vec2f& rhs);
std::ostream& operator<<(std::ostream& out, const Vec2f& v);

struct Boundary2f
{
	Vec2f min;
	Vec2f max;

	Boundary2f();
	Boundary2f(const Vec2f& min, const Vec2f& max);
	Boundary2f(float minx, float miny, float maxx, float maxy);
	
	bool contains(const Vec2f& point);
	bool contains(float x, float y);
};

struct CircleBoundary2f
{
	Vec2f position;
	float radius;

	CircleBoundary2f();
	CircleBoundary2f(const Vec2f& position, float radius);
	CircleBoundary2f(float x, float y, float radius);

	bool contains(const Vec2f& point);
	bool contains(float x, float y);
};

/************************************************************************************************************
* Alte functii utile.
*************************************************************************************************************/

int rand_int(int min, int max);
float rand_float(float min = 0.0f, float max = 1.0f);
Vec4f rand_color();
Vec2f rand_direction();

void drawText(Vec2f pos, const char* text, Vec4f color = Vec4f(1.0f, 1.0f, 1.0f), void* font = GLUT_BITMAP_8_BY_13);
void drawText(Vec2f pos, const std::string& text, Vec4f color = Vec4f(1.0f, 1.0f, 1.0f), void* font = GLUT_BITMAP_8_BY_13);

void glColorVec4f(const Vec4f& color);
void glVertexVec2f(const Vec2f& vec);
void glVertexVec4f(const Vec4f& vec);