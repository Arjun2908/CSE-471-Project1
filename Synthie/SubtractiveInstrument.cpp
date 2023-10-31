#include "stdafx.h"
#include "SubtractiveInstrument.h"
#include "Notes.h"
#include "Envelope.h"
#include "ADSREnvelope.h"
CSubtractiveInstrument::CSubtractiveInstrument()
{
	m_duration = 0.1;
}


CSubtractiveInstrument::~CSubtractiveInstrument()
{
}
CSubtractiveInstrument::CSubtractiveInstrument(double bpm)
{
	m_duration = 0.1;
	m_bpm = bpm;
}
void CSubtractiveInstrument::SetNote(CNote *note)
{
	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i<len; i++)
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
			SetDuration(value.dblVal * (60.0 / m_bpm));
		}

		else if (name == "note")
		{
			SetFreq(NoteToFrequency(value.bstrVal));
		}
		//Set what type of wave it is according to the xml
		else if (name == "wavetype")
		{
			if ((wstring)value.bstrVal == L"Sawtooth"){
				m_wave_type = L"Sawtooth";
				SetWaveType(L"Sawtooth");
			}
			else if ((wstring)value.bstrVal == L"Square"){
				m_wave_type = L"Square";
				SetWaveType(L"Square");
			}
			else if ((wstring)value.bstrVal == L"Triangle"){
				m_wave_type = L"Triangle";
				SetWaveType(L"Triangle");
			}
		}
		else if (name == "resonFreq")
		{                          
			mSubtractiveWave.SetReson(true);
			value.ChangeType(VT_R8);
			mSubtractiveWave.SetResonFreq(value.dblVal);
		}
		else if (name == "bandwidth")
		{
			mSubtractiveWave.SetReson(true);
			value.ChangeType(VT_R8);
			mSubtractiveWave.SetResonBandwidth(value.dblVal);
		}
		else if (name == "sweep")
		{
			if ((wstring)value.bstrVal == L"up"){
				mSubtractiveWave.SetMoogSweepUp(true);
			}
			else if ((wstring)value.bstrVal == L"down"){
				mSubtractiveWave.SetMoogSweepDown(true);
			}
			else if ((wstring)value.bstrVal == L"up-down"){
				mSubtractiveWave.SetMoogSweepUpDown(true);
			}
			else if ((wstring)value.bstrVal == L"down-up"){
				mSubtractiveWave.SetMoogSweepDownUp(true);
			}
		}
	}
	
}

void CSubtractiveInstrument::Start()
{
	// set the sample rate of the wave
	mSubtractiveWave.SetSampleRate(GetSampleRate());
	mSubtractiveWave.Start();
	m_time = 0;

	// create a new envelope object
	m_envelope = new CADSREnvelope();
	// set the attack and release of the ADSR envelope
	static_cast<CADSREnvelope*>(m_envelope)->SetAttack(.05);
	static_cast<CADSREnvelope*>(m_envelope)->SetRelease(.05);

	// set the envelope of the filter
	m_amp_filter.SetEnvelope(m_envelope);
	// set the source to be the additive wave object
	m_amp_filter.SetSource(&mSubtractiveWave);
	// set the sample rate
	m_amp_filter.SetSampleRate(GetSampleRate());
	// set the duration
	m_amp_filter.SetDuration(m_duration);
	m_amp_filter.Start();
}
bool CSubtractiveInstrument::Generate()
{
	// begin envelope generation
	m_envelope->Generate();

	mSubtractiveWave.Generate();

	auto continuePlay = m_amp_filter.Generate();

	m_frame[0] = mSubtractiveWave.Frame(0);
	m_frame[1] = mSubtractiveWave.Frame(1);

	// update the time
	m_time += GetSamplePeriod();

	return continuePlay;
}


