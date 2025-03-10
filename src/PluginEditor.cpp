/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "SequencerUI.h"
#include "SuffixedSlider.h"

//==============================================================================
TestpluginAudioProcessorEditor::TestpluginAudioProcessorEditor(
  TestpluginAudioProcessor &p)
  : AudioProcessorEditor(&p), audioProcessor(p) {
    // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
    setSize(350, 400);
  
    euclidComboBox.addItemList(audioProcessor.euclidStringOptions, 1);
    euclidComboBox.addListener(this);

    pSlider.addListener(this);
    nSlider.addListener(this);
    rSlider.addListener(this);
    attackSlider.addListener(this);
    decaySlider.addListener(this);
    sustainSlider.addListener(this);
    releaseSlider.addListener(this);
    noteLengthSlider.addListener(this);
    decibelSlider.addListener(this);

    juce::String comboBoxToolTip = "Click here for presets";
    }

    TestpluginAudioProcessorEditor::~TestpluginAudioProcessorEditor() {
      euclidComboBox.removeListener(this);
      
      pSlider.removeListener(this);
      nSlider.removeListener(this);
      rSlider.removeListener(this);

      attackSlider.removeListener(this);
      decaySlider.removeListener(this);
      sustainSlider.removeListener(this);
      releaseSlider.removeListener(this);

      decibelSlider.removeListener(this);
  
      noteLengthSlider.removeListener(this);
    }
    
    //==============================================================================
    void TestpluginAudioProcessorEditor::paint(juce::Graphics &g) {
      g.fillAll(juce::Colours::black);

      drawSliderAndLabel(gainSlider, gainLabel, "Gain", juce::Slider::TextBoxBelow);
      drawSliderAndLabel(nSlider, nLabel, "N", juce::Slider::TextBoxBelow);
      drawSliderAndLabel(pSlider, pLabel, "P", juce::Slider::TextBoxBelow);
      drawSliderAndLabel(rSlider, rLabel, "R", juce::Slider::TextBoxBelow);
      drawSliderAndLabel(attackSlider, attackLabel, "A", juce::Slider::TextBoxBelow);
      drawSliderAndLabel(decaySlider, decayLabel, "D", juce::Slider::TextBoxBelow);
      drawSliderAndLabel(sustainSlider, sustainLabel, "S", juce::Slider::TextBoxBelow);
      drawSliderAndLabel(releaseSlider, releaseLabel, "R", juce::Slider::TextBoxBelow);

      drawSliderAndLabel(noteLengthSlider, noteLengthLabel, "Note Length", juce::Slider::TextBoxBelow);

      drawSliderAndLabel(decibelSlider, decibelLabel, "Offbeat Gain", juce::Slider::TextBoxBelow);

      decibelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
      decibelSlider.setTextValueSuffix(" dB");

      euclidComboBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
      euclidComboBox.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::white);
      euclidComboBox.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::yellow);


      addAndMakeVisible(adsrLabel);
      addAndMakeVisible(gateLabel);
      adsrLabel.setText("Envelope", juce::dontSendNotification);
      adsrLabel.setFont(juce::Font(16.0f, juce::Font::bold));
      adsrLabel.setJustificationType(juce::Justification::centred);
      adsrLabel.setJustificationType(juce::Justification::centred);
      adsrLabel.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::white);
      adsrLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);

      gateLabel.setText("Gate Controls", juce::dontSendNotification);
      gateLabel.setFont(juce::Font(16.0f, juce::Font::bold));
      gateLabel.setJustificationType(juce::Justification::centred);
      gateLabel.setJustificationType(juce::Justification::centred);
      gateLabel.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::white);
      gateLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);

      nAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "n_size", nSlider);
      pAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "p_size", pSlider);
      rAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "r_size", rSlider);
      attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "attack_float", attackSlider);
      decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "decay_float", decaySlider);
      sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "sustain_float", sustainSlider);
      releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "release_float", releaseSlider);

      decibelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "decibels", decibelSlider);

      noteLengthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.getAPVTS(), "note_length", noteLengthSlider);
      comboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
          audioProcessor.getAPVTS(), "euclidOptions", euclidComboBox);

      addAndMakeVisible(euclidComboBox);

      auto bounds = getLocalBounds();
      int width = getWidth();
      int height = getHeight();

      g.setColour(juce::Colours::white);
      g.drawRect(0, 0, width, height, 5);
      

      sequencerUI.setEuclidRhythm(audioProcessor.getEuclidRhythm());
      sequencerUI.drawNoteBlocks(g);

      g.setColour(juce::Colours::white);
      g.drawLine(euclidBounds.getRight(), euclidBounds.getY(), euclidBounds.getRight(), euclidBounds.getBottom(), 5);
      g.drawLine(bounds.getRight() - 85, sequencerBounds.getBottom() - 3, bounds.getRight(), sequencerBounds.getBottom() -3, 5);
}

void TestpluginAudioProcessorEditor::drawSliderAndLabel(juce::Slider &slider, juce::Label &label, std::string labelTag, 
  juce::Slider::TextEntryBoxPosition textBoxEntryPosition) {
  slider.setLookAndFeel(&customLookAndFeel);
  slider.setTextBoxStyle(textBoxEntryPosition, false, 30, 25);
  slider.setTextBoxIsEditable(false);
  slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::white);

  label.setText(labelTag, juce::dontSendNotification);
  label.setFont(juce::Font(12.0f, juce::Font::bold));
  label.setJustificationType(juce::Justification::centred);


  addAndMakeVisible(label);
  addAndMakeVisible(slider);
}

void TestpluginAudioProcessorEditor::resized() {
  // Get the total bounds of the editor
  juce::Rectangle<int> bounds = getLocalBounds();
  int height = bounds.getHeight();
  int width = bounds.getWidth();

  // Define area for sequencer component
  auto sidebarBounds = bounds.removeFromTop(height - static_cast<float>(height)/3);
  sequencerBounds = sidebarBounds.removeFromRight(width - 80);
  sequencerUI.setBounds(sequencerBounds);

  euclidBounds = bounds.removeFromLeft(width/2);
  auto adsrBounds = bounds;

  int euclidWidth = euclidBounds.getWidth();
  int euclidHeight = euclidBounds.getHeight();
  int euclidX = euclidBounds.getX() + 2;
  int euclidY = euclidBounds.getY() - 10;

  int thirdX = static_cast<float>(euclidWidth) / 3;
  int halfY = static_cast<float>(euclidHeight) / 2;

  int labelOffset = 40;
  nSlider.setBounds(euclidX, euclidY + halfY, 50, 50);
  nLabel.setBounds(euclidX, euclidY + (halfY - labelOffset), 50, 60);
  pSlider.setBounds(euclidX + thirdX, euclidY + halfY, 50, 50);
  pLabel.setBounds(euclidX + thirdX, euclidY + (halfY - labelOffset), 50, 60);
  rSlider.setBounds(euclidX + (thirdX * 2) , euclidY + halfY, 50, 50);
  rLabel.setBounds(euclidX + (thirdX * 2) , euclidY + (halfY - labelOffset), 50, 60);

  euclidX = euclidBounds.getRight();
  int quarterX = static_cast<float>(euclidWidth) / 4;

  attackSlider.setBounds(euclidX, euclidY + halfY, 50, 50);
  attackLabel.setBounds(euclidX, euclidY + (halfY - labelOffset), 50, 60);
  decaySlider.setBounds(euclidX + quarterX, euclidY + halfY, 50, 50);
  decayLabel.setBounds(euclidX + quarterX, euclidY + (halfY - labelOffset), 50, 60);
  sustainSlider.setBounds(euclidX + (quarterX *2), euclidY + halfY, 50, 50);
  sustainLabel.setBounds(euclidX + (quarterX *2), euclidY + (halfY - labelOffset), 50, 60);
  releaseSlider.setBounds(euclidX + (quarterX * 3) , euclidY + halfY, 50, 50);
  releaseLabel.setBounds(euclidX + (quarterX * 3) , euclidY + (halfY - labelOffset), 50, 60);


  euclidComboBox.setBounds(sequencerBounds.getRight() - 80, sequencerBounds.getY(), 80, 20);
  adsrLabel.setBounds(adsrBounds.getCentreX() - 40, adsrBounds.getY() + 10, 80, 20);
  gateLabel.setBounds(euclidBounds.getCentreX() - 40, adsrBounds.getY() + 10, 80, 20);

  noteLengthSlider.setBounds(sequencerBounds.getRight() - 70, sidebarBounds.getY() + 75, 50, 50);
  noteLengthLabel.setBounds(sequencerBounds.getRight() - 70, sidebarBounds.getY() + 75 - labelOffset, 50, 60);

  decibelSlider.setBounds(sequencerBounds.getRight() - 70, sidebarBounds.getY() + 150, 50, 50);
  decibelLabel.setBounds(sequencerBounds.getRight() - 70, sidebarBounds.getY() + 150 - labelOffset, 50, 60);
}

