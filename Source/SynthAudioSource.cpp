/*
  ==============================================================================

    SynthAudioSource.cpp
    Created: 21 Jan 2025 7:44:15pm
    Author:  Keegan Moseley

  ==============================================================================
*/ 

#include "SynthAudioSource.h"

SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState) : keyboardState(keyState)
{
    //decide how many voices the synth can have
    for (auto i = 0; i < 4; ++i)
    {
        DBG("Added voice");
        synth.addVoice (new SineWaveVoice(i));
    }
    
    //define a sound for it to use to play each voice
    synth.addSound (new SineWaveSound());
    
    synth.setNoteStealingEnabled(false);
}

SynthAudioSource::~SynthAudioSource(){};


void SynthAudioSource::setUsingSineWaveSound()
{
    synth.clearSounds();
}

void SynthAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //synth needs to know the sample rate for playing audio
    synth.setCurrentPlaybackSampleRate (sampleRate);
}

void SynthAudioSource::getNextAudioBlock (const juce::AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    //we pull buffers of MIDI data from the MidiKeyboardState object.
    juce::MidiBuffer incomingMidi;
    keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                         bufferToFill.numSamples, true);

    //These buffers of MIDI are passed to the synthesiser which will be used to render
    //the voices using the timestamps of the note-on and note-off messages (and other MIDI channel voice messages).
    synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                           bufferToFill.startSample, bufferToFill.numSamples);
}

void SynthAudioSource::setGain(float newGain)
{
    gain = newGain;
    
    int numVoices = synth.getNumVoices();
    for (int i = 0; i < numVoices; i++)
    {
        SineWaveVoice* voice = (SineWaveVoice*) synth.getVoice(i);
        voice -> setLevel(newGain);
    }
}

void SynthAudioSource::clearNotesAndStop()
{
    synth.clearNotesAndStop();
}

void SynthAudioSource::releaseResources ()
{
    
}
