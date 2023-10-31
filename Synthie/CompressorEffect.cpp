#include "stdafx.h"
#include "CompressorEffect.h"


CCompressorEffect::CCompressorEffect()
{
}


CCompressorEffect::~CCompressorEffect()
{
}

void CCompressorEffect::Process(double* input, double* output)
{
	if (input[0] > mThreshold || input[0] < -mThreshold)
	{
		double difference = input[0] - mThreshold;
		double compressedFrame = (difference / 1.5) + mThreshold;
		output[0] = mDry * input[0] + mWet * compressedFrame;
	}
	else
	{
		output[0] = input[0] * mWet;
	}

	if (input[1] > mThreshold || input[1] < -mThreshold)
	{
		double difference = input[1] - mThreshold;
		double compressedFrame = (difference / 1.5) + mThreshold;
		output[1] = mDry * input[1] + mWet * compressedFrame;
	}
	else
	{
		output[1] = input[1] * mWet;
	}
}

void CCompressorEffect::Start()
{
}

bool CCompressorEffect::Generate()
{
	return true;
}