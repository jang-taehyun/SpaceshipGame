#pragma once

#include <Audio.h>

class SoundClass
{
public:
	SoundClass();
	virtual ~SoundClass();

	HRESULT Frame();

	inline bool IsBackgoundPlay() const { return m_BackgroundSound->GetState() == DirectX::SoundState::PLAYING; }
	inline bool IsEffectPlay() const { return m_EffectSound->GetState() == DirectX::SoundState::PLAYING; }

	HRESULT PlayWaveFile(SoundInfo info) const;
	HRESULT StopWaveFile(SoundInfo info) const;

private:
	HRESULT Initialize();
	void Shutdown();

	HRESULT InitializeAudioEngine();
	HRESULT LoadWaveFile();

private:
	static bool IsInitailize;
	
	std::unique_ptr<DirectX::AudioEngine> m_AudioEngine;

	std::unique_ptr<DirectX::SoundEffect> m_Effect;
	std::unique_ptr<DirectX::SoundEffect> m_Background;

	std::unique_ptr<DirectX::SoundEffectInstance> m_EffectSound;
	std::unique_ptr<DirectX::SoundEffectInstance> m_BackgroundSound;

public:
	SoundClass(const SoundClass& other) = delete;
	SoundClass(SoundClass&& other) = delete;
	SoundClass& operator=(const SoundClass& other) = delete;
	SoundClass& operator=(SoundClass&& other) = delete;
};