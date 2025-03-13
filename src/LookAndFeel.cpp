#include "LookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(juce::Slider::thumbColourId, juce::Colours::yellow);   // Thumb color
    setColour(juce::Slider::trackColourId, juce::Colours::black); // Track color
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


    void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
// Get the bounds of the button and convert to float for smoother drawing
auto bounds = button.getLocalBounds().toFloat();

// Define the toggle switch dimensions
const float cornerRadius = 10.0f; // Rounded corners for the toggle track
const float thumbSize = bounds.getHeight() * 0.8f; // Size of the sliding thumb
const float thumbMargin = (bounds.getHeight() - thumbSize) / 2.0f; // Margin for the thumb

// Colors
auto trackColour = juce::Colours::black; // Base color for the track
auto thumbColour = button.getToggleState() ? colourPalette[5] : colourPalette[0]; // Thumb color

// Draw the toggle track (background)
g.setColour(trackColour);
g.fillRoundedRectangle(bounds, cornerRadius);

// Draw the sliding thumb
auto thumbBounds = bounds.withWidth(thumbSize).withHeight(thumbSize)
.withX(button.getToggleState() ? bounds.getRight() - thumbSize - thumbMargin : bounds.getX() + thumbMargin)
.withY(bounds.getY() + thumbMargin);

g.setColour(thumbColour);
g.fillEllipse(thumbBounds); // Circular thumb

// Optional: Add a subtle outline for the thumb
g.setColour(juce::Colours::black.withAlpha(0.5f));
g.drawEllipse(thumbBounds.expanded(1.0f), 1.0f);

// Draw the button text (optional, can be removed for a cleaner look)
g.setColour(button.getToggleState() ? colourPalette[5] : colourPalette[0]);
g.drawText(button.getButtonText(), bounds, juce::Justification::centred, false);
}


