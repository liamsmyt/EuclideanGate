#include "LookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(juce::Slider::thumbColourId, juce::Colours::yellow);   // Thumb colour
    setColour(juce::Slider::trackColourId, juce::Colours::black); // Track colour
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle,
    float rotaryEndAngle, juce::Slider& slider)
{
// Define the bounds of the rotary slider
auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
auto centerX = bounds.getCentreX();
auto centerY = bounds.getCentreY();
auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

// Draw the background circle
g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
g.fillEllipse(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f);

// Draw the track (arc)
g.setColour(slider.findColour(juce::Slider::trackColourId));
auto trackWidth = radius * 0.1f; // Thickness of the track
juce::Path track;
track.addCentredArc(centerX, centerY, radius - trackWidth * 0.5f, radius - trackWidth * 0.5f,
0.0f, rotaryStartAngle, rotaryEndAngle, true);
g.strokePath(track, juce::PathStrokeType(trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

// Draw the thumb
g.setColour(slider.findColour(juce::Slider::thumbColourId));
auto thumbLength = radius * 0.7f; // Length the thumb
auto thumbWidth = radius * 0.2f;  // Thickness the thumb
juce::Path thumb;
thumb.addRectangle(-thumbWidth * 0.5f, -thumbLength, thumbWidth, thumbLength);
thumb.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));
g.fillPath(thumb);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
    {
        juce::Font font = label.getFont();
        font.setHeight(12.0f);
        label.setFont(font);
        LookAndFeel_V4::drawLabel(g, label);
    }


    void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
auto bounds = button.getLocalBounds().toFloat();

// Define dimensions
const float cornerRadius = 10.0f; // Rounded corners
const float thumbSize = bounds.getHeight() * 0.8f; // Size of sliding thumb
const float thumbMargin = (bounds.getHeight() - thumbSize) / 2.0f; // Margin for thumb

// Colour options
auto trackColour = juce::Colours::black; // Base color for track
auto thumbColour = button.getToggleState() ? colourPalette[5] : colourPalette[0]; // Thumb color

// Draw the background
g.setColour(trackColour);
g.fillRoundedRectangle(bounds, cornerRadius);

// Draw the thumb
auto thumbBounds = bounds.withWidth(thumbSize).withHeight(thumbSize)
.withX(button.getToggleState() ? bounds.getRight() - thumbSize - thumbMargin : bounds.getX() + thumbMargin)
.withY(bounds.getY() + thumbMargin);

g.setColour(thumbColour);
g.fillEllipse(thumbBounds);
}


