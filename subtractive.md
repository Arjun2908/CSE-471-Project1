---
title: Subtractive Synthesis
layout: template
filename: subtractive.md
--- 

[Go Back](https://arjun2908.github.io/CSE-471-Project1/)

# Subtractive Synthesis #

The subtractive synthesizer operates by generating waves devoid of certain harmonics and filtering them with a reson filter to define specific frequencies.

The ADSREnvelope class, which manages Attack, Release, Decay, and Sustain, is a descendant of the Envelope class. The SampleWave class serves as the foundation for the SubractiveWave class, which creates the different subtractive waves. It covers how to generate the wave, set the wavetable, and put the reson filter into practice. The purpose of the AmpFilter, which derives from the AudioFilter class, is to regulate the wave's amplitude to produce a clear, smooth sound. After the score file is read, the Synthesizer class creates the SubtractiveInstrument class, which is used to configure the SubtractiveWave. Here, it is decided which wave type—sawtooth, square, or triangle—as well as which frequency and bandwidth the reson filter should focus on.

Grading Elements Supported:
- 10 - Waveform playback from tables
- 20 - Varying pitch playback from tables
- 30 - Envelope generation
- 35 - Polyphony
- 40 - Reson implementation and the Moog sound
- 50 - Filter envelopes
