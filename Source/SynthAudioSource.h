/*
  ==============================================================================

    SynthAudioSource.h
    Created: 21 Jan 2025 7:44:15pm
    Author:  Keegan Moseley

  ==============================================================================
*/ 
#pragma once
#include <JuceHeader.h>
#include "SineWaveVoice.h"
#include "SineWaveSound.h"
#include "CustomSynth.h"

/*
 This outputs all of the audio from the synthesiser.
 */
class SynthAudioSource  :   public juce::AudioSource
{
public:
    SynthAudioSource (juce::MidiKeyboardState& keyState);
    
    ~SynthAudioSource();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources () override;
    
    void setUsingSineWaveSound();
    juce::MidiMessageCollector* getMidiCollector();
    
private:
    juce::MidiKeyboardState& keyboardState;
    //juce::Synthesiser synth;
    CustomSynth synth;
    juce::MidiMessageCollector midiCollector;

};
