#pragma once

/**
* ObjectClass 개요
* - affine 관련 데이터(position, rotate, scale) 관리
* - affine 관련 기능 정의
*/

#include "IObjectClass.h"

namespace Object
{
	class ObjectClass : public IObjectClass
	{
	public:
		ObjectClass() = default;
		virtual ~ObjectClass() = default;

		virtual const DirectX::XMFLOAT4X4 GetAffineMatrix() const override;																// affine matrxi(world matrix) 반환
		virtual const AffineInfo& GetAffine() const override { return m_Affine; }														// affine 데이터 반환

		virtual DirectX::XMFLOAT4 GetPosition() const override { return  m_Affine.position; }											// position 반환
		virtual DirectX::XMFLOAT4 GetRotation() const override { return m_Affine.rotation; }											// rotation 반환
		virtual DirectX::XMFLOAT4 GetScale() const override { return m_Affine.scale; }													// scale 반환
		virtual DirectX::XMFLOAT4 GetColor() const override { return m_Color; }															// color 반환

		virtual void SetPosition(DirectX::XMFLOAT4 pos) override { m_Affine.position = pos; };											// position 대입
		virtual void SetPosition(float x, float y, float z) override { m_Affine.position = DirectX::XMFLOAT4(x, y, z, 1.f); }			// position 대입

		virtual void SetRotation(DirectX::XMFLOAT4 rot) override { m_Affine.rotation = rot; }											// rotation 대입
		virtual void SetRotation(float x, float y, float z) override { m_Affine.rotation = DirectX::XMFLOAT4(x, y, z, 1.f); }			// rotation 대입

		virtual void SetScale(DirectX::XMFLOAT4 scale) override { m_Affine.scale = scale; }												// scale 대입
		virtual void SetScale(float x, float y, float z) override { m_Affine.scale = DirectX::XMFLOAT4(x, y, z, 1.f); }					// scale 대입

		virtual void SetColor(DirectX::XMFLOAT4 color) override { m_Color = color; }													// color 대입
		virtual void SetColor(float r, float g, float b, float a) override { m_Color = DirectX::XMFLOAT4(r, g, b, a); }					// color 대입

		virtual bool Update(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated) override;

		virtual std::unique_ptr<IObjectClass> Clone() const override = 0;

	private:
		AffineInfo m_Affine = { DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f),  DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) };
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	};
}