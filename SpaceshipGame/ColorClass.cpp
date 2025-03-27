#include "pch.h"
#include "ColorClass.h"

ColorClass::ColorClass() {}

ColorClass::ColorClass(const ColorClass& other)
{
	*this = other;
}

ColorClass::ColorClass(const float&& r, const float&& g = 0.f, const float&& b = 0.f, const float&& a = 1.f)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

ColorClass::~ColorClass() {}
