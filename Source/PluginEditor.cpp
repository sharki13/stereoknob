/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoKnobAudioProcessorEditor::StereoKnobAudioProcessorEditor (StereoKnobAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    stereoKnobSlider.reset(new Slider("StereoKnobSlider"));
    addAndMakeVisible(stereoKnobSlider.get());
    stereoKnobSlider->setRange(-1, 1, 0.01);
    stereoKnobSlider->setValue(0);
    stereoKnobSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    stereoKnobSlider->setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    stereoKnobSlider->setTextValueSuffix(" Width");
    stereoKnobSlider->setBounds(getLocalBounds());
    stereoKnobSlider->addListener(this);

    addAndMakeVisible(stereoKnobSlider.get());
}

StereoKnobAudioProcessorEditor::~StereoKnobAudioProcessorEditor()
{
}

//==============================================================================
void StereoKnobAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void StereoKnobAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    
}

void StereoKnobAudioProcessorEditor::sliderValueChanged(Slider* /*slider*/)
{
}
