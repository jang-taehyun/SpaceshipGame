#include "pch.h"
#include "TypeConverterClass.h"
#include "ObjectClass.h"

const DirectX::XMFLOAT4X4 Object::ObjectClass::GetAffineMatrix() const
{
	DirectX::XMFLOAT4X4 ret;

	// affine matrix = scale * rotate * pos
	ret = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT4X4(m_Affine.position, m_Affine.rotation, m_Affine.scale);
	return ret;
}

void Object::ObjectClass::Update(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated)
{
}
