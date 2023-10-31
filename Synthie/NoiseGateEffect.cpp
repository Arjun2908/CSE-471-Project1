#include "stdafx.h"
#include "NoiseGateEffect.h"


CNoiseGateEffect::CNoiseGateEffect()
{
}


CNoiseGateEffect::~CNoiseGateEffect()
{
}

void CNoiseGateEffect::Process(double* input, double* output)
{
	if (input[0] < mThreshold && input[0] > -mThreshold)
	{
		mGateL -= .005;
		if (mGateL < 0)
			mGateL = 0;
	}
	else
	{
		mGateL += .005;
		if (mGateL > 1)
			mGateL = 1;
	}
	output[0] = (mDry * input[0]) + (mWet * input[0] * mGateL);

	if (input[1] < mThreshold && input[1] > -mThreshold)
	{
		mGateR -= .005;
		if (mGateR < 0)
			mGateR = 0;
	}
	else
	{
		mGateR += .005;
		if (mGateR > 1)
			mGateR = 1;
	}
	output[1] = (mDry * input[1]) + (mWet * input[1] * mGateR);


}

void CNoiseGateEffect::Start()
{
	mWrloc = 0;
	mRdloc = 0;
	mGateR = 1;
	mGateL = 1;
}

bool CNoiseGateEffect::Generate()
{
	return true;
}