#pragma once

#include "CameraClass.h"

namespace Object
{
	class PlayerClass : public CameraClass
	{
	public:
		PlayerClass(std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate);
		PlayerClass(const PlayerClass& other);
		PlayerClass(PlayerClass&& other) noexcept;
		virtual ~PlayerClass() = default;

		PlayerClass& operator=(const PlayerClass& other);
		PlayerClass& operator=(PlayerClass&& other) noexcept;

		virtual bool Update(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated) override;

		virtual std::unique_ptr<IObjectClass> Clone() const override { return std::make_unique<PlayerClass>(*this); }
	};
}