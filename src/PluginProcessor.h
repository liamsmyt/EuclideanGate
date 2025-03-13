/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Gate.h"
#include "SequencerUI.h"
#include <regex>

class TestpluginAudioProcessorEditor;

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

  void setEditor(TestpluginAudioProcessorEditor* newEditor)
    {
        editor = newEditor;
    }

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
  
  juce::AudioProcessorValueTreeState &getAPVTS() { return parameters; };

  std::vector<int> getEuclidRhythm() { return euclidRhythm; };
  
  juce::StringArray addEuclidStringOptions();
  
  juce::StringArray euclidStringOptions = addEuclidStringOptions();
  
  void updateSlider();
  void updateComboBox();
  
  private:
  TestpluginAudioProcessorEditor* editor;
  void scaleADSRParameters(float attack, float decay, float sustain, float release);
  float dbToGain(float decibels);

  juce::ADSR::Parameters adsrParameters;

  Gate gate;

  juce::StringArray noteLengthOptions = { "1/16", "1/8", "1/4", "1/2", "1 Bar", "2 Bar"};

  int currentIndex;
  juce::ADSR adsr;
  void setEuclidParameters(int currentSelection);

  juce::AudioProcessorValueTreeState parameters;
  void calculateNoteLength();
  std::vector<int> euclid(int p_size, int n_size, int r_size);

  void reverseEuclid();

  std::vector<int> reversedEuclid;


  bool reverse = 0;


  int n_size = 4;
  int p_size = 4;
  int r_size = 4;

  std::vector<int> euclidRhythm;

  float note_fraction = 0.0f;

  //Envelope Variables
  float attack_float = 0.0f;
  float decay_float = 0.0f;
  float sustain_float = 0.0f;
  float release_float = 0.0f;

  //Gate Variables
  int period_length = 0;

  float calcPeriod = 0.0f;


  // Basic variables
  int sampleRate = 0;
  int samplesPerBlock = 0;
  int numChannels = 0;
  juce::dsp::ProcessSpec spec;

  int resetIncrement = 0;

  float gain = 0.0f;

  bool isBufferSilent(const juce::AudioBuffer<float>& buffer, float threshold = 1.0e-6f)
{
    // Check the magnitude of the buffer
    float magnitude = buffer.getMagnitude(0, buffer.getNumSamples());

    // If the magnitude is below the threshold, the buffer is considered silent
    return magnitude < threshold;
}

  juce::AudioPlayHead::PositionInfo posInfo;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TestpluginAudioProcessor)
};
