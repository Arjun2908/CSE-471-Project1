#include "stdafx.h"
#include "ToneInstrument.h"
#include "Notes.h"


CToneInstrument::CToneInstrument()
{
	m_duration = 0.1;
}


CToneInstrument::~CToneInstrument()
{
}

CToneInstrument::CToneInstrument(double bpm)
{
	m_duration = 0.1;
	m_bpm = bpm;
}

void CToneInstrument::Start()
{
	m_sinewave.SetSampleRate(GetSampleRate());
	m_sinewave.Start();
	m_time = 0;

	// Tell the AR object it gets its samples from 
	// the sine wave object.
	m_ar.SetSource(&m_sinewave);
	m_ar.SetSampleRate(GetSampleRate());
	m_ar.Start();
}


bool CToneInstrument::Generate()
{
	m_sinewave.Generate();
	bool valid = m_ar.Generate();

	m_frame[0] = m_ar.Frame(0);
	m_frame[1] = m_ar.Frame(1);

	m_time += GetSamplePeriod();
	return valid;
}

void CToneInstrument::SetNote(CNote *note)
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

	}

}