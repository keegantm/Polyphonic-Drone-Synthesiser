#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : synthAudioSource  (keyboardState),
      keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible (keyboardComponent);
    setAudioChannels (0, 2);

    setSize (600, 160);
    
    //need to grab focus of the timer after the app starts, so wait 400ms
    startTimer (400);
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    synthAudioSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    keyboardComponent.setBounds(area);
    
}
