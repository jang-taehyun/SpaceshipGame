#include "pch.h"
#include <fstream>
#include "SoundClass.h"

SoundClass::SoundClass(const HWND& hwnd, const SoundFileInfo& info)
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass Constructor");

	if (IsInitailize)
	{
		e.contents = _T("이미 SoundClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;

		throw e;
	}

	result = Initialize(hwnd, info);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("SoundClass 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	IsInitailize = true;
}

SoundClass::~SoundClass()
{
	Shutdown();
	IsInitailize = false;
}

HRESULT SoundClass::Initialize(const HWND& hwnd, const SoundFileInfo& info)
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass Initialize()");

	// 파라미터 검사 //
	if (NULL == hwnd || info.filename == _T(""))
	{
		e.contents = _T("윈도우 핸들 or 지정된 file이 없습니다.");
		e.errorCode = E_FAIL;
		throw e;
	}
	
	// direct sound, primary sound buffer 초기화 //
	result = InitializeDirectSound(hwnd);
	if (FAILED(result))
	{
		e.contents = _T("direct sound, primary sound buffer 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	// wav 오디오 파일을 secondary buffer에 load //
	result = LoadWaveFile(info.filename, &m_SecondaryBuffer1);
	if (FAILED(result))
	{
		e.contents = _T("wav 오디오 파일을 secondary buffer에 load 실패");
		e.errorCode = result;
		throw e;
	}

	// sound 재생 //
	result = PlayWaveFile();
	if (FAILED(result))
	{
		e.contents = _T("sound 재생 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
}

void SoundClass::Shutdown()
{
	ShutdownWaveFile(&m_SecondaryBuffer1);
	ShutdownDirectSound();
}

HRESULT SoundClass::InitializeDirectSound(const HWND& hwnd)
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass InitializeDirectSound()");

	// 기본 sound device 초기화 //
	// 기본 sound device로 쓸 direct sound의 interface 초기화
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		e.contents = _T("기본 sound device로 쓸 direct sound의 interface 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	// 기본 sound device의 cooperative level 설정
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		e.contents = _T("기본 sound device의 cooperative level 설정 실패");
		e.errorCode = result;
		throw e;
	}


	// primary sound buffer 생성 및 초기화 //
	DSBUFFERDESC PrimarySoundBufferDesc;
	memset(&PrimarySoundBufferDesc, 0, sizeof(PrimarySoundBufferDesc));

	// primary sound buffer 설정
	PrimarySoundBufferDesc.dwSize = sizeof(PrimarySoundBufferDesc);
	PrimarySoundBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	PrimarySoundBufferDesc.dwBufferBytes = 0;
	PrimarySoundBufferDesc.dwReserved = 0;
	PrimarySoundBufferDesc.lpwfxFormat = NULL;
	PrimarySoundBufferDesc.guid3DAlgorithm = GUID_NULL;

	// primary sound buffer 생성
	result = m_DirectSound->CreateSoundBuffer(&PrimarySoundBufferDesc, &m_PrimaryBuffer, NULL);
	if (FAILED(result))
	{
		e.contents = _T("primary sound buffer 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// primary sound buffer의 format 설정
	WAVEFORMATEX PrimarySoundBufferFormatDesc;
	memset(&PrimarySoundBufferFormatDesc, 0, sizeof(PrimarySoundBufferFormatDesc));

	PrimarySoundBufferFormatDesc.wFormatTag = WAVE_FORMAT_PCM;
	PrimarySoundBufferFormatDesc.nSamplesPerSec = 44100;
	PrimarySoundBufferFormatDesc.wBitsPerSample = 16;
	PrimarySoundBufferFormatDesc.nChannels = 2;
	PrimarySoundBufferFormatDesc.nBlockAlign = (PrimarySoundBufferFormatDesc.wBitsPerSample / 8) * PrimarySoundBufferFormatDesc.nChannels;
	PrimarySoundBufferFormatDesc.nAvgBytesPerSec = PrimarySoundBufferFormatDesc.nSamplesPerSec * PrimarySoundBufferFormatDesc.nBlockAlign;
	PrimarySoundBufferFormatDesc.cbSize = 0;

	// primary sound buffer의 format을 primary sound buffer에 바인드
	result = m_PrimaryBuffer->SetFormat(&PrimarySoundBufferFormatDesc);
	if (FAILED(result))
	{
		e.contents = _T("primary sound buffer의 format을 primary sound buffer에 바인드 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
}

void SoundClass::ShutdownDirectSound()
{
	if (m_PrimaryBuffer)
	{
		m_PrimaryBuffer->Release();
		m_PrimaryBuffer = nullptr;
	}

	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = nullptr;
	}
}

HRESULT SoundClass::LoadWaveFile(const tstring& FileName, IDirectSoundBuffer8** const& SecondaryBuffer)
{
	ErrorContent e;
	HRESULT result = S_OK;
	std::ifstream FileIn;
	WaveHeaderType WaveFileHeader;
	WAVEFORMATEX WaveFormat;
	DSBUFFERDESC BufferDesc;
	IDirectSoundBuffer* tempBuffer = nullptr;
	unsigned char* WaveData = nullptr;
	unsigned char* BufferPtr = nullptr;
	unsigned long BufferSize = 0;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass LoadWaveFile()");

	// wav 파일 open //
	FileIn.open(FileName, std::ios::binary);
	if (FileIn.fail())
	{
		e.contents = _T("wav 파일 open 실패");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav 파일의 header를 read 및 check //
	memset(&WaveFileHeader, 0, sizeof(WaveFileHeader));

	// wav 파일의 header를 read
	FileIn.read((char*)&WaveFileHeader, sizeof(WaveFileHeader));
	size_t count = FileIn.gcount();
	if (sizeof(WaveFileHeader) != count)
	{
		e.contents = _T("wav 파일의 header를 read 실패");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav 파일의 header를 check
	// Chunk ID가 RIFF인지 확인
	if (('R' != WaveFileHeader.ChunkID[0]) || ('I' != WaveFileHeader.ChunkID[1]) || ('F' != WaveFileHeader.ChunkID[2]) || ('F' != WaveFileHeader.ChunkID[3]))
	{
		e.contents = _T("Chunk ID가 RIFF이 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// format이 WAVE인지 확인
	if (('W' != WaveFileHeader.Format[0]) || ('A' != WaveFileHeader.Format[1]) || ('V' != WaveFileHeader.Format[2]) || ('E' != WaveFileHeader.Format[3]))
	{
		e.contents = _T("format이 WAVE이 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// sub Chunk ID가 fmt인지 확인
	if (('f' != WaveFileHeader.SubChunkID[0]) || ('m' != WaveFileHeader.SubChunkID[1]) || ('t' != WaveFileHeader.SubChunkID[2]) || (' ' != WaveFileHeader.SubChunkID[3]))
	{
		e.contents = _T("sub Chunk ID가 fmt이 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// audio format이 WAVE_FORMAT_PCM인지 확인
	if (WAVE_FORMAT_PCM != WaveFileHeader.AudioFormat)
	{
		e.contents = _T("audio format이 WAVE_FORMAT_PCM가 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav 파일이 stereo format으로 저장되었는지 확인
	if (2 != WaveFileHeader.NumberChannels)
	{
		e.contents = _T("wav 파일이 stereo format이 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav 파일의 sample rate가 44.1 KHz인지 확인
	if (44100 != WaveFileHeader.SampleRate)
	{
		e.contents = _T("sample rate이 44.1 KHz가 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav파일이 16bit foramt으로 저장되었는지 확인
	if (16 != WaveFileHeader.BitsPerSample)
	{
		e.contents = _T("wav파일이 16bit foramt이 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav 파일의 data chunk header 확인
	if (('d' != WaveFileHeader.DataChunkID[0]) || ('a' != WaveFileHeader.DataChunkID[1]) || ('t' != WaveFileHeader.DataChunkID[2]) || ('a' != WaveFileHeader.DataChunkID[3]))
	{
		e.contents = _T("Data chuck ID가 data가 아닙니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav 파일의 내용을 저장할 secondary buffer 생성 //
	memset(&WaveFormat, 0, sizeof(WaveFormat));

	// secondary buffer에 들어갈 데이터의 format(데이터 형식) 지정
	WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	WaveFormat.nSamplesPerSec = 44100;
	WaveFormat.wBitsPerSample = 16;
	WaveFormat.nChannels = 2;
	WaveFormat.nBlockAlign = (WaveFormat.wBitsPerSample / 8) * WaveFormat.nChannels;
	WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
	WaveFormat.cbSize = 0;

	// secondary buffer 설정
	memset(&BufferDesc, 0, sizeof(BufferDesc));

	BufferDesc.dwSize = sizeof(BufferDesc);
	BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	BufferDesc.dwBufferBytes = WaveFileHeader.DataSize;
	BufferDesc.dwReserved = 0;
	BufferDesc.lpwfxFormat = &WaveFormat;
	BufferDesc.guid3DAlgorithm = GUID_NULL;

	// 임시 sound buffer 생성 후, secondary buffer 생성 //
	// 임시 sound buffer를 통해 IDirectSoundBuffer interface를 얻고,
	// IDirectSoundBuffer interface를 통해 IDirectSoundBuffer8 interface를 얻는다.
	// -> 이렇게 생성하는 이유 : Direct Sound의 설계 상의 이유로 IDirectSoundBuffer8 interface를 직접 생성할 수 없기 때문

	// 임시 sound buffer 생성
	result = m_DirectSound->CreateSoundBuffer(&BufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		e.contents = _T("임시 sound buffer 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// 임시 sound buffer를 통해 IDirectSoundBuffer8의 interface 얻기
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(*SecondaryBuffer));
	if (FAILED(result))
	{
		e.contents = _T("임시 sound buffer를 통해 IDirectSoundBuffer8의 interface 얻기 실패");
		e.errorCode = result;
		throw e;
	}

	// 임시 sound buffer 해제
	tempBuffer->Release();
	tempBuffer = nullptr;


	// secondary buffer에 wav 파일의 데이터 복사 //
	// wav 파일 데이터를 저장할 임시 메모리 생성
	WaveData = new unsigned char[WaveFileHeader.DataSize];
	if (!WaveData)
	{
		e.contents = _T("wav 파일 데이터를 저장할 임시 메모리 생성 실패");
		e.errorCode = E_FAIL;
		throw e;
	}

	// wav 파일 데이터를 읽어 임시 메모리에 저장
	FileIn.read((char*)WaveData, WaveFileHeader.DataSize);
	count = FileIn.gcount();
	if (WaveFileHeader.DataSize != count)
	{
		e.contents = _T("wav 파일 데이터를 읽어 임시 메모리에 저장 실패");
		e.errorCode = E_FAIL;
		throw e;
	}

	// 임시 메모리에 저장된 데이터를 secondary buffer에 복사하기 위해 secondary buffer를 lock
	result = (*SecondaryBuffer)->Lock(0, WaveFileHeader.DataSize, (void**)&BufferPtr, (DWORD*)&BufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		e.contents = _T("secondary buffer를 lock 실패");
		e.errorCode = result;
		throw e;
	}

	// 임시 메모리에 저장된 데이터를 secondary buffer에 복사
	memcpy(BufferPtr, WaveData, WaveFileHeader.DataSize);

	// secondary buffer를 unlock
	result = (*SecondaryBuffer)->Unlock((void*)BufferPtr, BufferSize, NULL, 0);
	if (FAILED(result))
	{
		e.contents = _T("secondary buffer를 unlock 실패");
		e.errorCode = result;
		throw e;
	}


	// wav 파일 닫기 //
	FileIn.close();

	// 임시 메모리 해제 //
	delete[] WaveData;
	WaveData = nullptr;

	return result;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** const& SecondaryBuffer)
{
	if (*SecondaryBuffer)
	{
		(*SecondaryBuffer)->Release();
		*SecondaryBuffer = nullptr;
	}
}

HRESULT SoundClass::PlayWaveFile()
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SoundClass PlayWaveFile()");

	// audio를 재생할 시작 위치 설정 //
	// 여기서는 0(secondary buffer의 시작 위치)로 설정
	// audio를 재생할 시작 위치를 설정하지 않으면 가장 최근에 재생되었던 부분부터 시작함
	result = m_SecondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
	{
		e.contents = _T("audio를 재생할 시작 위치 설정 실패");
		e.errorCode = result;
		throw e;
	}

	// audio의 volume 지정 //
	// 여기서는 100%로 지정
	result = m_SecondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		e.contents = _T("audio의 volume 지정 실패");
		e.errorCode = result;
		throw e;
	}

	// secondary buffer에 저장된 데이터 재생 //
	result = m_SecondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(result))
	{
		e.contents = _T("secondary buffer에 저장된 데이터 재생 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
}
