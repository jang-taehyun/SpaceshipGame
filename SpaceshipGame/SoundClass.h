#pragma once

#include <Audio.h>

class SoundClass
{
public:
	SoundClass();
	~SoundClass();

	HRESULT Frame();

	bool IsBackgoundPlay() { return m_BackgroundSound->GetState() == DirectX::SoundState::PLAYING; }
	bool IsEffectPlay() { return m_EffectSound->GetState() == DirectX::SoundState::PLAYING; }

	HRESULT PlayWaveFile(const SoundInfo info);
	HRESULT StopWaveFile(const SoundInfo info);

private:
	HRESULT Initialize();
	void Shutdown();

	HRESULT InitializeAudioEngine();
	HRESULT LoadWaveFile();

private:
	static bool IsInitailize;
	
	std::unique_ptr<DirectX::AudioEngine> m_AudioEngine;

	std::unique_ptr<DirectX::SoundEffect> m_effect;
	std::unique_ptr<DirectX::SoundEffect> m_background;

	std::unique_ptr<DirectX::SoundEffectInstance> m_EffectSound;
	std::unique_ptr<DirectX::SoundEffectInstance> m_BackgroundSound;

public:
	SoundClass(const SoundClass& other) = delete;
};