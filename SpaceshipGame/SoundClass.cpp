#include "pch.h"
#include "SoundClass.h"

SoundClass::SoundClass() {}
SoundClass::SoundClass(const SoundClass& other) {}
SoundClass::~SoundClass() {}

bool SoundClass::Initialize(HWND hwnd)
{
	// direct sound, primary sound buffer �ʱ�ȭ //
	if (!InitializeDirectSound(hwnd))
	{
		return false;
	}

	// wav ����� ������ secondary buffer�� load //
	if (!LoadWaveFile("../Engine/data/sound01.wav", &m_SecondaryBuffer1))
	{
		return false;
	}

	return PlayWaveFile();
}

void SoundClass::Shutdown()
{
	ShutdownWaveFile(&m_SecondaryBuffer1);

	ShutdownDirectSound();
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	// �⺻ sound device �ʱ�ȭ //
	// �⺻ sound device�� �� direct sound�� interface �ʱ�ȭ
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
	{
		return false;
	}

	// �⺻ sound device�� cooperative level ����
	if (FAILED(m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
	{
		return false;
	}


	// primary sound buffer ���� �� �ʱ�ȭ //
	DSBUFFERDESC PrimarySoundBufferDesc;
	memset(&PrimarySoundBufferDesc, 0, sizeof(PrimarySoundBufferDesc));

	// primary sound buffer ����
	PrimarySoundBufferDesc.dwSize = sizeof(PrimarySoundBufferDesc);
	PrimarySoundBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	PrimarySoundBufferDesc.dwBufferBytes = 0;
	PrimarySoundBufferDesc.dwReserved = 0;
	PrimarySoundBufferDesc.lpwfxFormat = NULL;
	PrimarySoundBufferDesc.guid3DAlgorithm = GUID_NULL;

	// primary sound buffer ����
	if (FAILED(m_DirectSound->CreateSoundBuffer(&PrimarySoundBufferDesc, &m_PrimaryBuffer, NULL)))
	{
		return false;
	}

	// primary sound buffer�� format ����
	WAVEFORMATEX PrimarySoundBufferFormatDesc;
	memset(&PrimarySoundBufferFormatDesc, 0, sizeof(PrimarySoundBufferFormatDesc));

	PrimarySoundBufferFormatDesc.wFormatTag = WAVE_FORMAT_PCM;
	PrimarySoundBufferFormatDesc.nSamplesPerSec = 44100;
	PrimarySoundBufferFormatDesc.wBitsPerSample = 16;
	PrimarySoundBufferFormatDesc.nChannels = 2;
	PrimarySoundBufferFormatDesc.nBlockAlign = (PrimarySoundBufferFormatDesc.wBitsPerSample / 8) * PrimarySoundBufferFormatDesc.nChannels;
	PrimarySoundBufferFormatDesc.nAvgBytesPerSec = PrimarySoundBufferFormatDesc.nSamplesPerSec * PrimarySoundBufferFormatDesc.nBlockAlign;
	PrimarySoundBufferFormatDesc.cbSize = 0;

	// primary sound buffer�� format�� primary sound buffer�� ���ε�
	if (FAILED(m_PrimaryBuffer->SetFormat(&PrimarySoundBufferFormatDesc)))
	{
		return false;
	}

	return true;
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

bool SoundClass::LoadWaveFile(const char* FileName, IDirectSoundBuffer8** SecondaryBuffer)
{
	FILE* FilePtr = nullptr;

	// wav ���� open //
	int error = fopen_s(&FilePtr, FileName, "rb");
	if (error)
	{
		return false;
	}

	// wav ������ header�� read �� check //
	WaveHeaderType WaveFileHeader;
	memset(&WaveFileHeader, 0, sizeof(WaveFileHeader));

	// wav ������ header�� read
	unsigned int count = fread(&WaveFileHeader, sizeof(WaveFileHeader), 1, FilePtr);
	if (1 != count)
	{
		return false;
	}

	// wav ������ header�� check
	// Chunk ID�� RIFF���� Ȯ��
	if (('R' != WaveFileHeader.ChunkID[0]) || ('I' != WaveFileHeader.ChunkID[1]) || ('F' != WaveFileHeader.ChunkID[2]) || ('F' != WaveFileHeader.ChunkID[3]))
	{
		return false;
	}

	// format�� WAVE���� Ȯ��
	if (('W' != WaveFileHeader.Format[0]) || ('A' != WaveFileHeader.Format[1]) || ('V' != WaveFileHeader.Format[2]) || ('E' != WaveFileHeader.Format[3]))
	{
		return false;
	}

	// sub Chunk ID�� fmt���� Ȯ��
	if (('f' != WaveFileHeader.SubChunkID[0]) || ('m' != WaveFileHeader.SubChunkID[1]) || ('t' != WaveFileHeader.SubChunkID[2]) || (' ' != WaveFileHeader.SubChunkID[3]))
	{
		return false;
	}

	// audio format�� WAVE_FORMAT_PCM���� Ȯ��
	if (WAVE_FORMAT_PCM != WaveFileHeader.AudioFormat)
	{
		return false;
	}

	// wav ������ stereo format���� ����Ǿ����� Ȯ��
	if (2 != WaveFileHeader.NumberChannels)
	{
		return false;
	}

	// wav ������ sample rate�� 44.1 KHz���� Ȯ��
	if (44100 != WaveFileHeader.SampleRate)
	{
		return false;
	}

	// wav������ 16bit foramt���� ����Ǿ����� Ȯ��
	if (16 != WaveFileHeader.BitsPerSample)
	{
		return false;
	}

	// wav ������ data chunk header Ȯ��
	if (('d' != WaveFileHeader.DataChunkID[0]) || ('a' != WaveFileHeader.DataChunkID[1]) || ('t' != WaveFileHeader.DataChunkID[2]) || ('a' != WaveFileHeader.DataChunkID[3]))
	{
		return false;
	}

	// wav ������ ������ ������ secondary buffer ���� //
	WAVEFORMATEX WaveFormat;
	memset(&WaveFormat, 0, sizeof(WaveFormat));

	// secondary buffer�� �� �������� format(������ ����) ����
	WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	WaveFormat.nSamplesPerSec = 44100;
	WaveFormat.wBitsPerSample = 16;
	WaveFormat.nChannels = 2;
	WaveFormat.nBlockAlign = (WaveFormat.wBitsPerSample / 8) * WaveFormat.nChannels;
	WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
	WaveFormat.cbSize = 0;

	// secondary buffer ����
	DSBUFFERDESC BufferDesc;
	memset(&BufferDesc, 0, sizeof(BufferDesc));

	BufferDesc.dwSize = sizeof(BufferDesc);
	BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	BufferDesc.dwBufferBytes = WaveFileHeader.DataSize;
	BufferDesc.dwReserved = 0;
	BufferDesc.lpwfxFormat = &WaveFormat;
	BufferDesc.guid3DAlgorithm = GUID_NULL;

	// �ӽ� sound buffer ���� ��, secondary buffer ���� //
	// �ӽ� sound buffer�� ���� IDirectSoundBuffer interface�� ���,
	// IDirectSoundBuffer interface�� ���� IDirectSoundBuffer8 interface�� ��´�.
	// -> �̷��� �����ϴ� ���� : Direct Sound�� ���� ���� ������ IDirectSoundBuffer8 interface�� ���� ������ �� ���� ����

	// �ӽ� sound buffer ����
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&BufferDesc, &tempBuffer, NULL)))
	{
		return false;
	}

	// �ӽ� sound buffer�� ���� IDirectSoundBuffer8�� interface ���
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(*SecondaryBuffer))))
	{
		return false;
	}

	// �ӽ� sound buffer ����
	tempBuffer->Release();
	tempBuffer = nullptr;


	// secondary buffer�� wav ������ ������ ���� //
	// wav ���� �����͸� ���� ������ ã��
	fseek(FilePtr, sizeof(WaveHeaderType), SEEK_SET);

	// wav ���� �����͸� ������ �ӽ� �޸� ����
	unsigned char* WaveData = new unsigned char[WaveFileHeader.DataSize];
	if (!WaveData)
	{
		return false;
	}

	// wav ���� �����͸� �о� �ӽ� �޸𸮿� ����
	count = fread(WaveData, 1, WaveFileHeader.DataSize, FilePtr);
	if (WaveFileHeader.DataSize != count)
	{
		return false;
	}

	// �ӽ� �޸𸮿� ����� �����͸� secondary buffer�� �����ϱ� ���� secondary buffer�� lock
	unsigned char* BufferPtr = nullptr;
	unsigned long BufferSize = 0;
	if (FAILED((*SecondaryBuffer)->Lock(0, WaveFileHeader.DataSize, (void**)&BufferPtr, (DWORD*)&BufferSize, NULL, 0, 0)))
	{
		return false;
	}

	// �ӽ� �޸𸮿� ����� �����͸� secondary buffer�� ����
	memcpy(BufferPtr, WaveData, WaveFileHeader.DataSize);

	// secondary buffer�� unlock
	if (FAILED((*SecondaryBuffer)->Unlock((void*)BufferPtr, BufferSize, NULL, 0)))
	{
		return false;
	}


	// wav ���� �ݱ� //
	error = fclose(FilePtr);
	if (error)
	{
		return false;
	}

	// �ӽ� �޸� ���� //
	delete[] WaveData;
	WaveData = nullptr;

	return true;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** SecondaryBuffer)
{
	if (*SecondaryBuffer)
	{
		(*SecondaryBuffer)->Release();
		*SecondaryBuffer = nullptr;
	}
}

bool SoundClass::PlayWaveFile()
{
	// audio�� ����� ���� ��ġ ���� //
	// ���⼭�� 0(secondary buffer�� ���� ��ġ)�� ����
	// audio�� ����� ���� ��ġ�� �������� ������ ���� �ֱٿ� ����Ǿ��� �κк��� ������
	if (FAILED(m_SecondaryBuffer1->SetCurrentPosition(0)))
	{
		return false;
	}

	// audio�� volume ���� //
	// ���⼭�� 100%�� ����
	if (FAILED(m_SecondaryBuffer1->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}

	// secondary buffer�� ����� ������ ��� //
	if (FAILED(m_SecondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING)))
	{
		return false;
	}

	return true;
}
