#include "stdafx.h"
#include "Synthesizer.h"
#include "xmlhelp.h"

#include <cmath>

CSynthesizer::CSynthesizer()
{
	CoInitialize(NULL);

	m_channels = 2;
	m_sampleRate = 44100.;
	m_samplePeriod = 1 / m_sampleRate;

	m_bpm = 120;
	m_secperbeat = 0.5;
	m_beatspermeasure = 4;
	m_time = 0;
}


CSynthesizer::~CSynthesizer()
{
}

//! Start the synthesizer
void CSynthesizer::Start(void)
{
	//
	m_instruments.clear();
	m_currentNote = 0;
	m_measure = 0;
	m_beat = 0;
	m_time = 0;
}

//! Generate one audio frame
bool CSynthesizer::Generate(double * frame)
{
	// Which effects will be sent for this frame
	bool sends[] = { false, false, false, false };

	//
	// Phase 1: Determine if any notes need to be played.
	//

	while (m_currentNote < (int)m_notes.size())
	{
		// Get a pointer to the current note
		CNote *note = &m_notes[m_currentNote];

		// If the measure is in the future we can't play
		// this note just yet.
		if (note->Measure() > m_measure)
			break;

		// If this is the current measure, but the
		// beat has not been reached, we can't play
		// this note.
		if (note->Measure() == m_measure && note->Beat() > m_beat)
			break;

		//
		// Play the note!
		//

		// Create the instrument object
		CInstrument *instrument = NULL;
		if (note->Instrument() == L"ToneInstrument")
		{
			instrument = new CToneInstrument(GetBPM());
		}

		else if (note->Instrument() == L"AdditiveInstrument")
		{
			instrument = new CAdditiveInstrument(GetBPM());

			// used for note overlapping, do this if required
			// apparent in cross fading
			if ((m_currentNote + 1) < m_notes.size())
			{
				static_cast<CAdditiveInstrument*>(instrument)->SetNextNote(&m_notes[m_currentNote + 1]);
			}
		}
		else if (note->Instrument() == L"SubtractiveInstrument")
		{
			instrument = new CSubtractiveInstrument(GetBPM());
		}
		else if (note->Instrument() == L"Chorus")
		{
			sends[0] = true;
			mChorusEffect.SetNote(note);
			mChorusEffect.Start();
		}
		else if (note->Instrument() == L"Flange")
		{
			sends[1] = true;
			mFlangeEffect.SetNote(note);
			mFlangeEffect.Start();
		}
		else if (note->Instrument() == L"Compress")
		{
			sends[2] = true;
			mCompressorEffect.SetNote(note);
			mCompressorEffect.Start();
		}
		else if (note->Instrument() == L"NoiseGate")
		{
			sends[3] = true;
			mNoiseGateEffect.SetNote(note);
			mNoiseGateEffect.Start();
		}

		// Configure the instrument object
		if (instrument != NULL)
		{
			instrument->SetSampleRate(GetSampleRate());
			instrument->SetNote(note);
			instrument->Start();

			m_instruments.push_back(instrument);
		}

		m_currentNote++;
	}

	//
	// Phase 2: Clear all channels to silence 
	//

	// From the tips and questions for effects
	double channelframes[5][2];
	for (int i = 0; i < 5; i++)
	{
		for (int c = 0; c < GetNumChannels(); c++)
		{
			channelframes[i][c] = 0;
		}
	}

	for (int c = 0; c<GetNumChannels(); c++)
	{
		frame[c] = 0;
	}

	//
	// Phase 3: Play an active instruments
	//

	for (list<CInstrument *>::iterator node = m_instruments.begin(); node != m_instruments.end();)
	{
		// Since we may be removing an item from the list, we need to know in 
		// advance, what is after it in the list.  We keep that node as "next"
		list<CInstrument *>::iterator next = node;
		next++;

		// Get a pointer to the allocated instrument
		CInstrument *instrument = *node;

		// Call the generate function
		if (instrument->Generate())
		{
			// If we returned true, we have a valid sample.  Add it 
			// to the frame.
			/*for (int c = 0; c<GetNumChannels(); c++)
			{
			frame[c] += instrument->Frame(c);
			}*/
			for (int i = 0; i < 4; i++)
			{
				if (sends[i])
				{
					instrument->SetSend(i+1, 1 + i * .5);
					instrument->SetSend(0, 0);
				}
			}

			for (int i = 0; i < 5; i++)
			{
				for (int c = 0; c < GetNumChannels(); c++)
				{
					channelframes[i][c] += instrument->Frame(c) * instrument->Send(i);
				}
			}

		}
		else
		{
			// If we returned false, the instrument is done.  Remove it
			// from the list and delete it from memory.
			m_instruments.erase(node);
			delete instrument;
		}

		//
		// Phase 3a: Effects!
		//

		double frames[2];
		for (int i = 0; i < GetNumChannels(); i++)
		{
			frames[i] = channelframes[0][i];
		}

		double chorusframes[2], flangeframes[2], compressframes[2], noisegateframes[2];
		for (int i = 0; i < 2; i++)
		{
			chorusframes[i] = flangeframes[i] = compressframes[i] = noisegateframes[i] = 0;
		}
		
		if (channelframes[1][0] != 0)
		{
			mChorusEffect.Process(channelframes[1], chorusframes);
		}
		if (channelframes[2][0] != 0)
		{
			mFlangeEffect.Process(channelframes[2], flangeframes);
		}
		if (channelframes[3][0] != 0)
		{
			mCompressorEffect.Process(channelframes[3], compressframes);
		}
		if (channelframes[4][0] != 0)
		{
			mNoiseGateEffect.Process(channelframes[4], noisegateframes);
		}

		for (int i = 0; i < GetNumChannels(); i++)
		{
			frame[i] += frames[i];
			frame[i] += chorusframes[i];
			frame[i] += flangeframes[i];
			frame[i] += compressframes[i];
			frame[i] += noisegateframes[i];
		}



		// Move to the next instrument in the list
		node = next;
	}

	//
	// Phase 4: Advance the time and beats
	//

	// Time advances by the sample period
	m_time += GetSamplePeriod();

	m_beat += GetSamplePeriod() / m_secperbeat;

	if (m_beat > m_beatspermeasure)
	{
		m_beat -= m_beatspermeasure;
		m_measure++;
	}

	//
	// Phase 5: Determine when we are done
	//

	return !m_instruments.empty() || m_currentNote < (int)m_notes.size();
}

void CSynthesizer::Clear(void)
{
	m_instruments.clear();
	m_notes.clear();
}

void CSynthesizer::OpenScore(CString & filename)
{
	Clear();

	//
	// Create an XML document
	//

	CComPtr<IXMLDOMDocument>  pXMLDoc;
	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument, (void**)&pXMLDoc));
	if (!succeeded)
	{
		AfxMessageBox(L"Failed to create an XML document to use");
		return;
	}

	// Open the XML document
	VARIANT_BOOL ok;
	succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(filename), &ok));
	if (!succeeded || ok == VARIANT_FALSE)
	{
		AfxMessageBox(L"Failed to open XML score file");
		return;
	}

	//
	// Traverse the XML document in memory!!!!
	// Top level tag is <score>
	//

	CComPtr<IXMLDOMNode> node;
	pXMLDoc->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR nodeName;
		node->get_nodeName(&nodeName);

		if (nodeName == "score")
		{
			XmlLoadScore(node);
		}

	}

	sort(m_notes.begin(), m_notes.end());
}

void CSynthesizer::XmlLoadScore(IXMLDOMNode * xml)
{
	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
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

		if (name == L"bpm")
		{
			value.ChangeType(VT_R8);
			m_bpm = value.dblVal;
			m_secperbeat = 1 / (m_bpm / 60);
		}
		else if (name == L"beatspermeasure")
		{
			value.ChangeType(VT_I4);
			m_beatspermeasure = value.intVal;
		}

	}

	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);

		if (name == L"instrument")
		{
			XmlLoadInstrument(node);
		}

	}
}

void CSynthesizer::XmlLoadInstrument(IXMLDOMNode * xml)
{
	wstring instrument = L"";

	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
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

		// Get the value of the attribute.  
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "instrument")
		{
			instrument = value.bstrVal;
		}
	}


	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);

		if (name == L"note")
		{
			XmlLoadNote(node, instrument);
		}
	}
}

void CSynthesizer::XmlLoadNote(IXMLDOMNode * xml, std::wstring & instrument)
{
	m_notes.push_back(CNote());
	m_notes.back().XmlLoad(xml, instrument);
}