#pragma once
#include "AudioEffect.h"
class CCompressorEffect :
	public CAudioEffect
{
public:
	CCompressorEffect();
	virtual ~CCompressorEffect();

	virtual void Process(double *input, double *output) override;

	virtual void Start() override;

	virtual bool Generate() override;
};

