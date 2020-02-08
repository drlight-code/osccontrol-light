/*
  ==============================================================================

    ControlElement.cpp
    Created: 8 Feb 2020 3:28:52pm
    Author:  bzk

  ==============================================================================
*/

#include "LayoutHints.h"

#include "ControlElement.h"

ControlElement::
ControlElement
(OSCSender & oscSender) :
    oscSender(oscSender)
{
    // create mute button
    buttonMute.reset (new TextButton ("buttonMute"));
    buttonMute->setButtonText("m");
    buttonMute->setColour(TextButton::buttonOnColourId,
                          Colours::crimson);
    buttonMute->setClickingTogglesState(true);
    buttonMute->addListener(this);
    addAndMakeVisible(buttonMute.get());
}

void
ControlElement::
resized()
{
    auto area = getLocalBounds();

    auto gap = LayoutHints::sizeGap;
    auto buttonSize = LayoutHints::heightRow - 2*gap;

    area.removeFromRight(gap);
    area.removeFromTop(gap);
    area.setHeight(buttonSize);

    buttonMute->setBounds(area.removeFromRight(buttonSize));
}

void
ControlElement::
buttonClicked
(Button* button)
{
}
