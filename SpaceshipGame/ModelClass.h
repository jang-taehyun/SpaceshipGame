#pragma once

class TextureClass;
class TextureArrayClass;

class ModelClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		// DirectX::XMFLOAT4 color;			// -> ColorShaderClass를 사용하는 경우
		DirectX::XMFLOAT2 texture;			// -> TextureShaderClass를 사용하는 경우
		DirectX::XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Initialize(ID3D11Device* const Device, ID3D11DeviceContext* const DeviceContext, const char* const TextureFileName, const char* const ModelFileName);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*, const char*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*, const TCHAR*, const char*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*, const TCHAR*, const TCHAR*, const char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextures();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*);
	bool LoadTextures(ID3D11Device*, const TCHAR*, const TCHAR*);
	bool LoadTextures(ID3D11Device*, const TCHAR*, const TCHAR*, const TCHAR*);
	void ReleaseTexture();
	void ReleaseTextures();

	bool LoadModel(const char*);
	void ReleaseModel();

private:
	ID3D11Buffer* m_VertexBufer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	int m_VertexCount = 0;
	int m_IndexCount = 0;
	
	TextureClass* m_Texture = nullptr;
	TextureArrayClass* m_TextureArray = nullptr;
	ModelType* m_Model = nullptr;
};