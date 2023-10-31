#include "stdafx.h"
#include "AdditiveInstrument.h"
#include "Note.h"
#include "Notes.h"
#include "Envelope.h"
#include "ADSREnvelope.h"
#include <sstream>


CAdditiveInstrument::CAdditiveInstrument()
{
	m_duration = 0.1;
}


CAdditiveInstrument::CAdditiveInstrument(double bpm)
{
	m_duration = 0.1;
	m_bpm = bpm;
}


CAdditiveInstrument::~CAdditiveInstrument()
{
}


void CAdditiveInstrument::Start()
{
	m_additive_wave.SetSampleRate(GetSampleRate());
	m_additive_wave.Start();
	m_time = 0;

	m_envelope = new CADSREnvelope();
	static_cast<CADSREnvelope*>(m_envelope)->SetAttack(.05);
	static_cast<CADSREnvelope*>(m_envelope)->SetRelease(.05);

	m_amp_filter.SetEnvelope(m_envelope);
	m_amp_filter.SetSource(&m_additive_wave);
	m_amp_filter.SetSampleRate(GetSampleRate());
	m_amp_filter.SetDuration(m_duration);
	m_amp_filter.Start();
}


bool CAdditiveInstrument::Generate()
{
	// begin envelope generation
	m_envelope->Generate();

	// if cross fade flag is true, implement cross fading
	if (m_additive_wave.GetCrossfadeFlag())
	{
		// know when to end the cross fade effect
		auto crossfade_end_time = m_additive_wave.GetCrossfadeStartTime() + m_crossfade_duration;

		if (m_time < m_additive_wave.GetCrossfadeStartTime())
		{
			// generate wave by adding together several sinusoids (additive wave)
			m_additive_wave.Generate();
		}

		else if (m_time > m_additive_wave.GetCrossfadeStartTime() && m_time < crossfade_end_time)
		{
			// generate the cross fade wave
			m_additive_wave.GenerateCrossfade(m_time, m_crossfade_duration);
		}
	}

	else
	{
		// generate wave by adding together several sinusoids (additive wave)
		m_additive_wave.Generate();
	}

	// generate audio sample
	auto okay = m_amp_filter.Generate();

	// read the sample, make it the result frame
	m_frame[0] = m_amp_filter.Frame(0);
	m_frame[1] = m_amp_filter.Frame(1);

	// update the time
	m_time += GetSamplePeriod();

	// returns true until the time equals the duration
	return okay;
}


void CAdditiveInstrument::SetNote(CNote *note)
{
	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i < len; i++)
	{
		// Get attribute i
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		// Get the name of the attribute
		CComBSTR name;
		attrib->get_nodeName(&name);

		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "duration")
		{
			value.ChangeType(VT_R8);

			SetDuration(value.dblVal * (1 / (m_bpm / 60)));
		}

		else if (name == "note")
		{
			SetFreq(NoteToFrequency(value.bstrVal));
		}

		else if (name == "harmonics")
		{
			AddHarmonics(value.bstrVal);
		}

		else if (name == "vibrato")
		{
			value.ChangeType(VT_R8);
			m_additive_wave.SetVibratoFlag(true);
			m_additive_wave.SetVibrato(value.dblVal);
		}

		else if (name == "vibratoRate")
		{
			value.ChangeType(VT_R8);
			m_additive_wave.SetVibratoRate(value.dblVal);
		}

		else if (name == "crossfade")
		{
			value.ChangeType(VT_R8);

			m_crossfade_duration = value.dblVal;
			m_additive_wave.SetCrossfadeFlag(true);
			auto start_time = m_duration - m_crossfade_duration;

			// set the time to start the cross fade
			m_additive_wave.SetCrossfadeStartTime(start_time);
		}

		else if (name == "crossfadeOverlap")
		{
			// overlap in beats
			value.ChangeType(VT_R8);

			// 1 / (m_bpm / 60) - seconds per beat
			auto overlap_time = (1 / (m_bpm / 60)) * value.dblVal;
			auto crossfade_time = m_additive_wave.GetCrossfadeStartTime();

			// set the time to start the cross fade, taking into account the overlap time
			m_additive_wave.SetCrossfadeStartTime(crossfade_time - overlap_time);
		}
	}
}


void CAdditiveInstrument::SetNextNote(CNote* next_note)
{
	// used for cross fade
	m_next = new CAdditiveInstrument();

	// set the sample rate
	m_next->SetSampleRate(GetSampleRate());
	// move to the next note
	m_next->SetNote(next_note);
	m_next->Start();

	// used for interpolation between sound A and B
	m_additive_wave.SetNextWave(&m_next->m_additive_wave);
}


void CAdditiveInstrument::AddHarmonics(wstring harmonics)
{
	wstringstream sstream(harmonics);
	// individual amplitude in list (like 1, .5, or .7)
	wstring harmonic_amp;

	// read into the wave
	while (sstream >> harmonic_amp) 
	{
		m_additive_wave.AddHarmonic(stod(harmonic_amp));
	}
}
