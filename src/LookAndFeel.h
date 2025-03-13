#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();

                          void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                            float sliderPosProportional, float rotaryStartAngle,
                            float rotaryEndAngle, juce::Slider& slider) override;
                            void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label) override;
                            void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

                                
    void setColourPalette(std::vector<juce::Colour> colourPalette){
        this->colourPalette = colourPalette;
    };

    
    private:
    std::vector<juce::Colour> colourPalette;
    
    // Add private helper methods or members here if needed
};
