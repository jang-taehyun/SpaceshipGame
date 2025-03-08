#pragma once

#include "TextureClass.h"

class ModelLoaderClass;

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const tstring& TextureFileName, const tstring& ModelFileName);
	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<tstring>& TextureFileNames, const tstring& ModelFileName);
	void Shutdown();
	void Render(ID3D11DeviceContext* const& DeviceContext);

	inline const int& GetIndexCount() const { return m_IndexCount; }
	inline const ID3D11ShaderResourceView* const& GetTexture(const int idx = 0) { return m_Texture->GetTexture(idx); }
	inline const std::vector< ID3D11ShaderResourceView*>& GetTextureArray() { return m_Texture->GetTextures(); }

private:
	HRESULT LoadModel(const tstring& FileName);
	HRESULT InitializeBuffers(ID3D11Device* const& Device);

	void RenderBuffers(ID3D11DeviceContext*);

	HRESULT LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const tstring& FileName);
	HRESULT LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<tstring>& FileNames);

	void ShutdownBuffers();
	void ReleaseTexture();
	void ReleaseModel();

private:
	ID3D11Buffer* m_VertexBufer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	
	int m_VertexCount = 0;
	int m_IndexCount = 0;

	VertexType* m_Vertices = nullptr;
	unsigned long* m_Indices = nullptr;

	ModelLoaderClass* m_Loader = nullptr;
	
	TextureClass* m_Texture = nullptr;
};