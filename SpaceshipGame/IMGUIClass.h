#pragma once

class IMGUIClass
{
public:
	IMGUIClass();
	IMGUIClass(const IMGUIClass& other);
	~IMGUIClass();

	HRESULT Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext);
	void Shutdown();
	HRESULT Render();
	HRESULT Render(const HWND& hwnd);
};

