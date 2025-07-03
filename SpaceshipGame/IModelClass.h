#pragma once

namespace Graphic
{
	namespace Model
	{
		class IModelClass
		{
		public:
			virtual ~IModelClass() = default;

			virtual inline void ResetWorldMatrix() = 0;
			virtual inline void AddWorldMatrix(DirectX::XMFLOAT4X4& world) = 0;

			virtual void UpdateInstanceData(ID3D11DeviceContext* DeviceContext) = 0;
			virtual void RenderMesh(ID3D11DeviceContext* DeviceContext, int MeshIdx) = 0;

			virtual inline Shader::ID GetShaderID() const = 0;
			virtual inline ULONG GetIndexCount(int idx) const = 0;
			virtual inline ULONG GetInstanceCount() const = 0;
			virtual inline const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& GetMaterial(int idx) const = 0;

			virtual inline ULONG GetVertexCount(int idx) const = 0;
		};
	}
}