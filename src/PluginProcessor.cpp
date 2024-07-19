/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"

//==============================================================================
TestpluginAudioProcessor::TestpluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ),
      //Define our VALUE TREE paramters.
    parameters(*this, nullptr, "PARAMETERS", 
      {
        std::make_unique<juce::AudioParameterFloat>("gain","Gain:",   0.0f, 10.0f, .5f),
        std::make_unique<juce::AudioParameterChoice>("sequence","Sequence:", juce::StringArray("1/16", "1/8", "1/4", "1/2", "1"), 1),
        std::make_unique<juce::AudioParameterChoice>("glitchType","Glitch Type:", juce::StringArray("Reverse", "Wobble"), 0),
      }
#endif
    )
{
}

TestpluginAudioProcessor::~TestpluginAudioProcessor() {}

//==============================================================================
const juce::String TestpluginAudioProcessor::getName() const
{
  return JucePlugin_Name;
}

bool TestpluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool TestpluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool TestpluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double TestpluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int TestpluginAudioProcessor::getNumPrograms()
{
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not
            // really implementing programs.
}

int TestpluginAudioProcessor::getCurrentProgram() { return 0; }

void TestpluginAudioProcessor::setCurrentProgram(int index) {}

const juce::String TestpluginAudioProcessor::getProgramName(int index)
{
  return {};
}

void TestpluginAudioProcessor::changeProgramName(int index,
                                                 const juce::String &newName) {}

//==============================================================================
void TestpluginAudioProcessor::prepareToPlay(double sampleRate,
                                             int samplesPerBlock)
{
  // Use this method as the place to do any pre-playback
  // initialisation that you need..


  //Create processspec
  dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = samplesPerBlock;
  spec.numChannels = getTotalNumInputChannels();

  mSampleRate = sampleRate;

  gain.prepare(spec);
}

void TestpluginAudioProcessor::updateParameters(){
  //Gain(float)
  auto *atomicGain = parameters.getRawParameterValue("gain");
  mGain = atomicGain->load();
  gain.setGainLinear(mGain);

  //Glitch Type(int)
  juce::AudioParameterChoice* glitchParameter = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("glitchType"));
  mGlitchIndex = glitchParameter->getIndex();

  //Sequence(int)
  juce::AudioParameterChoice* sequenceParameter = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("sequence"));
  mSequenceIndex = sequenceParameter->getIndex();
}

void TestpluginAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestpluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void TestpluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                            juce::MidiBuffer &midiMessages)
{
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.


  updateParameters();

  
  //Set sequence sample limit
  calcSS(mSequenceIndex);



  for (int channel = 0; channel < totalNumInputChannels; ++channel)
  {
    float *channelData = buffer.getWritePointer(channel);
      // Select glitch type
  switch(mGlitchIndex){
    case 0: applyReverse(channelData, buffer); 
            break;
    case 1: applyGain(channelData, buffer); 
            break;
  }
  }
}

void TestpluginAudioProcessor::applyGain(float *channelData, juce::AudioBuffer<float> &buffer){
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
      mSampleCounter++;
      if(mSampleCounter<mSequenceSamples/4){
      channelData[sample] = gain.processSample(channelData[sample]);
      }
      else if(mSampleCounter>=mSequenceSamples){
        mSampleCounter = 0;
      }
    }
}

void TestpluginAudioProcessor::applyReverse(float *channelData, juce::AudioBuffer<float> &buffer){
  mGain = 100;
  }

//Calculate no. of samples needed for each sequence
//Should be called only when sequence is changed, can implement this later when implementing our own editor
void TestpluginAudioProcessor::calcSS(int sequenceIndex){
  //Retrieve index of StringArray
  int sequenceBV = (60/mBpm) * mSampleRate;

  switch(mSequenceIndex){
    //1/16
    case 0: mSequenceSamples = sequenceBV/4;
            break;
    //1/8
    case 1: mSequenceSamples = sequenceBV/2;
            break;
    //1/4
    case 2: mSequenceSamples = sequenceBV;
            break;
    //1/2
    case 3: mSequenceSamples = sequenceBV*2;
            break;
    //1 bar
    case 4: mSequenceSamples = sequenceBV*4;
            break;
    default:
            break;
    
  }
}

bool TestpluginAudioProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *TestpluginAudioProcessor::createEditor()
{
  return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void TestpluginAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void TestpluginAudioProcessor::setStateInformation(const void *data,
                                                   int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
  return new TestpluginAudioProcessor();
}
