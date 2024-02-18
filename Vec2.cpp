#include "Vec2.h"
#include <iostream>
#include <cmath>

Vec2::Vec2() 
{}

Vec2::Vec2(float xin, float yin)
	: x(xin)
	, y(yin) 
{}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const float val) const
{

	return Vec2(x * val, y * val);
}

bool Vec2::operator == (const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

void Vec2::operator += (const Vec2& rhs) 
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
	x *= val;
	y *= val;
}

void Vec2::operator /= (const float val)
{
	x /= val;
	y /= val;
}

double Vec2::dist(const Vec2& rhs) const
{
	Vec2 d(x - rhs.x, y - rhs.y);
	return sqrtf((d.x * d.x) + (d.y * d.y));
}

double Vec2::length() const
{
	return sqrtf((x * x) + (y * y));
}

void Vec2::normalize()
{
	double L = std::sqrt((x * x) + (y * y));
	x = x / L;
	y = y / L;
}

std::ostream& operator << (std::ostream& stream, const Vec2& vec)
{
	return stream
		<< vec.x
		<< " "
		<< vec.y
		<< '\n';
}