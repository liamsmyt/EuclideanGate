#pragma once

#include <JuceHeader.h>
#include <math.h>


class SequencerUI {
    
    public:
    void drawNoteBlocks(juce::Graphics&g);
    
    void setEuclidRhythm(std::vector<int> euclidRhythm){
        this->euclidRhythm = euclidRhythm;
    };

    void setBounds(juce::Rectangle<int> bounds);
    


    
    private:

    float width = 0;
    float height = 0;

    std::vector<int> euclidRhythm;




};