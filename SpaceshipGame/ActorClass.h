#pragma once

class TransformClass;
class ModelClass;

class ActorClass
{
public:
	ActorClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, const int ModelID = 0);
	virtual ~ActorClass();

	inline const DirectX::XMMATRIX& GetAffineMatrix() const;

private:
	HRESULT Initailize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, const int ModelID = 0);
	void Shutdown();

private:
	TransformClass* m_Transform = nullptr;
	int m_ModelID = 0;

public:
	ActorClass() = delete;
	ActorClass(const ActorClass& other) = delete;
};

