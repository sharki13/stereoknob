/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class StereoKnobAudioProcessorEditor  : public AudioProcessorEditor,
    private Slider::Listener
{
public:
    StereoKnobAudioProcessorEditor (StereoKnobAudioProcessor&);
    ~StereoKnobAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void sliderValueChanged(Slider* slider) override;

    StereoKnobAudioProcessor& processor;
    std::unique_ptr<Slider> stereoKnobSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoKnobAudioProcessorEditor)
};
