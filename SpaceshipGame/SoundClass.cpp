#include "pch.h"
#include "SoundClass.h"

SoundClass::SoundClass() {}
SoundClass::SoundClass(const SoundClass& other) {}
SoundClass::~SoundClass() {}

HRESULT SoundClass::Initialize(const HWND& hwnd, const SoundFileInfo& info)
{
	// 파라미터 검사 //
	if (NULL == hwnd || info.filename == _T(""))
	{
		MessageBox(hwnd, _T("file이 없습니다."), _T("Sound error"), MB_OK);
		return E_FAIL;
	}
	
	// direct sound, primary sound buffer 초기화 //
	if (FAILED(InitializeDirectSound(hwnd)))
	{
		return E_FAIL;
	}

	// wav 오디오 파일을 secondary buffer에 load //
	if (FAILED(LoadWaveFile(info.filename, &m_SecondaryBuffer1)))
	{
		return E_FAIL;
	}

	return PlayWaveFile();
}

void SoundClass::Shutdown()
{
	ShutdownWaveFile(&m_SecondaryBuffer1);

	ShutdownDirectSound();
}

HRESULT SoundClass::InitializeDirectSound(const HWND& hwnd)
{
	// 기본 sound device 초기화 //
	// 기본 sound device로 쓸 direct sound의 interface 초기화
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
	{
		return E_FAIL;
	}

	// 기본 sound device의 cooperative level 설정
	if (FAILED(m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
	{
		return E_FAIL;
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
	if (FAILED(m_DirectSound->CreateSoundBuffer(&PrimarySoundBufferDesc, &m_PrimaryBuffer, NULL)))
	{
		return E_FAIL;
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
	if (FAILED(m_PrimaryBuffer->SetFormat(&PrimarySoundBufferFormatDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
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
	FILE* FilePtr = nullptr;

	// wav 파일 open //
	int error = fopen_s(&FilePtr, (char*)FileName.c_str(), "rb");
	if (error)
	{
		return E_FAIL;
	}

	// wav 파일의 header를 read 및 check //
	WaveHeaderType WaveFileHeader;
	memset(&WaveFileHeader, 0, sizeof(WaveFileHeader));

	// wav 파일의 header를 read
	size_t count = fread(&WaveFileHeader, sizeof(WaveFileHeader), 1, FilePtr);
	if (1 != count)
	{
		return E_FAIL;
	}

	// wav 파일의 header를 check
	// Chunk ID가 RIFF인지 확인
	if (('R' != WaveFileHeader.ChunkID[0]) || ('I' != WaveFileHeader.ChunkID[1]) || ('F' != WaveFileHeader.ChunkID[2]) || ('F' != WaveFileHeader.ChunkID[3]))
	{
		return E_FAIL;
	}

	// format이 WAVE인지 확인
	if (('W' != WaveFileHeader.Format[0]) || ('A' != WaveFileHeader.Format[1]) || ('V' != WaveFileHeader.Format[2]) || ('E' != WaveFileHeader.Format[3]))
	{
		return E_FAIL;
	}

	// sub Chunk ID가 fmt인지 확인
	if (('f' != WaveFileHeader.SubChunkID[0]) || ('m' != WaveFileHeader.SubChunkID[1]) || ('t' != WaveFileHeader.SubChunkID[2]) || (' ' != WaveFileHeader.SubChunkID[3]))
	{
		return E_FAIL;
	}

	// audio format이 WAVE_FORMAT_PCM인지 확인
	if (WAVE_FORMAT_PCM != WaveFileHeader.AudioFormat)
	{
		return E_FAIL;
	}

	// wav 파일이 stereo format으로 저장되었는지 확인
	if (2 != WaveFileHeader.NumberChannels)
	{
		return E_FAIL;
	}

	// wav 파일의 sample rate가 44.1 KHz인지 확인
	if (44100 != WaveFileHeader.SampleRate)
	{
		return E_FAIL;
	}

	// wav파일이 16bit foramt으로 저장되었는지 확인
	if (16 != WaveFileHeader.BitsPerSample)
	{
		return E_FAIL;
	}

	// wav 파일의 data chunk header 확인
	if (('d' != WaveFileHeader.DataChunkID[0]) || ('a' != WaveFileHeader.DataChunkID[1]) || ('t' != WaveFileHeader.DataChunkID[2]) || ('a' != WaveFileHeader.DataChunkID[3]))
	{
		return E_FAIL;
	}

	// wav 파일의 내용을 저장할 secondary buffer 생성 //
	WAVEFORMATEX WaveFormat;
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
	DSBUFFERDESC BufferDesc;
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
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&BufferDesc, &tempBuffer, NULL)))
	{
		return E_FAIL;
	}

	// 임시 sound buffer를 통해 IDirectSoundBuffer8의 interface 얻기
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(*SecondaryBuffer))))
	{
		return E_FAIL;
	}

	// 임시 sound buffer 해제
	tempBuffer->Release();
	tempBuffer = nullptr;


	// secondary buffer에 wav 파일의 데이터 복사 //
	// wav 파일 데이터를 읽을 시작점 찾기
	fseek(FilePtr, sizeof(WaveHeaderType), SEEK_SET);

	// wav 파일 데이터를 저장할 임시 메모리 생성
	unsigned char* WaveData = new unsigned char[WaveFileHeader.DataSize];
	if (!WaveData)
	{
		return E_FAIL;
	}

	// wav 파일 데이터를 읽어 임시 메모리에 저장
	count = fread(WaveData, 1, WaveFileHeader.DataSize, FilePtr);
	if (WaveFileHeader.DataSize != count)
	{
		return E_FAIL;
	}

	// 임시 메모리에 저장된 데이터를 secondary buffer에 복사하기 위해 secondary buffer를 lock
	unsigned char* BufferPtr = nullptr;
	unsigned long BufferSize = 0;
	if (FAILED((*SecondaryBuffer)->Lock(0, WaveFileHeader.DataSize, (void**)&BufferPtr, (DWORD*)&BufferSize, NULL, 0, 0)))
	{
		return E_FAIL;
	}

	// 임시 메모리에 저장된 데이터를 secondary buffer에 복사
	memcpy(BufferPtr, WaveData, WaveFileHeader.DataSize);

	// secondary buffer를 unlock
	if (FAILED((*SecondaryBuffer)->Unlock((void*)BufferPtr, BufferSize, NULL, 0)))
	{
		return E_FAIL;
	}


	// wav 파일 닫기 //
	error = fclose(FilePtr);
	if (error)
	{
		return E_FAIL;
	}

	// 임시 메모리 해제 //
	delete[] WaveData;
	WaveData = nullptr;

	return S_OK;
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
	// audio를 재생할 시작 위치 설정 //
	// 여기서는 0(secondary buffer의 시작 위치)로 설정
	// audio를 재생할 시작 위치를 설정하지 않으면 가장 최근에 재생되었던 부분부터 시작함
	if (FAILED(m_SecondaryBuffer1->SetCurrentPosition(0)))
	{
		return E_FAIL;
	}

	// audio의 volume 지정 //
	// 여기서는 100%로 지정
	if (FAILED(m_SecondaryBuffer1->SetVolume(DSBVOLUME_MAX)))
	{
		return E_FAIL;
	}

	// secondary buffer에 저장된 데이터 재생 //
	if (FAILED(m_SecondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING)))
	{
		return E_FAIL;
	}

	return S_OK;
}
