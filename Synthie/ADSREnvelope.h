#pragma once
#include "Envelope.h"

class CADSREnvelope :
	public CEnvelope
{
public:
	CADSREnvelope(void);
	virtual ~CADSREnvelope();

	virtual void Start() override;
	virtual bool Generate() override;

	void SetAttack(double attack) { m_attack = attack; }
	void SetDecay(double decay) { m_decay = decay; }
	void SetSustain(double sustain) { m_sustain = sustain; }
	void SetRelease(double release) { m_release = release; }

private:
	double m_attack;
	double m_decay;
	double m_sustain;
	double m_release;
};

