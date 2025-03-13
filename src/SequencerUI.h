#pragma once

#include <JuceHeader.h>
#include <math.h>


class SequencerUI {
    
    public:
    void drawNoteBlocks(juce::Graphics&g);
    
    void setEuclidRhythm(std::vector<int> euclidRhythm){
        this->euclidRhythm = euclidRhythm;
    };

    void setCurrentIndex(int currentIndex){
        this->currentIndex = currentIndex;
    };

    void setColourPalette(std::vector<juce::Colour> colourPalette){
        this->colourPalette = colourPalette;
    };

    void setBounds(juce::Rectangle<int> bounds);
    


    
    private:

    std::vector<juce::Colour> colourPalette;

    float width = 0;
    float height = 0;
    int currentIndex = 0;

    std::vector<int> euclidRhythm;




};