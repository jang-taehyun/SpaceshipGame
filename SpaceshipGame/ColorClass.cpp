#include "pch.h"
#include "ColorClass.h"

ColorClass::ColorClass(const ColorClass& other)
{
	*this = other;
}

ColorClass::ColorClass(const float&& r, const float&& g, const float&& b, const float&& a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

ColorClass::~ColorClass() {}
