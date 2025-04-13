#include "pch.h"
#include <fstream>
#include "SoundClass.h"

bool SoundClass::IsInitailize = false;
static ErrorContent e;

SoundClass::SoundClass()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass Constructor");

	if (IsInitailize)
	{
		e.contents = _T("이미 SoundClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initialize();
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}

	IsInitailize = true;
}

SoundClass::~SoundClass()
{
	Shutdown();
	IsInitailize = false;
}

HRESULT SoundClass::Frame()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass Frame()");

	if (!m_AudioEngine->Update())
	{
		// 재생 중인 audio가 없는 경우
		if (m_AudioEngine->IsCriticalError())
		{

		}
	}

	return result;
}

HRESULT SoundClass::Initialize()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass Initialize()");
	
	// Audio Engine 초기화 //
	result = InitializeAudioEngine();
	if (FAILED(result))
		return result;

	// wav 오디오 파일을 sound effect 인스턴스에 load //
	result = LoadWaveFile();
	if (FAILED(result))
		return result;

	return result;
}

void SoundClass::Shutdown()
{
	// m_EffectSound->Stop();
	// m_BackgroundSound->Stop();
	// 
	// m_EffectSound->~SoundEffectInstance();
	// m_BackgroundSound->~SoundEffectInstance();
	// 
	// m_effect->~SoundEffect();
	// m_background->~SoundEffect();
	// 
	// m_AudioEngine->~AudioEngine();
}

HRESULT SoundClass::InitializeAudioEngine()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass InitializeAudioEngine()");

	// Audio Engine 생성 //
	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(result))
	{
		e.contents = _T("Audio Engine 초기화 실패");
		return result;
	}

	DirectX::AUDIO_ENGINE_FLAGS AudioEngineFlag = DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Default;

#ifdef _DEBUG
	AudioEngineFlag |= DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Debug;
#endif

	m_AudioEngine = std::make_unique<DirectX::AudioEngine>(AudioEngineFlag);

	return result;
}

HRESULT SoundClass::LoadWaveFile()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass LoadWaveFile()");

	// wave 파일을 load //
	m_effect = std::make_unique<DirectX::SoundEffect>(m_AudioEngine.get(), EffectSoundInfo.filename.c_str());
	m_background = std::make_unique<DirectX::SoundEffect>(m_AudioEngine.get(), BackgroundSoundInfo.filename.c_str());

	// sound effect instance 생성 //
	m_EffectSound = m_effect->CreateInstance();
	m_BackgroundSound = m_background->CreateInstance();

	return result;
}

HRESULT SoundClass::PlayWaveFile(const SoundInfo info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass LoadWaveFile()");

	switch (info)
	{
	case SoundInfo::BACKGROUND:
		m_BackgroundSound->Play();
		break;
	case SoundInfo::EFFECT:
		m_EffectSound->Play();
		break;
	default:
		break;
	}

	return result;
}

HRESULT SoundClass::StopWaveFile(const SoundInfo info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass LoadWaveFile()");

	// 현재 재생중인 사운드 파일이 없는 경우 //
	if (!m_AudioEngine->Update())
		return result;

	switch (info)
	{
	case SoundInfo::BACKGROUND:
		if(m_BackgroundSound->GetState() == DirectX::SoundState::PLAYING)
			m_BackgroundSound->Stop();
		break;
	case SoundInfo::EFFECT:
		if (m_EffectSound->GetState() == DirectX::SoundState::PLAYING)
			m_EffectSound->Stop();
		break;
	default:
		break;
	}

	return result;
}
