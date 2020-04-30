/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
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

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
const String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int /*index*/)
{
}

const String PluginProcessor::getProgramName (int /*index*/)
{
    return {};
}

void PluginProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PluginProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& /*midiMessages*/)
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

        // TODO Add output gain

        leftChannelBuffer[sampleNum] = ((midSignal + sideSignal) / 2); // recreation of L channel, L = (Mid + Side)/2
        rightChannelBuffer[sampleNum] = (midSignal - sideSignal) / 2; // recreation of R channel,  R = (Mid - Side)/2
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PluginProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
