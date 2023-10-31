#include "stdafx.h"
#include "Instrument.h"


CInstrument::CInstrument()
{
	mSends[0] = 1;
	mSends[1] = 0;
	mSends[2] = 0;
	mSends[3] = 0;
	mSends[4] = 0;
	mSends[5] = 0;
}


CInstrument::~CInstrument()
{
}

CInstrument::CInstrument(double bpm)
{
	m_bpm = bpm;
}