#include "pch.h"
#include "FontClass.h"
#include "FontShaderClass.h"
#include "Position2DClass.h"
#include "ColorClass.h"
#include "TextClass.h"

TextClass::TextClass() {}
TextClass::TextClass(const TextClass& other) {}
TextClass::~TextClass() {}

HRESULT TextClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const HWND& hwnd,  const int& ScreenWidth, const int& ScreenHeight, const DirectX::XMMATRIX& BaseViewMatrix)
{
	// ȭ�� �ػ�, view matrix �ʱ�ȭ //
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;
	m_BaseViewMatrix = BaseViewMatrix;


	// font object ���� �� �ʱ�ȭ //
	m_Font = new FontClass;
	if (!m_Font)
	{
		return E_FAIL;
	}
	if (!m_Font->Initialize(Device, DeviceContext, "../Engine/data/font01.txt", "../Engine/data/font01.tga"))
	{
		MessageBox(hwnd, _T("Could not initialize the font object"), _T("Error"), MB_OK);
		return E_FAIL;
	}


	// font shader object ���� �� �ʱ�ȭ //
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return E_FAIL;
	}
	if (!m_FontShader->Initialize(Device, hwnd))
	{
		MessageBox(hwnd, _T("Could not initialize the font shader object"), _T("Error"), MB_OK);
		return E_FAIL;
	}


	// ����� ���� ���� �� �ʱ�ȭ //
	for (int i = 0; i < 5; ++i)
	{
		if (FAILED(InitializeSentence(&m_Sentence[i], 16, Device)))
			return E_FAIL;
	}

	return S_OK;
}

void TextClass::Shutdown()
{
	// ���� ������ release //
	for(int i=0; i<5; ++i)
		ReleaseSentence(&m_Sentence[i]);

	// member variable release //
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}

	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}
}

HRESULT TextClass::Render(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& OrthoMatrix)
{
	for (int i = 0; i < 5; ++i)
	{
		if (FAILED(RenderSentence(DeviceContext, m_Sentence[i], WorldMatrix, OrthoMatrix)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT TextClass::SetSentenceAboutInteger(const int& Number, const tstring& Title, const int& SentenceIdx, const Position2DClass& Position, const ColorClass& TextColor, ID3D11DeviceContext* const& DeviceContext)
{
	tstring tmp = Title + _T(" ");
	tmp = tmp + std::to_wstring(Number);

	if (FAILED(UpdateSentence(m_Sentence[SentenceIdx], tmp, Position, TextColor, DeviceContext)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT TextClass::InitializeSentence(SentenceType** const& Sentence, const int& MaxLength, ID3D11Device* const& Device)
{
	// sentence object ���� �� �ʱ�ȭ //
	*Sentence = new SentenceType;
	if (!(*Sentence))
	{
		return E_FAIL;
	}

	(*Sentence)->VertexBuffer = nullptr;
	(*Sentence)->IndexBuffer = nullptr;
	(*Sentence)->MaxLength = MaxLength;
	(*Sentence)->VertexCount = MaxLength * 6;
	(*Sentence)->IndexCount = (*Sentence)->VertexCount;


	// ������ �̷�� ���� ������, �ε��� ������ ���� �� �ʱ�ȭ //
	VertexType* vertices = new VertexType[((*Sentence)->VertexCount)];
	if (!vertices)
	{
		return E_FAIL;
	}
	memset(vertices, 0, (sizeof(VertexType) * ((*Sentence)->VertexCount)));

	unsigned long* indices = new unsigned long[((*Sentence)->IndexCount)];
	if (!indices)
	{
		return E_FAIL;
	}
	for (int i = 0; i < ((*Sentence)->IndexCount); ++i)
	{
		indices[i] = i;
	}


	// dynamic vertex buffer ���� �� �ʱ�ȭ //
	D3D11_BUFFER_DESC VertexBufferDesc;
	memset(&VertexBufferDesc, 0, sizeof(VertexBufferDesc));

	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = (sizeof(VertexType) * ((*Sentence)->VertexCount));
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	if (FAILED(Device->CreateBuffer(&VertexBufferDesc, &VertexData, &((*Sentence)->VertexBuffer))))
	{
		return E_FAIL;
	}


	// index buffer ���� �� �ʱ�ȭ //
	D3D11_BUFFER_DESC IndexBufferDesc;
	memset(&IndexBufferDesc, 0, sizeof(IndexBufferDesc));

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = (sizeof(unsigned long) * ((*Sentence)->IndexCount));
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	if (FAILED(Device->CreateBuffer(&IndexBufferDesc, &IndexData, &((*Sentence)->IndexBuffer))))
	{
		return E_FAIL;
	}


	// ���� ������, �ε��� ������ ���� //
	delete[] indices;
	indices = nullptr;

	delete[] vertices;
	vertices = nullptr;

	return S_OK;
}

HRESULT TextClass::UpdateSentence(SentenceType* const& Sentence, const tstring& Text, const Position2DClass& Position, const ColorClass& TextColor, ID3D11DeviceContext* const& DeviceContext)
{
	// ���� �������� ���� ���� //
	Sentence->Color = TextColor;

	// ������ ���� ���� �� �˻� //
	int StringLength = (int)Text.length();
	if (StringLength > Sentence->MaxLength)
	{
		return E_FAIL;
	}

	// ���� ���� ������ �Է� //
	VertexType* vertices = new VertexType[(Sentence->VertexCount)];
	if (!vertices)
	{
		return E_FAIL;
	}
	memset(vertices, 0, (sizeof(VertexType) * (Sentence->VertexCount)));
	
	// �׷��� polygon�� ��ũ�� x, y ��ǥ ���
	float DrawX = ((float)((m_ScreenWidth / 2) * -1) + Position.GetPositionX());
	float DrawY = ((float)(m_ScreenHeight / 2) - Position.GetPositionY());

	// ���� ������ update
	m_Font->BuildVertexArray((void*)vertices, Text, DrawX, DrawY);


	// vertex buffer�� ���� update //
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(Sentence->VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return E_FAIL;
	}

	VertexType* VerticesPtr = (VertexType*)MappedResource.pData;
	memcpy(VerticesPtr, (void*)vertices, (sizeof(VertexType) * (Sentence->VertexCount)));

	DeviceContext->Unmap(Sentence->VertexBuffer, 0);


	// ���� ������ ���� //
	delete[] vertices;
	vertices = nullptr;

	return S_OK;
}

void TextClass::ReleaseSentence(SentenceType** const& Sentence)
{
	if (*Sentence)
	{
		if ((*Sentence)->VertexBuffer)
		{
			(*Sentence)->VertexBuffer->Release();
			(*Sentence)->VertexBuffer = nullptr;
		}

		if ((*Sentence)->IndexBuffer)
		{
			(*Sentence)->IndexBuffer->Release();
			(*Sentence)->IndexBuffer = nullptr;
		}

		delete* Sentence;
		*Sentence = nullptr;
	}
}

HRESULT TextClass::RenderSentence(ID3D11DeviceContext* const& DeviceContext, const SentenceType* const& Sentence, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& OrthoMatrix)
{
	// vertex buffer�� stride, offset ���� //
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// input assembler���� vertex buffer, index buffer Ȱ��ȭ //
	DeviceContext->IASetVertexBuffers(0, 1, &(Sentence->VertexBuffer), &stride, &offset);
	DeviceContext->IASetIndexBuffer((Sentence->IndexBuffer), DXGI_FORMAT_R32_UINT, 0);

	// �⺻ polygon ���� //
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ������ //
	DirectX::XMFLOAT4 PixelColor = DirectX::XMFLOAT4(Sentence->Color.GetColorRed(), Sentence->Color.GetColorGreen(), Sentence->Color.GetColorBlue(), Sentence->Color.GetColorAlpha());
	if (FAILED(m_FontShader->Render(DeviceContext, Sentence->IndexCount, WorldMatrix, m_BaseViewMatrix, OrthoMatrix, m_Font->GetTexture(), PixelColor)))
	{
		return E_FAIL;
	}

	return S_OK;
}
