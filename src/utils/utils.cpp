#include "utils.h"

#include <ostream>
#include <cmath>
#include <random>

/************************************************************************************************************
* Implementarea functiilor/metodelor care opereaza cu vectori.
* Sunt implementati si operatori pentru usurinta.
*************************************************************************************************************/

/************************************************************************************************************
*													Vec4f
*************************************************************************************************************/
Vec4f::Vec4f(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4f::Vec4f(const Vec2f& v2, float z, float w)
{
	this->x = v2.x;
	this->y = v2.y;
	this->z = z;
	this->w = w;
}

Vec4f operator*(const Vec4f& v, float s)
{
	return Vec4f(v.x * s, v.y * s, v.z * s);
}

Vec4f operator*(float s, const Vec4f& v)
{
	return v * s;
}

Vec4f operator/(const Vec4f& v, float s)
{
	return Vec4f(v.x / s, v.y / s, v.z / s);
}

Vec4f operator+(const Vec4f& lhs, const Vec4f& rhs)
{
	return Vec4f(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec4f operator-(const Vec4f& lhs, const Vec4f& rhs)
{
	return Vec4f(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

std::ostream& operator<<(std::ostream& out, const Vec4f& v)
{
	out << "(" << v.x << ", " << v.y << ", " << v.z << ")";

	return out;
}

float Vec4f::dot(const Vec4f& a, const Vec4f& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec4f Vec4f::cross(const Vec4f& a, const Vec4f& b)
{
	/*
	*  |  1   1   1  |
	*  | a.x a.y a.z |
	*  | b.x b.y b.z |
	*/

	return Vec4f(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
}

float Vec4f::length(const Vec4f& v)
{
	return sqrtf(Vec4f::dot(v, v));
}

float Vec4f::length2(const Vec4f& v)
{
	return Vec4f::dot(v, v);
}

Vec4f Vec4f::normalize(const Vec4f& v)
{
	return v / Vec4f::length(v);
}

Vec4f Vec4f::reflect(const Vec4f& v, const Vec4f& n)
{
	return Vec4f(v - 2.0f * Vec4f::dot(v, n) * n);
}

/************************************************************************************************************
*													Vec2f
*************************************************************************************************************/
Vec2f::Vec2f(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vec2f::Vec2f(const Vec4f& v4)
{
	this->x = v4.x;
	this->y = v4.y;
}

Vec2f operator*(const Vec2f& v, float s)
{
	return Vec2f(v.x * s, v.y * s);
}

Vec2f operator*(float s, const Vec2f& v)
{
	return v * s;
}

Vec2f operator/(const Vec2f& v, float s)
{
	return Vec2f(v.x / s, v.y / s);
}

Vec2f operator+(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vec2f operator-(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f(lhs.x - rhs.x, lhs.y - rhs.y);
}

std::ostream& operator<<(std::ostream& out, const Vec2f& v)
{
	out << "(" << v.x << ", " << v.y  << ")";

	return out;
}

float Vec2f::dot(const Vec2f& a, const Vec2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vec2f::det(const Vec2f& a, const Vec2f& b)
{
	return a.x * b.y - a.y * b.x;
}

Vec4f Vec2f::cross(const Vec2f& a, const Vec2f& b)
{
	/*
	*  |  1   1   1  |
	*  | a.x a.y a.z |
	*  | b.x b.y b.z |
	*/

	return Vec4f(0.0f, 0.0f, a.x * b.y - a.y * b.x);
}

float Vec2f::length(const Vec2f& v)
{
	return sqrtf(Vec2f::dot(v, v));
}

float Vec2f::length2(const Vec2f& v)
{
	return Vec2f::dot(v, v);
}

Vec2f Vec2f::normalize(const Vec2f& v)
{
	return v / Vec2f::length(v);
}

Vec2f Vec2f::reflect(const Vec2f& v, const Vec2f& n)
{
	return Vec2f(v - 2.0f * Vec2f::dot(v, n) * n);
}

float Vec2f::angleRad(const Vec2f& a, const Vec2f& b)
{
	return atan2f(Vec2f::det(a, b), Vec2f::dot(a, b));
}

float Vec2f::angleDeg(const Vec2f& a, const Vec2f& b)
{
	return Vec2f::angleRad(a, b) * (180.0f / (float)M_PI);
}

/************************************************************************************************************
*													Boundary2f
*************************************************************************************************************/

Boundary2f::Boundary2f()
{
}

Boundary2f::Boundary2f(const Vec2f& min, const Vec2f& max)
{
	this->min = min;
	this->max = max;
}

Boundary2f::Boundary2f(float minx, float miny, float maxx, float maxy)
{
	min.x = minx;
	min.y = miny;
	max.x = maxx;
	max.y = maxy;
}

bool Boundary2f::contains(const Vec2f& point)
{
	return ((min.x <= point.x) && (max.x >= point.x) && (min.y <= point.y) && (max.y >= point.y));
}

bool Boundary2f::contains(float x, float y)
{
	return contains(Vec2f(x, y));
}

/************************************************************************************************************
*													CircleBoundary2f
*************************************************************************************************************/

CircleBoundary2f::CircleBoundary2f()
{
	radius = 1.0f;
}

CircleBoundary2f::CircleBoundary2f(const Vec2f& position, float radius)
{
	this->position = position;
	this->radius = radius;
}

CircleBoundary2f::CircleBoundary2f(float x, float y, float radius)
{
	this->position = Vec2f(x, y);
	this->radius = radius;
}

bool CircleBoundary2f::contains(const Vec2f& point)
{
	return (Vec2f::length2(point - position) <= radius * radius);
}

bool CircleBoundary2f::contains(float x, float y)
{
	return contains(Vec2f(x, y));
}

/************************************************************************************************************
* Implementarea altor functii.
*************************************************************************************************************/

int rand_int(int min, int max)
{
	return min + std::rand() % (max - min + 1);
}

float rand_float(float min, float max)
{
	return min + static_cast<float>(std::rand() % 1024) / 1023.f * (max - min);
}

Vec4f rand_color()
{
	return Vec4f(rand_float(), rand_float(), rand_float());
}

Vec2f rand_direction()
{
	return Vec2f::normalize(Vec2f(rand_float(-1.0f, 1.0f), rand_float(-1.0f, 1.0f)));
}

void drawText(Vec2f pos, const char* text, Vec4f color, void* font)
{
	int h = glutBitmapHeight(font);
	glColor3f(color.x, color.y, color.z);
	glRasterPos2f(pos.x, pos.y + static_cast<float>(h));
	glutBitmapString(font, reinterpret_cast<const unsigned char*>(text));
}

void drawText(Vec2f pos, const std::string& text, Vec4f color, void* font)
{
	int h = glutBitmapHeight(font);
	glColor3f(color.x, color.y, color.z);
	glRasterPos2f(pos.x, pos.y + static_cast<float>(h));
	glutBitmapString(font, reinterpret_cast<const unsigned char*>(text.c_str()));
}

void glColorVec4f(const Vec4f& color)
{
	glColor4f(color.x, color.y, color.z, color.w);
}

void glVertexVec2f(const Vec2f& vec)
{
	glVertex2f(vec.x, vec.y);
}

void glVertexVec4f(const Vec4f& vec)
{
	glVertex4f(vec.x, vec.y, vec.z, vec.w);
}
