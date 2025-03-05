#pragma once

class ColorClass
{
public:
	ColorClass();
	ColorClass(const ColorClass& other);
	~ColorClass();

	// ---- Getter ---- //

	inline const float& GetColorRed() const { return red; }
	inline const float& GetColorGreen() const { return green; }
	inline const float& GetColorBlue() const { return blue; }
	inline const float& GetColorAlpha() const { return alpha; }

	// -------------------- //

	// ---- Setter ---- //

	inline void SetColorRed(const float& value) { red = value; }
	inline void SetColorBlue(const float& value) { blue = value; }
	inline void SetColorGreen(const float& value) { green = value; }
	inline void SetColorAlpha(const float& value) { alpha = value; }

	// -------------------- //

	// ---- operator overloading ---- //

	inline const ColorClass& operator=(const ColorClass& ref)
	{
		this->red = ref.red;
		this->green = ref.green;
		this->blue = ref.blue;
		this->alpha = ref.alpha;

		return (*this);
	}

	// -------------------- //

private:
	float red = 0.f;
	float green = 0.f;
	float blue = 0.f;
	float alpha = 1.f;
};

