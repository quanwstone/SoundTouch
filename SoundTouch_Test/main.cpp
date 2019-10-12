#include<stdio.h>
#include<Windows.h>
#include"SoundTouch.h"

using namespace soundtouch;

#pragma comment(lib,"SoundTouch.lib")

int main(int arvc, char *argv[])
{
	FILE *pPcm = nullptr;
	FILE *pEff = nullptr;

	errno_t er = fopen_s(&pPcm, "../Pcm/6.pcm", "rb+");
	if (er != 0)
	{
		printf("fopen_s failed.\n");
	}
	er = fopen_s(&pEff, "e.pcm", "wb+");
	if (er != 0)
	{
		printf("fopen_s failed.\n");
	}

	DWORD dwSamplesPerSec = 44100;
	WORD wChannels = 2;
	WORD wBitsPerSample = 32;

	//经过配置soundtouch，pcm必修为flt32类型.
	soundtouch::SoundTouch soud;

	soud.setChannels(wChannels);
	soud.setSampleRate(dwSamplesPerSec);
	float fRet = 40 + 100;
	fRet = fRet / 100;
	soud.setPitch(fRet);//修改频率,用于设置音调
	soud.setTempo(fRet);//修改速度.

	soud.setSetting(SETTING_USE_AA_FILTER, 1);
	soud.setSetting(SETTING_USE_QUICKSEEK, 1);

	int iLen = 220 * 2 * 2 * 20;

	char *buf = new char[iLen];
	memset(buf, 0, iLen);

	int iR = fread(buf, 1, iLen, pPcm);

	while (iR)
	{

		long numSamples = iR / (sizeof(soundtouch::SAMPLETYPE) *wChannels);

		soud.putSamples((const SAMPLETYPE *)buf, (uint)numSamples);

		int iRet = 0;
		if (soud.numSamples() >= (iR / (sizeof(soundtouch::SAMPLETYPE))))
		{
			iRet = soud.receiveSamples((soundtouch::SAMPLETYPE*)buf, numSamples);
		}

		if (iRet == 0)
		{
			memset(buf, 0, iR);
		}

		fwrite(buf, 1, iR, pEff);

		iR = fread(buf, 1, iLen, pPcm);
	}

	fclose(pPcm);
	fclose(pEff);

	return 0;
}


