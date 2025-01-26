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
    midiCollector.reset (sampleRate);
}

void SynthAudioSource::getNextAudioBlock (const juce::AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    //we pull buffers of MIDI data from the MidiKeyboardState object.
    juce::MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
    keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                         bufferToFill.numSamples, true);

    //These buffers of MIDI are passed to the synthesiser which will be used to render
    //the voices using the timestamps of the note-on and note-off messages (and other MIDI channel voice messages).
    synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                           bufferToFill.startSample, bufferToFill.numSamples);
}

juce::MidiMessageCollector* SynthAudioSource::getMidiCollector()
{
    return &midiCollector;
}


void SynthAudioSource::releaseResources ()
{
    
}
