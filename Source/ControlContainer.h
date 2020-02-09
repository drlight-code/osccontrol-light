/*
  ==============================================================================

    ControlContainer.h
    Created: 8 Feb 2020 9:48:27pm
    Author:  bzk

  ==============================================================================
*/

#pragma once

#include <list>

#include <JuceHeader.h>

class ControlElement;
using ControlElementUniq = std::unique_ptr<ControlElement>;

class ControlContainer :
    public Component
{
public:
    void resized() override;
    // void paint(Graphics &g) override;

    std::list<ControlElementUniq> & getElementList();

private:
    std::list<ControlElementUniq> listControlElements;
};
