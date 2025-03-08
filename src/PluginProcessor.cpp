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
      // Define our VALUE TREE paramters.
      parameters(*this, nullptr, "PARAMETERS",
                 {std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f,
                                                              1.0f, 0.5f),
                  std::make_unique<juce::AudioParameterBool>("reverse",
                                                             "Reverse", false),
                  std::make_unique<juce::AudioParameterFloat>(
                      "attack_float", "Attack", 0.0f, 1.0f, 0.25f),
                  std::make_unique<juce::AudioParameterFloat>(
                      "decay_float", "Decay", 0.0f, 1.0f, 0.25f),
                  std::make_unique<juce::AudioParameterFloat>(
                      "sustain_float", "Sustain", 0.0f, 1.0f, 0.25f),
                  std::make_unique<juce::AudioParameterFloat>(
                      "release_float", "Release", 0.0f, 1.0f, 0.25f),

                  std::make_unique<juce::AudioParameterInt>("n_size", "N Size",
                                                            1, 32, 8),
                  std::make_unique<juce::AudioParameterInt>("p_size", "P Size",
                                                            1, 32, 5),
                  std::make_unique<juce::AudioParameterInt>("r_size", "R Size",
                                                            0, 5, 1),
                  std::make_unique<juce::AudioParameterChoice>(
                      "note_length", "Note length", noteLengthOptions, 2),
                  std::make_unique<juce::AudioParameterChoice>(
                      "euclidOptions", "Rhythm", euclidStringOptions, 0)}) {
}
#endif

TestpluginAudioProcessor::~TestpluginAudioProcessor() {}

//==============================================================================
const juce::String TestpluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool TestpluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool TestpluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool TestpluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double TestpluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int TestpluginAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int TestpluginAudioProcessor::getCurrentProgram() { return 0; }

void TestpluginAudioProcessor::setCurrentProgram(int index) {}

const juce::String TestpluginAudioProcessor::getProgramName(int index) {
  return {};
}

void TestpluginAudioProcessor::changeProgramName(int index,
                                                 const juce::String &newName) {}

//==============================================================================

std::vector<int> TestpluginAudioProcessor::euclid(int p, int n, int r) {
  // Result vector
  std::vector<int> result(n, 0);

  // Validate input (only up to 32 steps supported in the original)
  if(n <= 0 || n >32){
    n=1;
  }
  if(p <= 0 || p >32){
    p=1;
  }

  int bucket = 0;

  // Generate the rhythm
  for (int i = 0; i < n; i++) {
    int pulse;
    int bitpos = (i + r) % n;

    bucket += p;
    pulse = bucket >= n;
    bucket %= n;

    // Set the bit in the vector instead of a uint32_t
    result[bitpos] = pulse;
  }

  return result;
}

void TestpluginAudioProcessor::prepareToPlay(double sampleRate,
                                             int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  spec.sampleRate = sampleRate;             // Set the sample rate
  spec.maximumBlockSize = samplesPerBlock;  // Set the maximum block size
  spec.numChannels =
      getTotalNumInputChannels();  // Set the number of audio channels

  this->numChannels = getTotalNumInputChannels();
  this->samplesPerBlock = samplesPerBlock;
  this->sampleRate = sampleRate;

  // Prepare effects objects
  gate.prepareToPlay(spec, period_length, euclidRhythm);
  adsr.setSampleRate(spec.sampleRate);
}

void TestpluginAudioProcessor::updateComboBox(){
  calculateNoteLength();

  int currentIndex = 1;

  auto *parameter = parameters.getParameter("euclidOptions");
  if (parameter != nullptr) {
    // Cast the parameter to AudioParameterChoice
    if (auto *choiceParameter =
            dynamic_cast<juce::AudioParameterChoice *>(parameter)) {
      // Get the current index of the selected choice
      currentIndex = choiceParameter->getIndex();
    }
  }

  setEuclidParameters(currentIndex);

  n_size = *parameters.getRawParameterValue("n_size");
  p_size = *parameters.getRawParameterValue("p_size");
  r_size = *parameters.getRawParameterValue("r_size");

  euclidRhythm = euclid(p_size, n_size, r_size);

  bool reverse = parameters.getRawParameterValue("reverse")->load() > 0.5f;
  
  if (reverse) {
    reversedEuclid = reverseEuclid(euclidRhythm);
    gate.updateGate(period_length, reversedEuclid);
  } else {
    gate.updateGate(period_length, euclidRhythm);
  }
}

void TestpluginAudioProcessor::updateSlider() {
  calculateNoteLength();
  n_size = *parameters.getRawParameterValue("n_size");
  p_size = *parameters.getRawParameterValue("p_size");
  r_size = *parameters.getRawParameterValue("r_size");

  euclidRhythm = euclid(p_size, n_size, r_size);

  bool reverse = parameters.getRawParameterValue("reverse")->load() > 0.5f;
  
  if (reverse) {
    reversedEuclid = reverseEuclid(euclidRhythm);
    gate.updateGate(period_length, reversedEuclid);
  } else {
    gate.updateGate(period_length, euclidRhythm);
  }

  attack_float = *parameters.getRawParameterValue("attack_float");
  sustain_float = *parameters.getRawParameterValue("decay_float");
  decay_float = *parameters.getRawParameterValue("sustain_float");
  release_float = *parameters.getRawParameterValue("release_float");

  gate.setADSRParameters(attack_float, decay_float, sustain_float, release_float);
}

void TestpluginAudioProcessor::calculateNoteLength() {
  juce::AudioParameterChoice* noteLengthParameter = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("note_length"));
  int note_length = noteLengthParameter->getIndex();
  switch (note_length) {
    case 0:
      note_fraction = 0.25f;
      break;
    case 1:
      note_fraction = 0.5f;
      break;
    case 2:
      note_fraction = 1.0f;
      break;
    case 3:
      note_fraction = 2.0f;
      break;
    case 4:
      note_fraction = 4.0f;
      break;
    case 5:
      note_fraction = 8.0f;
      break;
    default:
      break;
  }
  calcPeriod = ((spec.sampleRate * 60.0f) / 120.0f) * note_fraction;
  period_length = static_cast<int>(round(calcPeriod));
}

void TestpluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestpluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
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
                                            juce::MidiBuffer &midiMessages) {
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
  // updateParameters();
  gate.processBlock(buffer, midiMessages);
}

// This should be disengaged upon a slider being changed?
void TestpluginAudioProcessor::setEuclidParameters(int currentSelection) {
  // Takes in string int from combobox
  // Sets n_size, p_size and whether or not is reversed

  juce::String euclidPreset = euclidStringOptions[currentSelection];
  std::string stdEuclidPreset = euclidPreset.toStdString();

  // Check for reversal
  reverse = !(euclidPreset.contains("not"));

  // Process E(X, X) and set n_size and p_size accordingly
  //  Find index of (
  //  Use this index + 1 and index + 4 to extract n and p values

  std::smatch matches;
  std::regex pRegex(R"(\((\d+),)");
  std::regex nRegex(R"(\s(\d+))");

  regex_search(stdEuclidPreset, matches, pRegex);
  std::string stringp_size = matches.str(1);
  regex_search(stdEuclidPreset, matches, nRegex);
  std::string stringn_size = matches.str(1);

  int n_size = std::stoi(stringn_size);
  int p_size = std::stoi(stringp_size);



  // Get the parameters
  auto *nsize = parameters.getParameter("n_size");
  auto *psize = parameters.getParameter("p_size");

  if (nsize != nullptr && psize != nullptr) {
    // Convert raw values to normalized values
    float n_normalized = nsize->convertTo0to1(n_size);
    float p_normalized = psize->convertTo0to1(p_size);

    // Set the normalized values
    nsize->setValueNotifyingHost(n_normalized);
    psize->setValueNotifyingHost(p_normalized);
  }
}



bool TestpluginAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *TestpluginAudioProcessor::createEditor() {
  // return new juce::GenericAudioProcessorEditor(*this);
  return new TestpluginAudioProcessorEditor(*this);
}

juce::StringArray TestpluginAudioProcessor::addEuclidStringOptions() {
  juce::StringArray rhythms;

  rhythms.add("Classical, Jazz, Persian, E(2, 5), Not Reversed");
  rhythms.add("Bulgarian folk, E(3, 7), Not Reversed");
  rhythms.add("Turkey, E(4, 9), Not Reversed");
  rhythms.add("Classical_2, E(5, 11), Not Reversed");
  rhythms.add("Brazilian necklace, E(5, 16), Not Reversed");
  rhythms.add("West Africa, Latin America, E(2, 3), Reversed");
  rhythms.add("Trinidad, Persia, E(3, 4), Reversed");
  rhythms.add("Rumanian/Persian necklace, E(3, 5), Reversed");
  rhythms.add("West Africa, E(3, 8), Reversed");
  rhythms.add("Bulgaria, E(4, 7), Reversed");
  rhythms.add("Frank Zappa, E(4, 11), Reversed");
  rhythms.add("Arab_1, E(5, 6), Reversed");
  rhythms.add("Arab_2, E(5, 7), Reversed");
  rhythms.add("Arab rhythm, South African/Rumanian Necklace, E(5, 9), Reversed");
  rhythms.add("South Africa, E(5, 12), Reversed");
  rhythms.add("Tuareg rhythm of Libya, E(7, 8), Reversed");
  rhythms.add("Brazilian necklace, E(7, 16), Reversed");
  rhythms.add("Central Africa, E(11, 24), Reversed");

  return rhythms;
}

std::vector<int> TestpluginAudioProcessor::reverseEuclid(
    std::vector<int> inputVector) {
  std::vector<int> rhythm = inputVector;
  int length = rhythm.size();
  for (int i = 0; i < length; i++) {
    switch (rhythm[i]) {
      case 0:
        rhythm[i] = 1;
        break;

      case 1:
        rhythm[i] = 0;
        break;

      default:
        break;
    }
  }

  return rhythm;
}

//==============================================================================
void TestpluginAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void TestpluginAudioProcessor::setStateInformation(const void *data,
                                                   int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new TestpluginAudioProcessor();
}
