#pragma once

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& other);
	~FrustumClass();

	// viewing frustum ���� �Լ�
	void ConstructFrustum(const float ScreenDepth, const DirectX::XMMATRIX& const ProjectionMatrix, const DirectX::XMMATRIX& const ViewMatrix);

	// frustum volume�� �����ϴ��� Ȯ���ϴ� �Լ��� //

	bool CheckPoint(const float& const x, const float& const y, const float& const z);
	bool CheckCube(const float& const xCenter, const float& const yCenter, const float& const zCenter, const float& const radius);
	bool CheckSphere(const float& const xCenter, const float& const yCenter, const float& const zCenter, const float& const radius);
	bool CheckRectangle(const float& const xCenter, const float& const yCenter, const float& const zCenter, const float& const xSize, const float& const ySize, const float& const zSize);

private:
	DirectX::XMVECTOR m_planes[6] = { 0, };
};

