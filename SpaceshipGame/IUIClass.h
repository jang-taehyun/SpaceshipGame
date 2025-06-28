#pragma once

class TextureManagerClass;

namespace UI
{
	class IUIClass
	{
	public:
		virtual ~IUIClass() = default;

		virtual inline DirectX::XMFLOAT2 GetPosition() const = 0;
		virtual inline DirectX::XMFLOAT4 GetColor() const = 0;
		virtual inline void SetPosition(DirectX::XMFLOAT2 pos) = 0;
		virtual inline void SetColor(DirectX::XMFLOAT4 color) = 0;

		virtual void Update(bool IsLeftMouseButtonPressed) = 0;
	};
}