#pragma once
#include "AudioEffect.h"
class CFlangeEffect :
	public CAudioEffect
{
public:
	CFlangeEffect();
	virtual ~CFlangeEffect();

	virtual void Process(double *input, double *output) override;

	virtual void Start() override;

	virtual bool Generate() override;

private:
	std::vector<double> mOutL;
	std::vector<double> mOutR;
};

