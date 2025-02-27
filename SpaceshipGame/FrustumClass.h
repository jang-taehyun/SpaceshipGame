#pragma once

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	void ConstructFrustum(float, DirectX::XMMATRIX, DirectX::XMMATRIX);		// viewing frustum »ý¼º

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	DirectX::XMVECTOR m_planes[6] = { 0, };
};

