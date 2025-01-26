/*
  ==============================================================================

    CustomSynth.h
    Created: 25 Jan 2025 6:08:52pm
    Author:  Keegan Moseley

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// Custom Synth Class
class CustomSynth : public juce::Synthesiser
{
public:
    CustomSynth() = default; // Constructor
    ~CustomSynth() override = default; // Destructor

    /*
     The main point of this override is to filter incoming Midi messages, in order to produce a
     drone effect when a key is pressed
    */
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, const juce::MidiBuffer& incomingMidi, int startSample, int numSamples)
    {

        //custom buffer to give to default function
        juce::MidiBuffer customBuffer = juce::MidiBuffer();
        
        //iterate through midi events
        for (const auto meta : incomingMidi) {
            //Get the MidiMessage Object
            const auto msg = meta.getMessage();
            
            int samplePos = meta.samplePosition;
            int noteVal = msg.getNoteNumber();
            int midiChannel = msg.getChannel();
            //DBG("MIDI Message: " << msg.getDescription());
            
            //if its a key down msg
            if (msg.isNoteOn()){
                
                //check for if this note is already playing
                if (playingNotes.count(noteVal) != 0)
                {
                    //the note is already being played! Turn off the note
                    juce::MidiMessage noteOffMsg = juce::MidiMessage::noteOff(midiChannel, noteVal);
                    customBuffer.addEvent(noteOffMsg, samplePos);
                    
                    playingNotes.erase(noteVal);
                }
                else
                {

                    //can play a finite amount of voices at any moment
                    if (playingNotes.size() < getNumVoices())
                    {
                        //only send note on msg, to create a drone
                        customBuffer.addEvent(msg, samplePos);
                        
                        playingNotes[noteVal] = noteVal;
                    }
                }
            }
        }
        
        //juce::Synthesiser::renderNextBlock(outputBuffer, incomingMidi, startSample, numSamples);
        juce::Synthesiser::renderNextBlock(outputBuffer, customBuffer, startSample, numSamples);
    }

private:
    std::map<int, int> playingNotes = {};
};

