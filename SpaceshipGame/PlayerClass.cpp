#include "pch.h"
#include "InputClass.h"
#include "IMoveClass.h"
#include "IRotateClass.h"
#include "PlayerClass.h"

Object::PlayerClass::PlayerClass(std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate)
	: CameraClass(std::move(move), std::move(rotate))
{}

Object::PlayerClass::PlayerClass(const PlayerClass & other)
	: CameraClass(other)
{}

Object::PlayerClass::PlayerClass(PlayerClass&& other) noexcept
	: CameraClass(std::move(other))
{}

Object::PlayerClass& Object::PlayerClass::operator=(const PlayerClass& other)
{
	if (this == &other)
		return *this;

	CameraClass::operator=(other);

	return *this;
}

Object::PlayerClass& Object::PlayerClass::operator=(PlayerClass&& other) noexcept
{
	if (this == &other)
		return *this;

	CameraClass::operator=(std::move(other));

	return *this;
}

bool Object::PlayerClass::Update(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated)
{
	bool ret = false;
	System::KEYSTATE state = System::KEYSTATE::NONE;

	// 부모 클래스의 Update() 함수 호출
	if (!CameraClass::Update(input, objects, texts, sounds, frame_time, IsESCPopupWindowActivated))
		return false;

	state = input->GetKeyState(System::KEY::SPACEBAR);
	if (System::KEYSTATE::TAP == state)
		ret = true;

	return ret;
}
