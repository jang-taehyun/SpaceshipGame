#include "pch.h"
#include "SoundClass.h"

Sound::SoundClass::SoundClass(DirectX::AudioEngine* engine, ID SoundID) : m_ID(SoundID), m_IsLoop(false)
{
	LoadWaveFile(engine);
}

Sound::SoundClass::SoundClass(SoundClass&& other) noexcept :
	m_ID(other.m_ID), m_IsLoop(other.m_IsLoop),
	m_Effect(std::move(other.m_Effect)), m_EffectInstance(std::move(other.m_EffectInstance))
{}

Sound::SoundClass::~SoundClass()
{
	m_EffectInstance->Stop();
}

Sound::SoundClass& Sound::SoundClass::operator=(SoundClass&& other) noexcept
{
	if (this == &other)
		return *this;

	// 멤버 변수 모두 해제 //
	m_EffectInstance->Stop();
	m_EffectInstance.reset();
	m_Effect.reset();

	// 멤버 변수 모두 이동 //
	m_ID = other.m_ID;
	m_IsLoop = other.m_IsLoop;
	m_Effect = std::move(other.m_Effect);
	m_EffectInstance = std::move(other.m_EffectInstance);

	return *this;
}

void Sound::SoundClass::LoadWaveFile(DirectX::AudioEngine* engine)
{
	// wave 파일을 load하면서 sound effect 객체 생성 //
	m_Effect = std::make_unique<DirectX::SoundEffect>(engine, Sound::SoundFileList.at(m_ID).c_str());
	assert(m_Effect);
	m_EffectInstance = std::move(m_Effect->CreateInstance());
	assert(m_EffectInstance);
}

void Sound::SoundClass::Play() const
{
	if (m_EffectInstance->GetState() == DirectX::SoundState::PLAYING)
		return;

	m_EffectInstance->Play(m_IsLoop);
}

void Sound::SoundClass::Stop() const
{
	if (m_EffectInstance->GetState() == DirectX::SoundState::STOPPED)
		return;

	m_EffectInstance->Stop();
}
