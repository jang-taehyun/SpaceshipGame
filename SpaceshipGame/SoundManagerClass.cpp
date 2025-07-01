#include "pch.h"
#include "SoundClass.h"
#include "SoundManagerClass.h"

bool Sound::SoundManagerClass::IsInitailize = false;

Sound::SoundManagerClass::SoundManagerClass()
{
	Initailize();
	IsInitailize = true;
}

Sound::SoundManagerClass::~SoundManagerClass()
{
	Shutdown();
	IsInitailize = false;
}

void Sound::SoundManagerClass::Frame() const
{
	m_AudioEngine->Update();
}

void Sound::SoundManagerClass::Play(ID SoundID) const
{
	std::map<ID, std::unique_ptr<SoundClass>>::const_iterator iter;

	iter = m_SoundList.find(SoundID);
	assert(m_SoundList.end() != iter);

	iter->second->Play();
}

void Sound::SoundManagerClass::Initailize()
{
	HRESULT result = S_OK;
	DirectX::AUDIO_ENGINE_FLAGS AudioEngineFlag = DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Default;

	// sound 관련 COM 객체 초기화 //
	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

#ifdef _DEBUG
	AudioEngineFlag |= DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Debug;
#endif

	// Audio Engine 생성 //
	m_AudioEngine = std::make_unique<DirectX::AudioEngine>(AudioEngineFlag);
	assert(m_AudioEngine);
}

void Sound::SoundManagerClass::Shutdown()
{
	m_SoundList.clear();
	m_AudioEngine.reset();

	// COM 객체 초기화 해제
	CoUninitialize();
}