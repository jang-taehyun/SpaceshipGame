#include "pch.h"
#include "SoundClass.h"

Sound::SoundClass::SoundClass(DirectX::AudioEngine* engine, ID SoundID, bool loop) : m_ID(SoundID), IsLoop(loop)
{
	LoadWaveFile(engine);
}

Sound::SoundClass::~SoundClass()
{
	m_EffectInstance->Stop();
}

void Sound::SoundClass::LoadWaveFile(DirectX::AudioEngine* engine)
{
	// wave 파일을 load하면서 sound effect 객체 생성 //
	m_Effect = std::make_unique<DirectX::SoundEffect>(engine, Sound::SoundList.at(m_ID).c_str());
	assert(m_Effect);
	m_EffectInstance = std::move(m_Effect->CreateInstance());
	assert(m_EffectInstance);
}

void Sound::SoundClass::Play() const
{
	if (m_EffectInstance->GetState() == DirectX::SoundState::PLAYING)
		return;

	m_EffectInstance->Play(IsLoop);
}
