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
class PluginEditor  : public AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    PluginProcessor& processor;

    Slider mStereoFactorSlider;
    Slider mGainSlider;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment ;

    std::unique_ptr<SliderAttachment> mStereoFactorAttachment;
    std::unique_ptr<SliderAttachment> mGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
