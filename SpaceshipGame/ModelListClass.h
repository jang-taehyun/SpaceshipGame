#pragma once

class ModelListClass
{
private:
	struct ModelInfoType
	{
		DirectX::XMFLOAT4 Color;
		float PositionX, PositionY, PositionZ;
	};

public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, DirectX::XMFLOAT4&);

private:
	int m_ModelCount = 0;
	ModelInfoType* m_ModelInfoList = nullptr;
};

