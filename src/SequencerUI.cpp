#include "SequencerUI.h"

void SequencerUI::drawNoteBlocks(juce::Graphics& g) {
  // Circle = 360*, distance between inner / outer circle = diameter

  const float PI = 3.141592653589793238f;

  // float diameterDifference = outerDiameter - innerDiameter;
  float diameter = static_cast<float>(width)/8.0f;
  float conversionConstant = PI / 180.0f;
  float scale = width/4; // Scaling factor to make the line visible
  float increment = 360.0f/static_cast<float>(euclidRhythm.size());

  for(int i = 0; i<euclidRhythm.size(); i++){
    float angleRadians = increment * i * (PI / 180.0f);

    //X and Y coords for circles
    float lineX = scale * std::cos(angleRadians);
    float lineY = scale * std::sin(angleRadians);

    g.setColour(juce::Colours::white);


    g.drawEllipse(width/2 + lineX - diameter/2, 
      height/2 + lineY - diameter/2, 
      diameter, diameter, 5);
  }

  g.drawRect(0, 0, static_cast<int>(width), static_cast<int>(height), 5);

}

void SequencerUI::setBounds(juce::Rectangle<int> bounds){
  this->width = static_cast<float>(bounds.getWidth());
  this->height = static_cast<float>(bounds.getHeight());
}
