#pragma once

#include "msxml2.h"
#include "Instrument.h"
#include "ToneInstrument.h"
#include "Note.h"
#include "AdditiveInstrument.h"
#include "SubtractiveInstrument.h"

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include "ChorusEffect.h"
#include "FlangeEffect.h"
#include "CompressorEffect.h"
#include "NoiseGateEffect.h"

using namespace std;

class CSynthesizer
{
public:
	CSynthesizer();
	virtual ~CSynthesizer();

public:
	int GetNumChannels() { return m_channels; }

	double GetSampleRate() { return m_sampleRate; }

	double GetSamplePeriod() { return m_samplePeriod; }

	void SetNumChannels(int n) { m_channels = n; }

	void SetSampleRate(double s) { m_sampleRate = s;  m_samplePeriod = 1.0 / s; }

	void Start();

	bool Generate(double * frame);

	double GetTime() { return m_time; }

	void Clear();

	void OpenScore(CString & filename);

	double GetBPM() { return m_bpm; }

private:
	int		m_channels;
	double	m_sampleRate;
	double	m_samplePeriod;
	double m_time;
	list<CInstrument *> m_instruments;
	double  m_bpm;            
	int     m_beatspermeasure; 
	double  m_secperbeat;       
	vector<CNote> m_notes;
	int m_currentNote;      
	int m_measure;             
	double m_beat;      

private:
	void XmlLoadScore(IXMLDOMNode * xml);
	void XmlLoadInstrument(IXMLDOMNode * xml);
	void XmlLoadNote(IXMLDOMNode * xml, std::wstring & instrument);

private:
	CChorusEffect mChorusEffect;
	CFlangeEffect mFlangeEffect;
	CCompressorEffect mCompressorEffect;
	CNoiseGateEffect mNoiseGateEffect;
	
};