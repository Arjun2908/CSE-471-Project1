#pragma once
#include "AudioNode.h"
class CSineWave :
	public CAudioNode
{
public:
	CSineWave();
	~CSineWave();

public:
	virtual void Start();

	virtual bool Generate();

	void SetFreq(double f) { m_freq = f; }

	void SetAmplitude(double a) { m_amp = a; }

private:
	double m_freq;
	double m_amp;
	double m_phase;
};

