/*
  ==============================================================================

    SineWaveVoice.h
    Created: 21 Jan 2025 7:44:37pm
    Author:  Keegan Moseley

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SineWaveSound.h"

/*
 A voice class renders one of the voices of the synthesiser mixing it with the other sounding voices in a Synthesiser object. A single instance of a voice class renders one voice.
 */
class SineWaveVoice :   public juce::SynthesiserVoice
{
public:
    SineWaveVoice(int vId);
    ~SineWaveVoice();
    
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    
    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

    void stopNote (float /*velocity*/, bool allowTailOff) override;
    
    void setLevel(float newLevel);
    
    //================ Not implemented
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    
    
    int voiceId;
private:
    //It needs to maintain the state of one of the voices of the synthesiser. For our sine wave, we need these data members:
    
    //current phase angle
    double currentAngle = 0.0;
    //amount by which the phase angle needs to increment for each output sample
    double angleDelta = 0.0;
    //effects amplitude/loudness
    double level = 0.0;
    //make the note have decay. 0 at the start of each note playing
    double tailOff = 0.0;

};
