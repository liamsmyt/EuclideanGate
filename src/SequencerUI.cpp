#include "SequencerUI.h"

void SequencerUI::drawNoteBlocks(juce::Graphics& g) {
  int numCircles = euclidRhythm.size();
  // Circle = 360*, distance between inner / outer circle = diameter

  const float PI = 3.141592653589793238f;

  float diameter = static_cast<float>(width)/static_cast<float>(numCircles);
  if(numCircles < 7){
    diameter = static_cast<float>(width)/5;
  }
  float conversionConstant = PI / 180.0f;
  float scale = width/3.5;
  float increment = 360.0f/static_cast<float>(numCircles);
  g.setColour(juce::Colours::black);
  g.fillRect(0, 0, static_cast<int>(width), static_cast<int>(height));

  for(int i = 0; i<numCircles; i++){
    float angleRadians = increment * i * (PI / 180.0f);

    //X and Y coords for circles
    float lineX = scale * std::cos(angleRadians);
    float lineY = scale * std::sin(angleRadians);

    
    if( euclidRhythm[i] == 1){
      g.setColour(juce::Colours::yellow);
      g.fillEllipse(width/2 + lineX - diameter/2, 
        height/2 + lineY - diameter/2, 
        diameter, diameter);
      }

      if(i == currentIndex){
        g.setColour(juce::Colours::green);
        g.fillEllipse(width/2 + lineX - diameter/2, 
          height/2 + lineY - diameter/2, 
          diameter, diameter);
        }

      g.setColour(juce::Colours::white);

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

