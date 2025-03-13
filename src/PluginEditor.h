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
class TestpluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::ComboBox::Listener, public juce::Slider::Listener, public juce::Button::Listener {
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

  void buttonClicked(juce::Button* button) override
  {
    audioProcessor.updateSlider();
    repaint();
  }

  void setCurrentIndex(int currentIndex){
    this->currentIndex = currentIndex;
    repaint();
  }

  void createColourPalette(){
    juce::Colour defaultGrey = juce::Colour(130, 130, 130);;
    juce::Colour red = juce::Colour(209,17,65);
    juce::Colour green = juce::Colour(0,177,89);
    juce::Colour blue = juce::Colour(0,174,219);
    juce::Colour orange = juce::Colour(243,119,53);
    juce::Colour yellow = juce::Colours::yellow;
    juce::Colour white = juce::Colour(242,240,239);
    juce::Colour red2 = juce::Colour(140,12,44);
    colourPalette.push_back(defaultGrey);
    colourPalette.push_back(red);
    colourPalette.push_back(green);
    colourPalette.push_back(blue);
    colourPalette.push_back(orange);
    colourPalette.push_back(yellow);
    colourPalette.push_back(white);
    colourPalette.push_back(red2);
  }
  
  void drawSliderAndLabel(juce::Slider &slider, juce::Label &label, std::string labelTag, juce::Slider::TextEntryBoxPosition textBoxEntryPosition);
  
  
  private:

  std::vector<juce::Colour> colourPalette;

  int currentIndex;

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

  juce::Slider decibelSlider;

  juce::ToggleButton reverseToggle;
  juce::Label reverseLabel;

  juce::Slider noteLengthSlider;
  
  juce::Label gainLabel;
  juce::Label nLabel;
  juce::Label pLabel;
  juce::Label rLabel;
  juce::Label attackLabel;
  juce::Label decayLabel;
  juce::Label sustainLabel;
  juce::Label releaseLabel;

  juce::Label decibelLabel;

  juce::Label noteLengthLabel;

  juce::Label adsrLabel;
  juce::Label gateLabel;
  
  juce::ComboBox euclidComboBox;

  juce::Rectangle<int> sequencerBounds;
  juce::Rectangle<int> euclidBounds;
  juce::Rectangle<int> sidebarBounds;

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

  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverseAttachment;
  
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decibelAttachment;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noteLengthAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

  float gain = 0.0f;
  
  CustomLookAndFeel customLookAndFeel;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  TestpluginAudioProcessor &audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TestpluginAudioProcessorEditor)
};
