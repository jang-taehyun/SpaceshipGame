#include "stdafx.h"
#include "FontClass.h"
#include "FontShaderClass.h"
#include "TextClass.h"

TextClass::TextClass() {}
TextClass::TextClass(const TextClass& other) {}
TextClass::~TextClass() {}

bool TextClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, HWND hwnd, int ScreenWidth, int ScreenHeight, DirectX::XMMATRIX BaseViewMatrix)
{
	// ȭ�� �ػ�, view matrix �ʱ�ȭ //
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;
	m_BaseViewMatrix = BaseViewMatrix;


	// font object ���� �� �ʱ�ȭ //
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}
	if (!m_Font->Initialize(Device, DeviceContext, "../Engine/data/font01.txt", "../Engine/data/font01.tga"))
	{
		MessageBox(hwnd, _T("Could not initialize the font object"), _T("Error"), MB_OK);
		return false;
	}


	// font shader object ���� �� �ʱ�ȭ //
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}
	if (!m_FontShader->Initialize(Device, hwnd))
	{
		MessageBox(hwnd, _T("Could not initialize the font shader object"), _T("Error"), MB_OK);
		return false;
	}


	// ����� ���� ���� �� �ʱ�ȭ //
	if (!InitializeSentence(&m_Sentence1, 16, Device))
	{
		return false;
	}

	if (!InitializeSentence(&m_Sentence2, 16, Device))
	{
		return false;
	}

	if (!InitializeSentence(&m_Sentence3, 16, Device))
	{
		return false;
	}

	if (!InitializeSentence(&m_Sentence4, 16, Device))
	{
		return false;
	}

	if (!InitializeSentence(&m_Sentence5, 16, Device))
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// ���� ������ release //
	ReleaseSentence(&m_Sentence5);
	ReleaseSentence(&m_Sentence4);
	ReleaseSentence(&m_Sentence3);
	ReleaseSentence(&m_Sentence2);
	ReleaseSentence(&m_Sentence1);

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

bool TextClass::Render(ID3D11DeviceContext* DeviceContext, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX OrthoMatrix)
{
	if (!RenderSentence(DeviceContext, m_Sentence1, WorldMatrix, OrthoMatrix))
	{
		return false;
	}

	if (!RenderSentence(DeviceContext, m_Sentence2, WorldMatrix, OrthoMatrix))
	{
		return false;
	}

	if (!RenderSentence(DeviceContext, m_Sentence3, WorldMatrix, OrthoMatrix))
	{
		return false;
	}

	if (!RenderSentence(DeviceContext, m_Sentence4, WorldMatrix, OrthoMatrix))
	{
		return false;
	}

	if (!RenderSentence(DeviceContext, m_Sentence5, WorldMatrix, OrthoMatrix))
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int MouseX, int MouseY, ID3D11DeviceContext* DeviceContext)
{
	char tmpString[16] = { 0, };
	char MouseString[16] = { 0, };

	// ���콺 X ��ǥ�� delta ��(�̵��� �Ÿ�) ��� //
	_itoa_s(MouseX, tmpString, 10);

	strcpy_s(MouseString, "Mouse X: ");
	strcat_s(MouseString, tmpString);

	if (!UpdateSentence(m_Sentence1, MouseString, 20, 20, 1.f, 1.f, 1.f, DeviceContext))
	{
		return false;
	}

	// ���콺 Y ��ǥ�� delta ��(�̵��� �Ÿ�) ��� //
	_itoa_s(MouseY, tmpString, 10);

	strcpy_s(MouseString, "Mouse Y: ");
	strcat_s(MouseString, tmpString);

	if (!UpdateSentence(m_Sentence2, MouseString, 20, 40, 1.f, 1.f, 1.f, DeviceContext))
	{
		return false;
	}

	return true;
}

bool TextClass::SetFPS(int FPS, ID3D11DeviceContext* DeviceContext)
{
	char tempString[16] = { 0, };
	char FPSString[16] = { 0, };
	float r = 0.f, g = 0.f, b = 0.f;

	// FPS�� �ִ�ġ ����
	if (FPS > 9999)
	{
		FPS = 9999;
	}

	// integer�� string���� ��ȯ�ϰ� ���ڿ� �ϼ� //
	_itoa_s(FPS, tempString, 10);
	strcpy_s(FPSString, "FPS: ");
	strcat_s(FPSString, tempString);

	// FPS�� ���� ���� ���ڿ��� ���� ���� //
	if (FPS >= 60)
	{
		g = 1.f;
	}
	else if (FPS >= 30 && FPS < 60)
	{
		r = 1.f;
		g = 1.f;
	}
	else
	{
		r = 1.f;
	}

	// vertex buffer update //
	if (!UpdateSentence(m_Sentence3, FPSString, 20, 60, r, g, b, DeviceContext))
	{
		return false;
	}

	return true;
}

bool TextClass::SetCPU(int CPU, ID3D11DeviceContext* DeviceContext)
{
	char tempString[16] = { 0, };
	char CPUString[16] = { 0, };

	// CPU ��뷮�� ���ڿ��� ��ȯ�ϰ�, ���ڿ� �ϼ� //
	_itoa_s(CPU, tempString, 10);
	strcpy_s(CPUString, "CPU: ");
	strcat_s(CPUString, tempString);

	// vertex buffer update //
	if (!UpdateSentence(m_Sentence4, CPUString, 20, 80, 1.f, 1.f, 1.f, DeviceContext))
	{
		return false;
	}

	return true;
}

bool TextClass::SetRenderCount(int renderCount, ID3D11DeviceContext* DeviceContext)
{
	char tempString[16] = { 0, };
	char RenderCountString[16] = { 0, };

	// integer�� string���� ��ȯ�ϰ� ���ڿ� �ϼ� //
	_itoa_s(renderCount, tempString, 10);
	strcpy_s(RenderCountString, "RC: ");
	strcat_s(RenderCountString, tempString);

	// vertex buffer update //
	if (!UpdateSentence(m_Sentence5, RenderCountString, 20, 100, 1.f, 1.f, 1.f, DeviceContext))
	{
		return false;
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** Sentence, int MaxLength, ID3D11Device* Device)
{
	// sentence object ���� �� �ʱ�ȭ //
	*Sentence = new SentenceType;
	if (!(*Sentence))
	{
		return false;
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
		return false;
	}
	memset(vertices, 0, (sizeof(VertexType) * ((*Sentence)->VertexCount)));

	unsigned long* indices = new unsigned long[((*Sentence)->IndexCount)];
	if (!indices)
	{
		return false;
	}
	for (int i = 0; i < ((*Sentence)->IndexCount); i++)
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
		return false;
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
		return false;
	}


	// ���� ������, �ε��� ������ ���� //
	delete[] indices;
	indices = nullptr;

	delete[] vertices;
	vertices = nullptr;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* Sentence, const char* Text, int PositionX, int PositionY, float r, float g, float b, ID3D11DeviceContext* DeviceContext)
{
	// ���� �������� ���� ���� //
	Sentence->r = r;
	Sentence->g = g;
	Sentence->b = b;

	// ������ ���� ���� �� �˻� //
	int StringLength = (int)strlen(Text);
	if (StringLength > Sentence->MaxLength)
	{
		return false;
	}

	// ���� ���� ������ �Է� //
	VertexType* vertices = new VertexType[(Sentence->VertexCount)];
	if (!vertices)
	{
		return false;
	}
	memset(vertices, 0, (sizeof(VertexType) * (Sentence->VertexCount)));
	
	// �׷��� polygon�� ��ũ�� x, y ��ǥ ���
	float DrawX = (float)(((m_ScreenWidth / 2) * -1) + PositionX);
	float DrawY = (float)((m_ScreenHeight / 2) - PositionY);

	// ���� ������ update
	m_Font->BuildVertexArray((void*)vertices, Text, DrawX, DrawY);


	// vertex buffer�� ���� update //
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(Sentence->VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	VertexType* VerticesPtr = (VertexType*)MappedResource.pData;
	memcpy(VerticesPtr, (void*)vertices, (sizeof(VertexType) * (Sentence->VertexCount)));

	DeviceContext->Unmap(Sentence->VertexBuffer, 0);


	// ���� ������ ���� //
	delete[] vertices;
	vertices = nullptr;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** Sentence)
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

bool TextClass::RenderSentence(ID3D11DeviceContext* DeviceContext, SentenceType* Sentence, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX OrthoMatrix)
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
	DirectX::XMFLOAT4 PixelColor = DirectX::XMFLOAT4(Sentence->r, Sentence->g, Sentence->b, 1.f);
	if (!m_FontShader->Render(DeviceContext, Sentence->IndexCount, WorldMatrix, m_BaseViewMatrix, OrthoMatrix, m_Font->GetTexture(), PixelColor))
	{
		return false;
	}

	return true;
}
