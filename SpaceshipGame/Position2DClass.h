#pragma once

class Position2DClass
{
public:
	Position2DClass();
	Position2DClass(const Position2DClass& other);
	~Position2DClass();

	// ---- Getter ---- //

	inline const float& GetPositionX() const { return x; }
	inline const float& GetPositionY() const { return y; }

	// -------------------- //


	// ---- Setter ---- //

	inline void SetPositionX(const float& value) { x = value; }
	inline void SetPositionY(const float& value) { y = value; }

	// -------------------- //


	// ---- operator overloading ---- //

	inline const Position2DClass& operator=(const Position2DClass& ref)
	{
		this->x = ref.x;
		this->y = ref.y;

		return (*this);
	}

	// -------------------- //

private:
	float x = 0.f;
	float y = 0.f;
};

