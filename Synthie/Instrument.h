#pragma once
#include "AudioNode.h"
#include "Note.h"
#include "AmpFilter.h"

class CInstrument :
	public CAudioNode
{
public:
	CInstrument();
	~CInstrument();
	CInstrument(double);

	virtual void SetNote(CNote *note) = 0;
	double Send(int i) { return mSends[i]; }
	void SetSend(int i, double value) { mSends[i] = value; }


protected:
	// the amplitude filter object
	CAmpFilter m_amp_filter;
	// the envelope object
	CEnvelope* m_envelope;

	double mSends[5];
};

