#pragma once
#include <JuceHeader.h>

//Current bugs
// - Does not work until slider is moved but also is not always fixed when slider is moved - Fixed due to 
// - Clipping - Is fixed by envelope


class Gate {
    public:

    void prepareToPlay(juce::dsp::ProcessSpec spec, int period_length, std::vector<int> rhythm);

    void updateGate(int period_length, std::vector<int> rhythm);

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

    void setADSRParameters(float attack, float decay, float sustain, float release);
    void setGain(float gain);

    void reset();

    int getCurrentIndex(){
        return currentIndex;
    }

    private:

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParameters;
    std::vector<juce::ADSR> adsrVector;

    std::vector<bool> noteState;

    float attack = 0.0f;
    float decay = 0.0f;
    float sustain = 0.0f;
    float release = 0.0f;

    int currentIndex = 0;
    
    // Nested vector of rhythms, number of channels determines vertical size
    std::vector<std::vector<int>> rhythmCH;
    std::vector<int> rhythm;
    
    // i.e. where index are we at in terms of euclidRhythm e.g. {0, 0, 1, 2} 1 is at = index 2
    std::vector<int> rhythmIndex;
    //sampleIndex
    std::vector<int> channelSampleIndex;

    int period_length = 0;
    int numChannels = 0;

    float gain = 0;


};