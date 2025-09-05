#pragma once

namespace System { class InputClass; }
namespace Object { class ObjectManagerClass; }
namespace Sound { class SoundManagerClass; }
namespace Text { class TextManagerClass; }

namespace Object
{
	class IObjectClass
	{
	public:
		virtual ~IObjectClass() = default;

		virtual const DirectX::XMFLOAT4X4 GetAffineMatrix() const = 0;			// affine matrix(world matrix) 반환
		virtual const AffineInfo& GetAffine() const = 0;						// affine 데이터 반환

		virtual DirectX::XMFLOAT4 GetPosition() const = 0;						// position 반환
		virtual DirectX::XMFLOAT4 GetRotation() const = 0;						// rotation 반환
		virtual DirectX::XMFLOAT4 GetScale() const = 0;							// scale 반환
		virtual DirectX::XMFLOAT4 GetColor() const = 0;							// color 반환

		virtual void SetPosition(DirectX::XMFLOAT4 pos) = 0;					// position 대입
		virtual void SetPosition(float x, float y, float z) = 0;				// position 대입
			    
		virtual void SetRotation(DirectX::XMFLOAT4 rot) = 0;					// rotation 대입
		virtual void SetRotation(float x, float y, float z) = 0;				// rotation 대입
			    
		virtual void SetScale(DirectX::XMFLOAT4 scale) = 0;						// scale 대입
		virtual void SetScale(float x, float y, float z) = 0;					// scale 대입

		virtual void SetColor(DirectX::XMFLOAT4 color) = 0;						// color 대입
		virtual void SetColor(float r, float g, float b, float a) = 0;			// color 대입

		virtual void Update(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated) = 0;
			    
		virtual std::unique_ptr<IObjectClass> Clone() const = 0;
	};
}