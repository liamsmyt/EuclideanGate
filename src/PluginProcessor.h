/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 */
class TestpluginAudioProcessor : public juce::AudioProcessor {
 public:
  //==============================================================================
  TestpluginAudioProcessor();
  ~TestpluginAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  juce::AudioProcessorValueTreeState parameters; 


 private:
  void updateParameters();

  //effects
  void applyGain(float *channeldata, juce::AudioBuffer<float> &buffer);
  void applyReverse(float *channeldata, juce::AudioBuffer<float> &buffer);

  void calcSS(int sequenceIndex);
  dsp::Gain<float> gain;
  juce::AudioPlayHead::PositionInfo positionInfo;

  //Member variables
  float mGain = 0;
  double mBpm = 120;
  int mSampleRate = 0;
  int mSequenceSamples = 44100;
  int mSamplesUsed = 0;
  int mSampleCounter = 0;
  int mGlitchIndex = 0;
  int mSequenceIndex = 0;

//==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TestpluginAudioProcessor)
};
