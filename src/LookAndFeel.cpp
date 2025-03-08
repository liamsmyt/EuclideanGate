#include "LookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(juce::Slider::thumbColourId, juce::Colours::yellow);   // Thumb color
    setColour(juce::Slider::trackColourId, juce::Colours::white); // Track color
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

// Draw the thumb (pointer)
g.setColour(slider.findColour(juce::Slider::thumbColourId));
auto thumbLength = radius * 0.7f; // Length of the thumb
auto thumbWidth = radius * 0.2f;  // Thickness of the thumb
juce::Path thumb;
thumb.addRectangle(-thumbWidth * 0.5f, -thumbLength, thumbWidth, thumbLength);
thumb.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));
g.fillPath(thumb);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
    {
        // Get the current font from the label
        juce::Font font = label.getFont();

        // Set the new font size (e.g., 14.0f)
        font.setHeight(12.0f);

        // Apply the new font to the label
        label.setFont(font);

        // Call the base class implementation to draw the slider label
        LookAndFeel_V4::drawLabel(g, label);
    }


