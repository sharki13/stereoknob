/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible(mStereoFactorSlider);
    mStereoFactorAttachment.reset(new SliderAttachment(processor.getValueTreeState(), processor.stereoFactorId, mStereoFactorSlider));
    mStereoFactorSlider.setSliderStyle(Slider::Rotary);

    addAndMakeVisible(mGainSlider);
    mGainAttachment.reset(new SliderAttachment(processor.getValueTreeState(), processor.gainId, mGainSlider));
    mGainSlider.setSliderStyle(Slider::Rotary);

    setSize(800, 600);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void PluginEditor::resized()
{
    auto box = getLocalBounds().reduced(20);
    box.removeFromBottom(40);

    const auto width = box.getWidth() / 3;
    mGainSlider.setBounds(box.removeFromLeft(width).reduced(10));
    mStereoFactorSlider.setBounds(box.removeFromLeft(width).reduced(10));
}