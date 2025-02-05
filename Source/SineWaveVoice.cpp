/*
  ==============================================================================

    SineWaveVoice.cpp
    Created: 21 Jan 2025 7:44:37pm
    Author:  Keegan Moseley

  ==============================================================================
*/

#include "SineWaveVoice.h"

SineWaveVoice::SineWaveVoice(int vId){
    voiceId = vId;
};

SineWaveVoice::~SineWaveVoice(){};

//return whether the voice can play a sound
bool SineWaveVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    //We could just return true in this case but our example illustrates how to use dynamic_cast to check the type of the sound class being passed in.
    //Dynamic casting is a type-safe way to perform downcasting (casting from a base class to a derived class) in C++. It ensures at runtime that the cast is valid and the object being cast is of the expected type. If the cast is not valid, it returns a nullptr
    return dynamic_cast<SineWaveSound*> (sound) != nullptr;
}

void SineWaveVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{

    currentAngle = 0.0;
    //level = velocity * 0.15;
    tailOff = 0.0;
    
    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();
    
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    
}

void SineWaveVoice::setLevel(float newLevel)
{
    level = newLevel;
}


//called by owning SynthAudioSource when we need this voice to stop
void SineWaveVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    
    if (allowTailOff)
    {
        if (tailOff == 0.0)
            tailOff = 1.0;
    }
    else
    {
        clearCurrentNote();
        angleDelta = 0.0;
    }
}

void SineWaveVoice::renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    //wave angle is not changing
    if (! juce::approximatelyEqual (angleDelta, 0.0))
    {
        
        /*
        DBG("----------------------");
        DBG("angleDelta: " << angleDelta);
        DBG("tailOff: " << tailOff);
        DBG("currentAngle: " << currentAngle);
        DBG("level: " << level);
        DBG("numSamples: " << numSamples);
        DBG("outputBuffer channels: " << outputBuffer.getNumChannels());
        */
        
        if (tailOff > 0.0)
        {
            while (--numSamples >= 0)
            {
                //use tail off to make note quieter
                auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);
                
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                 outputBuffer.addSample (i, startSample, currentSample);
                    
                currentAngle += angleDelta;
                ++startSample;
                
                //exponential decay envelope shape
                tailOff *= 0.99;
                
                //once the note is quiet enough, stop it
                if (tailOff <= 0.005)
                {
                    //reset the voice, so it can be reused!
                    clearCurrentNote();
                    
                    angleDelta = 0.0;
                    break;
                }
            }
        }
        else
        {
            //loop is used for the normal state of the voice, while the key is being held down.
            while (--numSamples >= 0)
            {
                
                //get sine wave amplitude for this sample
                auto currentSample = (float) (std::sin (currentAngle) * level);
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample (i, startSample, currentSample);
                
                //update the angle for the next sample
                currentAngle += angleDelta;
                ++startSample;
            }
        }
    }
}


void SineWaveVoice::pitchWheelMoved (int newPitchWheelValue) {};
void SineWaveVoice::controllerMoved (int controllerNumber, int newControllerValue){};
