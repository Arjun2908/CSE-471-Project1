---
title: Effects
layout: template
filename: effects.md
--- 

[Go back](https://arjun2908.github.io/CSE-471-Project1/)

The final step in the synthesizing process is the effects. Every component produces its own audio, which it then sends to the program's effects section to be modified by the appropriate effect or effects that are applied. After that, the effects are combined and sent to the synthesizer.

All effects use the AudioEffect class as their foundation class. All effects require these member variables and tiny functions. Constants for PI, level, rate, and range are included in this basic class and will be used for various effects. I have hard coded the rate, level, and range as constants rather than include them in the xml. The ChorusEffect is the initial effect. The original audio is combined with a delayed replica of the audio to create this effect. This is not an echo; rather, it is a combination of sounds that resembles a chorus singing or several piano notes being played simultaneously. The easiest way to display this effect is with a longer sustained note.

The Compressor Effect is the second effect. By using the xml score to build a threshold and then lowering the loudness of sounds that are higher than this threshold, this effect is produced. The sound flows through unaffected if the threshold is not broken. FlangeEffect is the third effect. To get this effect, combine the original audio signal with a delayed version of the same sound that varies in time. As a result, the sound has a sweeping impact. The NoiseGateEffect is the final and fourth effect. The xml score was used to create a threshold, or the minimum volume required to flow past the gate, in order to achieve this effect.

In the event that the volume is below the threshold, it will be progressively lowered over a brief period of time by a tiny factor before being completely eliminated.

Grading Elements Supported:
10 - Component passes audio
20 - 1 Effect
30 - 3 Effects
40 - Controllable effects send
50 - 4 Effects
