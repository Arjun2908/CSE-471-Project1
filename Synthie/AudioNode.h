#pragma once
class CAudioNode
{
public:
	CAudioNode();
	virtual ~CAudioNode();

	CAudioNode(double bpm);

public:
	virtual void Start() = 0;

	virtual bool Generate() = 0;

	double GetSampleRate() { return m_sampleRate; }

	double GetSamplePeriod() { return m_samplePeriod; }

	void SetSampleRate(double s) { m_sampleRate = s;  m_samplePeriod = 1 / s; }

	const double *Frame() { return m_frame; }

	double Frame(int c) { return m_frame[c]; }

protected:
	double m_sampleRate;
	double m_samplePeriod;
	double m_frame[2];
	double m_bpm;
};

