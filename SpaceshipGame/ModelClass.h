#pragma once

#include "TextureClass.h"

class ModelLoaderClass;
class ShaderClass;
class LightClass;
class CameraClass;

class ModelClass
{
protected:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	explicit ModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info);
	virtual ~ModelClass();

	HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera);

	inline const int& GetIndexCount() const { return m_IndexCount; }
	inline const ID3D11ShaderResourceView* const& GetTexture(const int idx = 0) { return m_Texture->GetTexture(idx); }
	inline const std::vector<ID3D11ShaderResourceView*>& GetTextureArray() { return m_Texture->GetTextures(); }
	inline ShaderClass* const& GetShader() { return m_Shader; }

private:
	HRESULT Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info);
	HRESULT LoadModel(const std::wstring& FileName);
	HRESULT LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& FileNames);
	
	virtual HRESULT InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) = 0;
	virtual HRESULT RenderShader(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera) = 0;

	void Shutdown();
	void ShutdownBuffers();
	void ReleaseTexture();
	virtual void ReleaseShader() = 0;
	void ReleaseModel();

protected:
	HRESULT InitializeBuffers(ID3D11Device* const& Device);

protected:
	virtual void SetBuffers(ID3D11DeviceContext* const& DeviceContext);
	inline ID3D11Buffer* const& GetVertexBuffer() const { return m_VertexBuffer; }
	inline ID3D11Buffer* const& GetIndexBuffer() const { return m_IndexBuffer; }

private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	
	int m_VertexCount = 0;
	int m_IndexCount = 0;

	ModelType* m_Model = nullptr;
	TextureClass* m_Texture = nullptr;

protected:
	ShaderClass* m_Shader = nullptr;

public:
	ModelClass() = delete;
	ModelClass(const ModelClass& other) = delete;
};