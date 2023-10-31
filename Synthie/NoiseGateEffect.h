#pragma once
#include "AudioEffect.h"
#include "CAR.h"

class CNoiseGateEffect :
	public CAudioEffect
{
public:
	CNoiseGateEffect();
	virtual ~CNoiseGateEffect();

	virtual void Process(double *input, double *output) override;

	virtual void Start() override;

	virtual bool Generate() override;

private:
	double mGateL;
	double mGateR;

};

