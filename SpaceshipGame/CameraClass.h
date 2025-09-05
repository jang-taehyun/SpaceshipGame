#pragma once

/**
* CameraClass °³¿ä
*/

#include <DirectXCollision.h>
#include "ObjectClass.h"

namespace Object
{
	class IMoveClass;
	class IRotateClass;
}

namespace Object
{
	class CameraClass : public ObjectClass
	{
	public:
		CameraClass(std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate);
		CameraClass(const CameraClass& other);
		CameraClass(CameraClass&& other) noexcept;
		virtual ~CameraClass() = default;

		DirectX::XMFLOAT4X4 GetViewMatrix() const { return m_ViewMatrix; }

		void UpdateFrustum(DirectX::XMFLOAT4X4 projection);
		DirectX::XMFLOAT4X4 Render();

		bool IsRenderModel(DirectX::BoundingOrientedBox ModelOBB, DirectX::XMFLOAT4X4 ModelWorldMatrix) const;
		bool IsRenderQuadTree(DirectX::XMFLOAT4 point1, DirectX::XMFLOAT4 point2);

		void Move(MoveState state, float frame_time, bool IsKeyDown);
		void Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown);

		CameraClass& operator=(const CameraClass& other);
		CameraClass& operator=(CameraClass&& other) noexcept;

		virtual void Update(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated) override;

		virtual std::unique_ptr<IObjectClass> Clone() const override { return std::make_unique<CameraClass>(*this); }

	private:
		DirectX::BoundingFrustum m_Frustum;
		DirectX::XMFLOAT4X4 m_ViewMatrix = DirectX::XMFLOAT4X4();
		std::unique_ptr<IMoveClass> m_Move = nullptr;
		std::unique_ptr<IRotateClass> m_Rotate = nullptr;
	};
}