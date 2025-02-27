#pragma once

class FontClass;
class FontShaderClass;

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		int VertexCount;
		int IndexCount;

		int MaxLength;

		float r;
		float g;
		float b;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, DirectX::XMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFPS(int, ID3D11DeviceContext*);
	bool SetCPU(int, ID3D11DeviceContext*);
	bool SetRenderCount(int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, DirectX::XMMATRIX, DirectX::XMMATRIX);

private:
	FontClass* m_Font = nullptr;
	FontShaderClass* m_FontShader = nullptr;
	int m_ScreenWidth = 0;
	int m_ScreenHeight = 0;
	
	SentenceType* m_Sentence1 = nullptr;
	SentenceType* m_Sentence2 = nullptr;
	SentenceType* m_Sentence3 = nullptr;
	SentenceType* m_Sentence4 = nullptr;
	SentenceType* m_Sentence5 = nullptr;

	DirectX::XMMATRIX m_BaseViewMatrix;
};