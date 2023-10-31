#pragma once
#include "SampleWave.h"

using namespace std;

class CAdditiveWave :
	public CSampleWave
{
public:
	CAdditiveWave();
	virtual ~CAdditiveWave();

	struct Vibrato
	{
		double vibrato = 0;
		double vibrato_rate = 0;
	};

	virtual void Start() override;
	virtual bool Generate() override;
	virtual void SetWavetables() override;

	void AddHarmonic(double harmonic) { m_harmonics.push_back(harmonic); }
	void ImplementHarmonics(double* sample, double time);
	void ImplementVibrato(double* sine_rads, double* vibrato_rads);
	void GenerateCrossfade(double time, double crossfade_dur);

	vector<double> GetHarmonics() { return m_harmonics; }
	bool GetVibratoFlag() { return m_implement_vibrato; }
	bool GetCrossfadeFlag() { return m_implement_crossfade; }
	double GetCrossfadeStartTime() { return m_crossfade_start_time; }

	void SetCrossfadeFlag(bool flag) { m_implement_crossfade = flag; }
	void SetCrossfadeStartTime(double time) { m_crossfade_start_time = time; }

	void SetVibratoFlag(bool flag) { m_implement_vibrato = flag; }
	void SetVibrato(double vibrato) { m_vibrato_effect.vibrato = vibrato; }
	void SetVibratoRate(double vibrato_rate) { m_vibrato_effect.vibrato_rate = vibrato_rate; }

	void SetNextWave(CAdditiveWave* next) { m_next_wave = next; }

private:
	vector<double> m_harmonics;
	double m_time;
	bool m_implement_vibrato = false;
	bool m_implement_crossfade = false;
	double m_crossfade_start_time;
	Vibrato m_vibrato_effect;
	CAdditiveWave* m_next_wave = nullptr;
};

