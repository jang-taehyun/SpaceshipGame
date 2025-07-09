#pragma once

#include <DirectXCollision.h>

namespace Graphic
{
	namespace Model
	{
		class IModelClass
		{
		public:
			virtual ~IModelClass() = default;

			virtual inline void AddWorldMatrix(const InstanceBufferType& world) = 0;

			virtual void UpdateInstanceBuffer(ID3D11DeviceContext* DeviceContext) = 0;
			virtual void RenderMesh(ID3D11DeviceContext* DeviceContext, UINT MeshIdx) = 0;

			virtual inline UINT GetMeshCount() const = 0;
			virtual inline Shader::ID GetShaderID() const = 0;
			virtual inline ULONG GetIndexCount(UINT idx) const = 0;
			virtual inline ULONG GetInstanceCount() const = 0;
			virtual inline const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& GetMaterial(UINT idx) const = 0;
			virtual inline DirectX::BoundingOrientedBox GetModelOBB() const = 0;

			virtual inline ULONG GetVertexCount(UINT idx) const = 0;
		};
	}
}