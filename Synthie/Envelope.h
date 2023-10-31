#pragma once
#include "AudioNode.h"

class CEnvelope :
	public CAudioNode
{
public:
	CEnvelope();
	virtual ~CEnvelope();

	virtual void Start() override {};
	virtual bool Generate() override { return false; }

	double GetEnvelopeLevel() { return m_level; }

	void SetDuration(double duration) { m_duration = duration; }

protected:
	double m_duration;
	double m_time;
	double m_level;
};

