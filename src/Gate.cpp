#include "Gate.h"

void Gate::prepareToPlay(juce::dsp::ProcessSpec spec, int period_length,
                         std::vector<int> rhythm) {
  // Set pre-processing member variables
  this->numChannels = spec.numChannels;
  this->period_length = period_length;
  this->rhythm = rhythm;
  
  // Set numChannels in member vectors
  noteState.resize(numChannels, 0);
  adsrVector.resize(numChannels, adsr);
  channelSampleIndex.resize(numChannels, 0);
  rhythmIndex.resize(numChannels, 0);
  rhythmCH.resize(numChannels);

  // Multichannel rhythm
  for (int ch = 0; ch < numChannels; ++ch) {
    rhythmCH[ch] = rhythm;  // Copy the original data into each channel
  }
};

// Set parameters for member ADSR object
void Gate::setADSRParameters(juce::ADSR::Parameters adsrParameters){
  for (int ch = 0; ch < numChannels; ++ch) {
    adsrVector[ch].setParameters(adsrParameters);
  }
}

void Gate::setGain(float gain){
  this->gain = gain;
}

// On parameter changes in pluginProcessor this is called
void Gate::updateGate(int period_length, std::vector<int> rhythm) {
  this->period_length = period_length;
  this->rhythm = rhythm;

  // Multichannel updated rhythm
  for (int ch = 0; ch < numChannels; ++ch) {
    rhythmCH[ch] = rhythm;
  }
};


void Gate::reset() {
    // Reset indices for all channels to 0
    channelSampleIndex.assign(numChannels, 0);
    rhythmIndex.assign(numChannels, 0);
    
    // Reset adsr note states to false
    noteState.assign(numChannels, false);
    
    // Reset adsr envelopes for all channels
    for (auto& adsr : adsrVector) {
        adsr.reset();
    }
}

void Gate::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {

  // Safety checks at the beginning of the function
  if (rhythm.empty() || rhythmCH.empty()) {
    return;  // Exit early if vectors are empty
  }
  if (period_length <= 0) {
    period_length = 1;  // Ensure period_length is valid
  }
  int bufferSize = buffer.getNumSamples();

  // Channelize algo
  for (auto channel = 0; channel < numChannels; ++channel) {

    //Get samples for the specified channel
    auto channelSamples = buffer.getWritePointer(channel);
    int i = 0;

    //Process for each sample[i] in this channel
    while (i < bufferSize) {

      // Safety check for vector bounds
      // Resets index if rhythmIndex is bigger than the max index of the rhythm
      if (rhythmIndex[channel] >= rhythm.size()) {
        rhythmIndex[channel] = 0;  // Reset if out of bounds
      }

      /*switch statement to get the correct on/off based on getting the current channel of rhythm and then the current index
      of that channel*/
      switch (rhythmCH[channel][rhythmIndex[channel]]) {

        case 1: {  // On state

          
          currentIndex = rhythmIndex[channel];

          const float gapDuration = 0.2f * period_length;  // 20% of period_length as gap
          const float onDuration = period_length - gapDuration;

          // Process active phase
          while (channelSampleIndex[channel] < onDuration && i < bufferSize) {
            
            // bool serves purpose of only executing noteOn once
            if (!noteState[channel]) {
              noteState[channel] = true;
              adsrVector[channel].noteOn();
          }
          float env = adsrVector[channel].getNextSample();
          channelSamples[i++] *= env; // Apply envelope to current sample
          channelSampleIndex[channel]++; // Increment to move to next sample in channel

          }

          // Add gap at end of on phase to add audible gap between two adjacent on phases
          // while statement to continue until either hits period length or buffer ends
          while (channelSampleIndex[channel] < period_length && i < bufferSize) {
                  if(noteState[channel] == true){
                    noteState[channel] = false;
                    adsrVector[channel].noteOff();
                  }
            float env = adsrVector[channel].getNextSample();
            channelSamples[i++] *= env;  // Mute audio
            channelSampleIndex[channel]++;
          }

          // Move to next rhythm step i.e. increment euclid index for this channel or wraparound to beginning of rhythm
          if (channelSampleIndex[channel] >= period_length) {
            rhythmIndex[channel] =
                (rhythmIndex[channel] + 1) % rhythm.size();
            channelSampleIndex[channel] = 0;
            adsrVector[channel].reset();
          }
          break;  // Break to prevent fall through
        }

        case 0: {  // Off state

          currentIndex = rhythmIndex[channel];

          
          const float gapDuration = 0.2f * period_length;  // 20% of period_length as gap
          const float onDuration = period_length - gapDuration;

          while (channelSampleIndex[channel] < onDuration && i < bufferSize) {
                  
            if (!noteState[channel]) {
                    noteState[channel] = true;
                    adsrVector[channel].noteOn();
                }
            //Channel samples(i) is in relation to buffer passed
            //Channel sample index is in relation to the period length
            float env = adsrVector[channel].getNextSample();
            channelSamples[i++] *= gain * env;  // Modulate by gain
            channelSampleIndex[channel]++;
          }
          
          while (channelSampleIndex[channel] < period_length && i < bufferSize) {
            if(noteState[channel] == true){
              noteState[channel] = false;
              adsrVector[channel].noteOff();
            }
          float env = adsrVector[channel].getNextSample();
          channelSamples[i++] *= env * gain;  // Mute audio
          channelSampleIndex[channel]++;
          }

          //Once period_length is hit increment/wraparound through rhythmIndex and reset channelSampleIndex to 0
          if (channelSampleIndex[channel] >= period_length) {
            rhythmIndex[channel] =
                (rhythmIndex[channel] + 1) % rhythm.size();
            channelSampleIndex[channel] = 0;
            adsrVector[channel].reset();
          }
          break;
        }
      }
    }
  }
};
