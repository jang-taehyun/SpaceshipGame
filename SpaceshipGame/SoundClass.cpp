#include "pch.h"
#include "SoundClass.h"

bool Sound::SoundClass::IsInitailize = false;

Sound::SoundClass::SoundClass()
{
	HRESULT result = S_OK;

	assert(IsInitailize);

	result = Initialize();
	IsInitailize = true;
}

Sound::SoundClass::~SoundClass()
{
	Shutdown();
	IsInitailize = false;
}

HRESULT Sound::SoundClass::Frame()
{
	HRESULT result = S_OK;

	if (!m_AudioEngine->Update())
	{
		// 재생 중인 audio가 없는 경우
		if (m_AudioEngine->IsCriticalError())
		{

		}
	}

	return result;
}

HRESULT Sound::SoundClass::Initialize()
{
	HRESULT result = S_OK;
	
	// Audio Engine 초기화 //
	result = InitializeAudioEngine();

	// wav 오디오 파일을 sound effect 인스턴스에 load //
	result = LoadWaveFile();

	return result;
}

void Sound::SoundClass::Shutdown()
{
	if (m_EffectSound)
		m_EffectSound->Stop();

	if (m_BackgroundSound)
		m_BackgroundSound->Stop();

	m_EffectSound.reset();
	m_BackgroundSound.reset();
	m_Effect.reset();
	m_Background.reset();
	m_AudioEngine.reset();

	// COM 객체 초기화 해제
	CoUninitialize();
}

HRESULT Sound::SoundClass::InitializeAudioEngine()
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

	return result;
}

HRESULT Sound::SoundClass::LoadWaveFile()
{
	HRESULT result = S_OK;

	// wave 파일을 load하면서 sound effect 객체 생성 //
	m_Effect = std::make_unique<DirectX::SoundEffect>(m_AudioEngine.get(), Sound::SoundList.at(ID::EFFECT).c_str());
	assert(m_Effect);
	m_Background = std::make_unique<DirectX::SoundEffect>(m_AudioEngine.get(), Sound::SoundList.at(ID::BACKGROUND).c_str());
	assert(m_Background);

	// sound effect instance 생성 //
	m_EffectSound = m_Effect->CreateInstance();
	m_BackgroundSound = m_Background->CreateInstance();

	return result;
}

HRESULT Sound::SoundClass::PlayWaveFile(ID info) const
{
	HRESULT result = S_OK;

	switch (info)
	{
	case ID::BACKGROUND:
		m_BackgroundSound->Play();
		break;
	case ID::EFFECT:
		m_EffectSound->Play();
		break;
	default:
		break;
	}

	return result;
}

HRESULT Sound::SoundClass::StopWaveFile(ID info) const
{
	HRESULT result = S_OK;

	// 현재 재생중인 사운드 파일이 없는 경우 //
	if (!m_AudioEngine->Update())
		return result;

	switch (info)
	{
	case ID::BACKGROUND:
		if(m_BackgroundSound->GetState() == DirectX::SoundState::PLAYING)
			m_BackgroundSound->Stop();
		break;
	case ID::EFFECT:
		if (m_EffectSound->GetState() == DirectX::SoundState::PLAYING)
			m_EffectSound->Stop();
		break;
	default:
		break;
	}

	return result;
}
