#include "stdafx.h"
#include "SubtractiveWave.h"
#include <cmath>

CSubtractiveWave::CSubtractiveWave()
{
	m_phase = 0;
	m_amp = 0.1;
	m_freq = 440;
}


CSubtractiveWave::~CSubtractiveWave()
{
}

void CSubtractiveWave::Start()
{
	m_phase = 0;
	m_time = 0;
	SetWavetables();
}

bool CSubtractiveWave::Generate()
{
	m_frame[0] = m_wavetable[m_phase];
	m_frame[1] = m_frame[0];
	m_phase = (m_phase + 1) % m_wavetable.size();

	return true;
}

void CSubtractiveWave::SetWavetables()
{
	auto table_size = GetSampleRate();
	m_wavetable.resize(table_size);
	double amplitudeFilter = 1.0;

	if (m_freq > 1000. / 44100){
		amplitudeFilter = 0.75;
	}
	if (m_freq > 1200. / 44100){
		amplitudeFilter = 0.65;
	}
	if (m_freq > 1400. / 44100){
		amplitudeFilter = 0.4;
	}

	double setFreq = m_freq;
	for (auto i = 0; i < table_size; i++, m_time += 1. / GetSampleRate()){
		double sample = 0.0;
		
		if (m_sweep_down){
			setFreq -= 440. / table_size;
		}
		else if (m_sweep_up){
			setFreq += 440. / table_size;
		}
		else if (m_sweep_down_up){
			if (i < table_size / 2){
				setFreq -= 440. / table_size;
			}
			else{
				setFreq += 440. / table_size;
			}
		}
		else if (m_sweep_up_down){
			if (i < table_size / 2){
				setFreq += 440. / table_size;
			}
			else{
				setFreq -= 440. / table_size;
			}
		}
		if (m_wave_type == L"Sawtooth"){
			
			//Initial sin wave
			sample = amplitudeFilter * m_amp * sin(m_time * 2 * PI * setFreq);

			//Add every harmonic up to nyqist frequency
			for (double h = 2.0; h <= (GetSampleRate() / 2.0) / m_freq; h++){
				sample += (amplitudeFilter * m_amp / h) * sin(m_time * 2 * PI  * setFreq * h);
			}
		}
		else if (m_wave_type == L"Square"){
			//Initial sin wave
			sample = amplitudeFilter * m_amp * sin(m_time * 2 * PI * setFreq);

			//Add every odd harmonic up to nyqist frequency
			for (double h = 3.0; h <= (GetSampleRate() / 2.0) / m_freq; h++){
				if ((int)h % 2 == 1)
					sample += (amplitudeFilter * m_amp / h) * sin(m_time * 2 * PI  * setFreq * h);
			}
		}
		else if (m_wave_type == L"Triangle"){
			int alternatingSign = -1;
			//Initial sin wave
			sample = amplitudeFilter * m_amp * sin(m_time * 2 * PI * setFreq);

			//Add every odd harmonic up to nyqist frequency
			for (double h = 3.0; h <= (GetSampleRate() / 2.0) / m_freq; h++){
				if ((int)h % 2 == 1){
					sample += (alternatingSign * amplitudeFilter * m_amp / (h*h)) * sin(m_time * 2 * PI  * setFreq * h);
					alternatingSign = -alternatingSign;
				}
			}
		}

		//Implement Reson
		if (m_implement_reson && i > 1){
			ImplementReson(&sample, i);
		}

		//Implement Filter envelope***

		m_wavetable[i] = sample;
	}
}
//Implement the Reson Filter
void CSubtractiveWave::ImplementReson(double* sample, int tableIndex){
	//Need to have at least two samples to start doing reson
	
	//Calculate reson values for reson equation
	double R = 1.0 - (m_reson_bandwidth / 2.0);
	double cosTheta = (2 * R * cos(2 * PI * m_reson_freq)) / (1 + pow(R, 2));
	double A = (1 - pow(R, 2)) * sqrt(1 - pow(cosTheta, 2));

	int prevIndex; //tableIndex - 1
	int secondPrevIndex;//tableIndex - 2

	//Set two previous table index's and loop around if need be
	if (tableIndex - 1 < 0)
		prevIndex = m_wavetable.size() - 1;
	else
		prevIndex = tableIndex - 1;
	if (tableIndex - 2 < 0)
		secondPrevIndex = m_wavetable.size() - 1;
	else
		secondPrevIndex = tableIndex - 2;

	*sample = A * *sample + (2 * R*cosTheta) * m_wavetable[prevIndex] - pow(R, 2) * m_wavetable[secondPrevIndex];


}