#pragma once

#include <Audio.h>

namespace Sound { class SoundClass; }

namespace Sound
{
	class SoundManagerClass
	{
	private:
		std::map<ID, const std::wstring> m_SoundFileList =
		{
			{ ID::BACKGROUND, _T("./resource/dedede.wav") },
			{ ID::EFFECT, _T("./resource/sound01.wav") },
		};

	public:
		SoundManagerClass();
		virtual ~SoundManagerClass();

		void Load(UINT SoundMask);
		void Play(ID SoundID) const;
		void Stop(ID SoundID) const;
		void SetLoop(ID SoundID, bool IsLoop);
		void Frame() const;

		UINT GetSoundIDCount() const { return m_SoundIDCount; }
		UINT GetSoundMask() const { return m_CurrentSoundMask; } 

		void AllStop() const;

	private:
		void Initailize();
		void Shutdown();

	private:
		static bool IsInitailize;
		const UINT m_SoundIDCount = 3;

		std::unique_ptr<DirectX::AudioEngine> m_AudioEngine = nullptr;
		std::map<ID, std::unique_ptr<SoundClass>> m_SoundList;
		UINT m_CurrentSoundMask = 0;
	};
}
