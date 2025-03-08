/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "SequencerUI.h"


//==============================================================================
/**
 */
class TestpluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::ComboBox::Listener, public juce::Slider::Listener {
 public:
  TestpluginAudioProcessorEditor(TestpluginAudioProcessor &);
  ~TestpluginAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

  void comboBoxChanged(juce::ComboBox* comboBox) override
  {
    audioProcessor.updateComboBox();
    repaint();
  } 

  void sliderValueChanged(juce::Slider* slider) override
  {
    audioProcessor.updateSlider();
    repaint();
  }
  void drawSliderAndLabel(juce::Slider &slider, juce::Label &label, std::string labelTag, juce::Slider::TextEntryBoxPosition textBoxEntryPosition);
  
  
  private:

  std::vector<int> euclidRhythm;
  
  // Declare UI elements
  /*
    Need sliders for npr algo, note length, offnote Gain, adsr

    ADSR sliders should limit themselves based on note length
  */
  juce::Slider gainSlider;
  juce::Slider nSlider;
  juce::Slider pSlider;
  juce::Slider rSlider;

  juce::Slider attackSlider;
  juce::Slider decaySlider;
  juce::Slider sustainSlider;
  juce::Slider releaseSlider;

  juce::Slider noteLengthSlider;
  
  juce::Label gainLabel;
  juce::Label nLabel;
  juce::Label pLabel;
  juce::Label rLabel;
  juce::Label attackLabel;
  juce::Label decayLabel;
  juce::Label sustainLabel;
  juce::Label releaseLabel;

  juce::Label noteLengthLabel;

  juce::Label adsrLabel;
  juce::Label gateLabel;
  
  juce::ComboBox euclidComboBox;

  juce::Rectangle<int> sequencerBounds;
  juce::Rectangle<int> euclidBounds;

  SequencerUI sequencerUI;
  
  // Declare attachments
  std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachment;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> nAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rAttachment;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noteLengthAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

  float gain = 0.0f;
  
  CustomLookAndFeel customLookAndFeel;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  TestpluginAudioProcessor &audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TestpluginAudioProcessorEditor)
};
