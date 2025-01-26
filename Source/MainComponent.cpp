#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : synthAudioSource  (keyboardState),
      keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible (keyboardComponent);
    
    clearBtn.setButtonText(juce::String("Clear Drones"));
    clearBtn.onClick = [this] ()
    {
        synthAudioSource.clearNotesAndStop();
    };
    addAndMakeVisible(clearBtn);

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
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    // Make the button smaller but flexible
    juce::FlexItem clearBtnFlexItem = juce::FlexItem (clearBtn)
                        .withWidth(100)
                        .withHeight(100)
                        .withMargin(juce::FlexItem::Margin (10.0f)) // Adds a margin of 10px on all sides
                        .withAlignSelf(juce::FlexItem::AlignSelf::center); // Centers along the y-axis
    fb.items.add(clearBtnFlexItem);

    juce::FlexItem keyboardFlexItem =juce::FlexItem (keyboardComponent)
                        .withMinWidth(400.0)
                        .withHeight(100.0)
                        .withFlex(1.0)  // Keyboard takes up the remaining space
                        .withAlignSelf(juce::FlexItem::AlignSelf::center);
    // Make the keyboard component fill the remaining space
    fb.items.add(keyboardFlexItem);
    
    fb.performLayout (getLocalBounds());
    //keyboardComponent.setBounds(area);
    
}
