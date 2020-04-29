/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoKnobAudioProcessor::StereoKnobAudioProcessor()
     : AudioProcessor (BusesProperties().withInput("Input", AudioChannelSet::stereo(), true).withOutput ("Output", AudioChannelSet::stereo(), true))
{
    addParameter(gain = new AudioParameterFloat(
        "gain",
        "Gain",
        0.0f,
        1.0f,
        1.0f)
    );

    addParameter(stereoFactor = new AudioParameterFloat(
        "stereoFactor",
        "StereoFactor",
        0.0f,
        1.0f,
        0.5f)
    );
}

StereoKnobAudioProcessor::~StereoKnobAudioProcessor()
{
}

//==============================================================================
const String StereoKnobAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoKnobAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoKnobAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoKnobAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoKnobAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoKnobAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoKnobAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoKnobAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String StereoKnobAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void StereoKnobAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void StereoKnobAudioProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StereoKnobAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoKnobAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StereoKnobAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& /*midiMessages*/)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear extra outputs which are not used
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* leftChannelBuffer = buffer.getWritePointer(0);
    auto* rightChannelBuffer = buffer.getWritePointer(1);

    float midGain = 1;
    float sideGain = 1;

    if (*stereoFactor > 0)
    {
        midGain -= *stereoFactor;
    }
    else
    {
        sideGain += *stereoFactor;
    }

    for (int sampleNum = 0; sampleNum < buffer.getNumSamples(); ++sampleNum)
    {
        auto midSignal = (leftChannelBuffer[sampleNum] + rightChannelBuffer[sampleNum]) * midGain; // Mid = L + R (mono signal / all data)
        auto sideSignal = (leftChannelBuffer[sampleNum] - rightChannelBuffer[sampleNum]) * sideGain; // Side  = L - R (difference beetween L and R)

        leftChannelBuffer[sampleNum] = (midSignal + sideSignal) / 2; // recreation of L channel, L = (Mid + Side)/2
        rightChannelBuffer[sampleNum] = (midSignal - sideSignal) / 2; // recreation of R channel,  R = (Mid - Side)/2
    }
}

//==============================================================================
bool StereoKnobAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* StereoKnobAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void StereoKnobAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereoKnobAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoKnobAudioProcessor();
}
