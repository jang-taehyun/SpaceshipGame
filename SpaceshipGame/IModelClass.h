#pragma once

#include <DirectXCollision.h>

namespace Graphic
{
	namespace Texture { class TextureClass; }
}

namespace Graphic
{
	namespace Model
	{
		class IModelClass
		{
		public:
			virtual ~IModelClass() = default;

			virtual void AddWorldMatrix(const InstanceBufferType& world) = 0;

			virtual void UpdateInstanceBuffer(ID3D11DeviceContext* DeviceContext) = 0;
			virtual void RenderMesh(ID3D11DeviceContext* DeviceContext, UINT MeshIdx) = 0;

			virtual UINT GetMeshCount() const = 0;
			virtual Shader::ID GetShaderID() const = 0;
			virtual ULONG GetIndexCount(UINT idx) const = 0;
			virtual ULONG GetInstanceCount() const = 0;
			virtual const std::vector<std::unique_ptr<Texture::TextureClass>>& GetMaterial(UINT idx) const = 0;
			virtual DirectX::BoundingOrientedBox GetModelOBB() const = 0;

			virtual ULONG GetVertexCount(UINT idx) const = 0;
		};
	}
}