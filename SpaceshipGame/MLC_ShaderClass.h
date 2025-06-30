#pragma once
#include "ShaderClass.h"

namespace Graphic
{
	namespace Shader
	{
		class MLC_ShaderClass : public ShaderClass<MLC_ShaderBuffers>
		{
		public:
			explicit MLC_ShaderClass(ID ShaderID);
			virtual ~MLC_ShaderClass() = default;

		private:
			// shader에서 사용하는 buffer들을 생성하는 함수
			virtual HRESULT CreateBuffers(ID3D11Device* Device) override;

		private:
			// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
			virtual void SetShaderBuffers(ID3D11DeviceContext* DeviceContext, const MLC_ShaderBuffers& ShaderBufferDatas) override;

			// Matrix buffer를 업데이트 하는 함수 //
			HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const MatrixBufferType& transform);

			// Light buffer를 업데이트 하는 함수 //
			HRESULT UpdateLightBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const LightBufferType& light);

			// camera buffer를 업데이트 하는 함수 //
			HRESULT UpdateCameraBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const CameraBufferType& camera);

		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_MatrixBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_CameraBuffer = nullptr;
		};
	}
}