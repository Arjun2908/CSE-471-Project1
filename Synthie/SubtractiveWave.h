#pragma once
#include "SampleWave.h"
#include <string>
using namespace std;

class CSubtractiveWave :
	public CSampleWave
{
public:
	CSubtractiveWave();
	~CSubtractiveWave();

	void Start();
	bool Generate();

	void ImplementReson(double* sample, int tableIndex);

	//Setters
	void SetWaveType(wstring type){ m_wave_type = type; }
	void SetReson(bool reson){ m_implement_reson = reson; }
	void SetResonFreq(double freq){ m_reson_freq = freq; }
	void SetResonBandwidth(double band){ m_reson_bandwidth = band; }
	void SetMoogSweepDown(bool flag){ m_sweep_down = flag; }
	void SetMoogSweepUp(bool flag){ m_sweep_up = flag; }
	void SetMoogSweepDownUp(bool flag){ m_sweep_down_up = flag; }
	void SetMoogSweepUpDown(bool flag){ m_sweep_up_down = flag; }
	void SetWavetables();

private:
	wstring m_wave_type;
	double m_time;
	bool m_implement_reson = false;
	double m_reson_freq = 0.01134;
	double m_reson_bandwidth = 0.01;
	double m_sweep_freq = 0.0;
	bool m_sweep_down = false;
	bool m_sweep_up = false;
	bool m_sweep_down_up = false;
	bool m_sweep_up_down = false;
};
