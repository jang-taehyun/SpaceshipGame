#include "pch.h"
#include "FrustumClass.h"

FrustumClass::FrustumClass() {}
FrustumClass::FrustumClass(const FrustumClass& other) {}
FrustumClass::~FrustumClass() {}

void FrustumClass::ConstructFrustum(const float& ScreenDepth, const DirectX::XMMATRIX& ProjectionMatrix, const DirectX::XMMATRIX& ViewMatrix)
{
	// projection matrix�� XMFLOAT4X4 �ڷ������� ��ȯ //
	DirectX::XMFLOAT4X4 projectionMatrix;
    DirectX::XMMATRIX FinalProjectionMatrix;
	DirectX::XMStoreFloat4x4(&projectionMatrix, ProjectionMatrix);

	// viewing frustum���� near plane�� �ּ� z�� ��� //
	float zMinimum = -projectionMatrix._43 / projectionMatrix._33;

	// viewing frustum�� far plane���� Ȯ���ϱ� ���� scaling �� ��� //
	float r = ScreenDepth / (ScreenDepth - zMinimum);

	// near plane�� �ּ� z���� far plane���� Ȯ���ϱ� ���� scaling ���� �̿��� projection matrix�� �ٽ� ���� //
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;
    FinalProjectionMatrix = DirectX::XMLoadFloat4x4(&projectionMatrix);

	// view matrix, projection matrix�� �̿��� viewing frustum matrix �����ϰ� XMFLOAT4X4 �ڷ������� ��ȯ //
	DirectX::XMMATRIX FrustumCullingMatrix = DirectX::XMMatrixMultiply(ViewMatrix, FinalProjectionMatrix);

	DirectX::XMFLOAT4X4 FrustumMatrix;
	DirectX::XMStoreFloat4x4(&FrustumMatrix, FrustumCullingMatrix);

    // viewing frustum matrix�� plane(���) ��� //
    // near plane
    float x = (float)(FrustumMatrix._14 + FrustumMatrix._13);
    float y = (float)(FrustumMatrix._24 + FrustumMatrix._23);
    float z = (float)(FrustumMatrix._34 + FrustumMatrix._33);
    float w = (float)(FrustumMatrix._44 + FrustumMatrix._43);
    m_planes[0] = DirectX::XMVectorSet(x, y, z, w);
    m_planes[0] = DirectX::XMPlaneNormalize(m_planes[0]);

    // far plane
    x = (float)(FrustumMatrix._14 - FrustumMatrix._13);
    y = (float)(FrustumMatrix._24 - FrustumMatrix._23);
    z = (float)(FrustumMatrix._34 - FrustumMatrix._33);
    w = (float)(FrustumMatrix._44 - FrustumMatrix._43);
    m_planes[1] = DirectX::XMVectorSet(x, y, z, w);
    m_planes[1] = DirectX::XMPlaneNormalize(m_planes[1]);

    // left plane
    x = (float)(FrustumMatrix._14 + FrustumMatrix._11);
    y = (float)(FrustumMatrix._24 + FrustumMatrix._21);
    z = (float)(FrustumMatrix._34 + FrustumMatrix._31);
    w = (float)(FrustumMatrix._44 + FrustumMatrix._41);
    m_planes[2] = DirectX::XMVectorSet(x, y, z, w);
    m_planes[2] = DirectX::XMPlaneNormalize(m_planes[2]);

    // right plane
    x = (float)(FrustumMatrix._14 - FrustumMatrix._11);
    y = (float)(FrustumMatrix._24 - FrustumMatrix._21);
    z = (float)(FrustumMatrix._34 - FrustumMatrix._31);
    w = (float)(FrustumMatrix._44 - FrustumMatrix._41);
    m_planes[3] = DirectX::XMVectorSet(x, y, z, w);
    m_planes[3] = DirectX::XMPlaneNormalize(m_planes[3]);

    // up plane
    x = (float)(FrustumMatrix._14 - FrustumMatrix._12);
    y = (float)(FrustumMatrix._24 - FrustumMatrix._22);
    z = (float)(FrustumMatrix._34 - FrustumMatrix._32);
    w = (float)(FrustumMatrix._44 - FrustumMatrix._42);
    m_planes[4] = DirectX::XMVectorSet(x, y, z, w);
    m_planes[4] = DirectX::XMPlaneNormalize(m_planes[4]);

    // bottom plane
    x = (float)(FrustumMatrix._14 + FrustumMatrix._12);
    y = (float)(FrustumMatrix._24 + FrustumMatrix._22);
    z = (float)(FrustumMatrix._34 + FrustumMatrix._32);
    w = (float)(FrustumMatrix._44 + FrustumMatrix._42);
    m_planes[5] = DirectX::XMVectorSet(x, y, z, w);
    m_planes[5] = DirectX::XMPlaneNormalize(m_planes[5]);
}

bool FrustumClass::CheckPoint(const float& x, const float& y,  const float& z)
{
    // vertex�� viewing frustum ���ο� �����ϴ��� Ȯ��
    for (int i = 0; i < 6; i++)
    {
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet(x, y, z, 1.f))) < 0.f)
            return false;
    }

    return true;
}

bool FrustumClass::CheckCube(const float& xCenter, const float& yCenter, const float& zCenter, const float& radius)
{
    // ������ü�� ������ �� �ϳ��� viewing frustum ���ο� �����ϴ��� Ȯ��
    for (int i = 0; i < 6; i++)
    {
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
            continue;

        return false;
    }

    return true;
}

bool FrustumClass::CheckSphere(const float& xCenter, const float& yCenter, const float& zCenter, const float& radius)
{
    // ���� �߽��� viewing frustum ���ο� �ִ��� Ȯ��
    for (int i = 0; i < 6; i++)
    {
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet(xCenter, yCenter, zCenter, 1.0f))) < (radius * -1.f))
            return false;
    }

    return true;
}

bool FrustumClass::CheckRectangle(const float& xCenter, const float& yCenter, const float& zCenter, const float& xSize, const float& ySize, const float& zSize)
{
    // ����ü�� ������ �� �ϳ��� viewing frustum ���ο� �����ϴ��� Ȯ��
    for (int i = 0; i < 6; i++)
    {
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
            continue;
        if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
            continue;

        return false;
    }

    return true;
}
