/*
  ==============================================================================

    SineWaveSound.h
    Created: 21 Jan 2025 7:45:22pm
    Author:  Keegan Moseley

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/*
 A sound class is effectively a description of the sound that can be created as a voice. For example, this may contain the sample data for a sampler voice or the wavetable data for a wavetable synthesiser.
 */
struct SineWaveSound   : public juce::SynthesiserSound
{
    SineWaveSound() {}
 
    bool appliesToNote    (int) override        {return true; }
    bool appliesToChannel (int) override        { return true; }
};

