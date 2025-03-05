#pragma once

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& other);
	~FrustumClass();

	// viewing frustum 생성 함수
	void ConstructFrustum(const float& ScreenDepth, const DirectX::XMMATRIX& ProjectionMatrix, const DirectX::XMMATRIX& ViewMatrix);

	// frustum volume에 존재하는지 확인하는 함수들 //

	bool CheckPoint(const float& x, const float& y, const float& z);
	bool CheckCube(const float& xCenter, const float& yCenter, const float& zCenter, const float& radius);
	bool CheckSphere(const float& xCenter, const float& yCenter, const float& zCenter, const float& radius);
	bool CheckRectangle(const float& xCenter, const float& yCenter, const float& zCenter, const float& xSize, const float& ySize, const float& zSize);

private:
	DirectX::XMVECTOR m_planes[6] = { 0, };
};

