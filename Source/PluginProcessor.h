/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class PluginProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int) override;
    const String getProgramName (int) override;
    void changeProgramName (int, const String&) override;

    //==============================================================================
    void getStateInformation (MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)

    std::atomic<float>* gainParameter;
    std::atomic<float>* stereoFactorParameter;
    std::atomic<float>* testGainLinParameter;
    std::atomic<float>* testGainLogParameter;

    AudioProcessorValueTreeState parameters;
};
