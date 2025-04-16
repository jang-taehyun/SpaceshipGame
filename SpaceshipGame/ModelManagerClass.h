#pragma once

#include <map>

class ModelClass;

class ModelManagerClass
{
public:
	ModelManagerClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext);
	virtual ~ModelManagerClass();

	ModelClass* const GetModel(ModelIDs id);

private:
	HRESULT Initailize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext);
	void Shutdown();

private:
	static bool IsInitialize;
	std::map<ModelIDs, ModelClass*> m_ModelList;

public:
	ModelManagerClass(const ModelManagerClass& other) = delete;
};

