#pragma once

#include "ColorClass.h"

class FontClass;
class FontShaderClass;
class Position2DClass;

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

		ColorClass Color;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
	};

public:
	TextClass();
	TextClass(const TextClass& other);
	~TextClass();

	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const HWND& hwnd, const int& ScreenWidth, const int& ScreenHeight, const DirectX::XMMATRIX& BaseViewMatrix);
	void Shutdown();
	HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& OrthoMatrix);

	HRESULT SetSentenceAboutInteger(const int& Number, const tstring& Title, const int& SentenceIdx, const Position2DClass& Position, const ColorClass& TextColor, ID3D11DeviceContext* const& DeviceContext);

private:
	HRESULT InitializeSentence(SentenceType** const& Sentence, const int& MaxLength, ID3D11Device* const& Device);
	HRESULT UpdateSentence(SentenceType* const& Sentence, const tstring& Text, const Position2DClass& Position, const ColorClass& TextColor, ID3D11DeviceContext* const& DeviceContext);
	void ReleaseSentence(SentenceType** const& Sentence);
	HRESULT RenderSentence(ID3D11DeviceContext* const& DeviceContext, const SentenceType* const& Sentence, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& OrthoMatrix);

private:
	FontClass* m_Font = nullptr;
	FontShaderClass* m_FontShader = nullptr;
	int m_ScreenWidth = 0;
	int m_ScreenHeight = 0;
	
	SentenceType* m_Sentence[5] = { 0, };

	DirectX::XMMATRIX m_BaseViewMatrix;
};