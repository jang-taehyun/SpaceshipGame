#pragma once

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& other);
	~FrustumClass();

	// viewing frustum ���� �Լ�
	void ConstructFrustum(const float& ScreenDepth, const DirectX::XMMATRIX& ProjectionMatrix, const DirectX::XMMATRIX& ViewMatrix);

	// frustum volume�� �����ϴ��� Ȯ���ϴ� �Լ��� //

	bool CheckPoint(const float& x, const float& y, const float& z);
	bool CheckCube(const float& xCenter, const float& yCenter, const float& zCenter, const float& radius);
	bool CheckSphere(const float& xCenter, const float& yCenter, const float& zCenter, const float& radius);
	bool CheckRectangle(const float& xCenter, const float& yCenter, const float& zCenter, const float& xSize, const float& ySize, const float& zSize);

private:
	DirectX::XMVECTOR m_planes[6] = { 0, };
};

