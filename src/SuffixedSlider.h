#pragma once

class SuffixedSlider : public juce::Slider {
    public:
        SuffixedSlider(const juce::String& suffix) : suffix(suffix) {}
    
        juce::String getTextFromValue(double value) override {
            // Convert the value to a string and append the suffix
            return juce::String(value, 2) + " " + suffix;
        }
    
    private:
        juce::String suffix; // The suffix to append to the value
    };