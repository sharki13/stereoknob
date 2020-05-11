/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const String PluginProcessor::stereoFactorId ("stereoFactor");
const String PluginProcessor::gainId ("gain");

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties().withInput("Input", AudioChannelSet::stereo(), true).withOutput ("Output", AudioChannelSet::stereo(), true)),
       parameters(*this, nullptr, "PARAMETERS",
           {
               std::make_unique<AudioParameterFloat> (stereoFactorId,
                                                      "StereoFactor",
                                                      NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
                                                      0.0f, "%",
                                                      AudioProcessorParameter::genericParameter),
              std::make_unique<AudioParameterFloat>(gainId,
                                                    "Gain",
                                                    NormalisableRange<float>(-20.0f, 20.0f, 0.1f),
                                                    0.0f, "dB",
                                                    AudioProcessorParameter::genericParameter,
                                                    [](float v, int) { return String(v, 1) + " dB"; },
                                                    [](const String& t) { return t.dropLastCharacters(3).getFloatValue(); })
           })
{
    gainParameter = parameters.getRawParameterValue("gain");
    stereoFactorParameter = parameters.getRawParameterValue("stereoFactor");
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
    return false;
}

bool PluginProcessor::producesMidi() const
{
    return false;
}

bool PluginProcessor::isMidiEffect() const
{
    return false;
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

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    bool isSupportedLayout = true;

    if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        isSupportedLayout = false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        isSupportedLayout = false;

    return isSupportedLayout;
}

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

    auto stereoFactor = *stereoFactorParameter/100;

    auto gain = Decibels::decibelsToGain(static_cast<float>(*gainParameter));

    if (stereoFactor > 0)
    {
        midGain -= stereoFactor;
    }
    else
    {
        sideGain += stereoFactor;
    }

    for (int sampleNum = 0; sampleNum < buffer.getNumSamples(); ++sampleNum)
    {
        auto midSignal = (leftChannelBuffer[sampleNum] + rightChannelBuffer[sampleNum]) * midGain; // Mid = L + R (mono signal / all data)
        auto sideSignal = (leftChannelBuffer[sampleNum] - rightChannelBuffer[sampleNum]) * sideGain; // Side  = L - R (difference beetween L and R)

        leftChannelBuffer[sampleNum] = ((midSignal + sideSignal) / 2) * gain; // recreation of L channel, L = (Mid + Side)/2
        rightChannelBuffer[sampleNum] = ((midSignal - sideSignal) / 2) * gain; // recreation of R channel,  R = (Mid - Side)/2
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState& PluginProcessor::getValueTreeState()
{
    return parameters;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
