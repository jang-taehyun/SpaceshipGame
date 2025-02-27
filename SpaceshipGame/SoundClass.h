#pragma once

class SoundClass
{
private:
	struct WaveHeaderType
	{
		char ChunkID[4];
		unsigned long ChunkSize;
		char Format[4];
		char SubChunkID[4];
		unsigned long SubChunkSize;
		unsigned short AudioFormat;
		unsigned short NumberChannels;
		unsigned long SampleRate;
		unsigned long BytesPerSecond;
		unsigned short BlockAlign;
		unsigned short BitsPerSample;
		char DataChunkID[4];
		unsigned long DataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND);
	void Shutdown();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(const char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();

private:
	IDirectSound8* m_DirectSound = nullptr;
	IDirectSoundBuffer* m_PrimaryBuffer = nullptr;
	IDirectSoundBuffer8* m_SecondaryBuffer1 = nullptr;
};