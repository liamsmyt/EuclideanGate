#include "SequencerUI.h"

void SequencerUI::drawNoteBlocks(juce::Graphics& g) {
  int numCircles = euclidRhythm.size();
  // Circle = 360*, distance between inner / outer circle = diameter

  const float PI = 3.141592653589793238f;

  float diameter = static_cast<float>(width)/static_cast<float>(numCircles);
  if(numCircles < 7){
    diameter = static_cast<float>(width)/6;
    scale = width/3.5;
  }
  else if (numCircles < 16){
    scale = width/3;
  }
  else{
    scale = width/2.5;
  }
  float conversionConstant = PI / 180.0f;
  float increment = 360.0f/static_cast<float>(numCircles);
  g.setColour(colourPalette[6]);
  g.fillRect(0, 0, static_cast<int>(width), static_cast<int>(height));

  for(int i = 0; i<numCircles; i++){
    float angleRadians = increment * i * (PI / 180.0f);

    //X and Y coords for circles
    float lineX = scale * std::cos(angleRadians);
    float lineY = scale * std::sin(angleRadians);

    
    if( euclidRhythm[i] == 1){

      // Define the colors for the gradient
      juce::Colour startColour = colourPalette[7];    // Starting color
      juce::Colour endColour = colourPalette[1];     // Ending color
      
      // Define the ellipse's bounds
      float ellipseX = width / 2 + lineX - diameter / 2;
      float ellipseY = height / 2 + lineY - diameter / 2;
      juce::Rectangle<float> ellipseBounds(ellipseX, ellipseY, diameter, diameter);
      
      // Create a radial gradient
      juce::ColourGradient gradient(
          startColour, ellipseBounds.getCentre(), // Start color and center point
          endColour, ellipseBounds.getTopLeft(),  // End color and radial point
          true                                   // Radial gradient
      );
      
      // Set the gradient as the fill for the graphics context
      g.setGradientFill(gradient);
      
      // Fill the ellipse with the gradient
      g.fillEllipse(ellipseBounds);
      }

      if(i == currentIndex){
            // Define the colors for the gradient
    juce::Colour startColour = colourPalette[4];    // Starting color
    juce::Colour endColour = colourPalette[5];     // Ending color

    // Define the ellipse's bounds
    float ellipseX = width / 2 + lineX - diameter / 2;
    float ellipseY = height / 2 + lineY - diameter / 2;
    juce::Rectangle<float> ellipseBounds(ellipseX, ellipseY, diameter, diameter);

    // Create a radial gradient
    juce::ColourGradient gradient(
        startColour, ellipseBounds.getCentre(), // Start color and center point
        endColour, ellipseBounds.getTopLeft(),  // End color and radial point
        true                                   // Radial gradient
    );

    // Set the gradient as the fill for the graphics context
    g.setGradientFill(gradient);

    // Fill the ellipse with the gradient
    g.fillEllipse(ellipseBounds);
        }

      g.setColour(colourPalette[0]);

    g.drawEllipse(width/2 + lineX - diameter/2, 
      height/2 + lineY - diameter/2, 
      diameter, diameter, diameter/8);

  }
  g.drawRect(0, 0, static_cast<int>(width), static_cast<int>(height), 5);
}

void SequencerUI::setBounds(juce::Rectangle<int> bounds){
  this->width = static_cast<float>(bounds.getWidth());
  this->height = static_cast<float>(bounds.getHeight());
}

