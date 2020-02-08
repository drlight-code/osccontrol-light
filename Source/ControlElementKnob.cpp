/*
  ==============================================================================

    ControlElementKnob.cpp
    Created: 8 Feb 2020 3:30:01pm
    Author:  bzk

  ==============================================================================
*/

#include <JuceHeader.h>

#include "LayoutHints.h"

#include "ControlElementKnob.h"

ControlElementKnob::
ControlElementKnob
(const CreateInfo & info,
 OSCSender & oscSender) :
    ControlElement(oscSender),
    message(info.message)
{
    knob.reset (new Slider("knob"));
    knob->setSliderStyle (Slider::RotaryVerticalDrag);
    knob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    knob->setRange (info.range.first, info.range.second, 0);
    knob->setValue(info.value, NotificationType::dontSendNotification);
    knob->addListener (this);
    addAndMakeVisible(knob.get());

    textEditor.reset (new TextEditor ("textEditor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (false);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setText (info.message);
    addAndMakeVisible (textEditor.get());
}

int
ControlElementKnob::
getNumberOfRows() const
{
    return 1;
}

void
ControlElementKnob::
sliderValueChanged
(Slider* slider)
{
    send();
}


void
ControlElementKnob::
send()
{
    if (!buttonMute->getToggleState()) {
        auto value = knob->getValue();
        auto message = textEditor->getText();

        auto oscMessage = OSCMessage(message, float(value));
        oscSender.send(oscMessage);
    }
}

void
ControlElementKnob::
resized()
{
    ControlElement::resized();

    auto area = getLocalBounds();
    auto areaKnob = area.removeFromLeft(LayoutHints::heightRow);
    areaKnob = areaKnob.withSizeKeepingCentre
        (LayoutHints::sizeKnob, LayoutHints::sizeKnob);
    knob->setBounds(areaKnob);

    area.removeFromRight(LayoutHints::heightRow);
    area.removeFromTop(LayoutHints::getTextBoxInset());
    area.removeFromBottom(LayoutHints::getTextBoxInset());
    textEditor->setBounds(area);
}
