#pragma once
#include "Instrument.h"
#include "AdditiveWave.h"

#include <list>

using namespace std;

class CAdditiveInstrument :
	public CInstrument
{
public:
	CAdditiveInstrument(void);
	virtual ~CAdditiveInstrument();
	CAdditiveInstrument(double bpm);

	// Start the node generation
	virtual void Start() override;
	// Cause one sample to be generated
	virtual bool Generate() override;
	virtual void SetNote(CNote *note) override;

	// used for cross fading, allow notes to play over each other
	void SetNextNote(CNote* next_note);
	// add harmonics to the additive wave
	void AddHarmonics(std::wstring harmonics);

	// SETTERS
	void SetFreq(double freq) { m_additive_wave.SetFreq(freq); }
	void SetAmplitude(double amp) { m_additive_wave.SetAmp(amp); }
	void SetDuration(double duration) { m_duration = duration; }

private:
	double m_duration;
	double m_time;
	double m_crossfade_duration = 0;
	CAdditiveWave m_additive_wave;
	CAdditiveInstrument* m_next = nullptr;
};

