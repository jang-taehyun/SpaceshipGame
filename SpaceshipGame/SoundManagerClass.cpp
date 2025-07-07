#include "pch.h"
#include "SoundClass.h"
#include "SoundManagerClass.h"

bool Sound::SoundManagerClass::IsInitailize = false;

Sound::SoundManagerClass::SoundManagerClass() : m_CurrentSoundMask(0)
{
	assert(!IsInitailize);
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

#ifdef DEBUG
	AudioEngineFlag |= DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Debug;
#endif // DEBUG

	// Audio Engine 생성 //
	m_AudioEngine = std::make_unique<DirectX::AudioEngine>(AudioEngineFlag);
	assert(m_AudioEngine);
}

void Sound::SoundManagerClass::Load(UINT SoundMask)
{
	bool IsLoad = false, IsExist = false;
	ID id = ID::NONE;
	UINT flag = 0;
	std::unique_ptr<SoundClass> sound = nullptr;

	for(UINT i=0; i<SoundIDCount; ++i)
	{
		IsLoad = (SoundMask & (1 << i));
		IsExist = (m_CurrentSoundMask & (1 << i));
		id = static_cast<ID>(i);

		// 로드를 해야하는데 map에 없는 경우
		if (IsLoad && !IsExist)
		{
			// instance 생성
			sound = std::make_unique<SoundClass>(m_AudioEngine.get(), id);
			assert(sound);

			// 현재 로드된 Sound ID 업데이트
			m_CurrentSoundMask |= (1 << i);

			// map에 저장
			m_SoundList.insert(std::make_pair(id, std::move(sound)));
		}
		// 해제해야 하는데 map에 있는 경우
		else if (!IsLoad && IsExist)
		{
			// instance가 실제로 존재하는지 확인하고 해제
			if(m_SoundList.end() != m_SoundList.find(id))
				m_SoundList.erase(id);

			// 현재 로드된 Sound ID 제거
			flag = ~(1 << i);
			m_CurrentSoundMask &= flag;
		}
	}
}

void Sound::SoundManagerClass::Shutdown()
{
	m_SoundList.clear();
	m_AudioEngine.reset();

	// COM 객체 초기화 해제
	CoUninitialize();
}