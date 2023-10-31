#include "stdafx.h"
#include "ADSREnvelope.h"


CADSREnvelope::CADSREnvelope()
{
	m_attack = 0.05;
	m_decay = 0.;
	m_sustain = 1.;
	m_release = 0.05;
}


CADSREnvelope::~CADSREnvelope()
{
}


void CADSREnvelope::Start()
{
	m_level = 1.;
	m_time = 0.;
}


bool CADSREnvelope::Generate()
{
	// ADSR

	// A - Attack
	if (m_time <= m_attack)
	{
		m_level = m_time / m_attack * m_sustain;
	}

	// R - Release
	else if ((m_duration - m_release) < m_time)
	{
		m_level = (1 - (m_time - (m_duration - m_release)) / m_release) * m_sustain;
	}

	// D - Decay
	else if (m_time > m_attack && m_time <= m_attack + m_decay)
	{
		m_level = (m_level - 1) * ((m_time - (m_duration - m_decay)) / m_decay) + 1;
	}

	// S - Sustain
	else
	{
		m_level = m_sustain;
	}

	// update the time
	m_time += GetSamplePeriod();

	return m_duration > m_time;
}
