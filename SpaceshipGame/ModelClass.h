#pragma once

#include "TextureClass.h"
#include "TransformClass.h"

class ModelLoaderClass;

class ModelClass
{
private:
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
	ModelClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::wstring& TextureFileName, const std::wstring& ModelFileName);
	ModelClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& TextureFileNames, const std::wstring& ModelFileName);
	~ModelClass();

	void Render(ID3D11DeviceContext* const& DeviceContext);

	inline const int& GetIndexCount() const { return m_IndexCount; }
	inline const ID3D11ShaderResourceView* const& GetTexture(const int idx = 0) { return m_Texture->GetTexture(idx); }
	inline const std::vector<ID3D11ShaderResourceView*>& GetTextureArray() { return m_Texture->GetTextures(); }

	inline const DirectX::XMMATRIX& GetAffineMatrix() const { return m_Transform->GetAffine(); }

private:
	HRESULT Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::wstring& TextureFileName, const std::wstring& ModelFileName);
	HRESULT Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& TextureFileNames, const std::wstring& ModelFileName);
	void Shutdown();

	HRESULT LoadModel(const std::wstring& FileName);
	HRESULT InitializeBuffers(ID3D11Device* const& Device);

	void RenderBuffers(ID3D11DeviceContext* const& DeviceContext);

	HRESULT LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::wstring& FileName);
	HRESULT LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& FileNames);

	void ShutdownBuffers();
	void ReleaseTexture();
	void ReleaseModel();
	void ShutdownPosition();

private:
	ID3D11Buffer* m_VertexBufer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	
	int m_VertexCount = 0;
	int m_IndexCount = 0;

	ModelType* m_Model = nullptr;
	TextureClass* m_Texture = nullptr;

	TransformClass* m_Transform = nullptr;

	bool m_Line = false;

public:
	ModelClass() = delete;
	ModelClass(const ModelClass& other) = delete;
};