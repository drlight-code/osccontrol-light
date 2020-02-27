/*

  oscsend-vst - An audio plugin that speaks OSC.
  Copyright (C) 2020 Patric Schmitz

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <JuceHeader.h>

#include "LayoutHints.h"

#include "ControlElementKnob.h"

ControlElementKnob::
ControlElementKnob
(const CreateInfo & createInfo,
 OSCSender & oscSender) :
    ControlElement(createInfo, oscSender)
{
    knob.reset (new Slider("knob"));
    knob->setSliderStyle (Slider::RotaryVerticalDrag);
    knob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    knob->setRange (createInfo.range.first, createInfo.range.second, 0);
    knob->setValue (createInfo.defaultValue,
        NotificationType::dontSendNotification);
    knob->setPaintingIsUnclipped (true);
    addAndMakeVisible (knob.get());

    textEditor.reset (new TextEditor ("textEditor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setScrollbarsShown (false);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    if(createInfo.showNames) {
        textEditor->setReadOnly (true);
        auto backgroundColour =
            textEditor->findColour(TextEditor::backgroundColourId);
        textEditor->setColour
            (TextEditor::outlineColourId,
            backgroundColour);
        textEditor->setText (createInfo.name);
    }
    else {
        textEditor->setReadOnly (false);
        textEditor->setText (createInfo.message);
    }
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

Value &
ControlElementKnob::
getSpecificSendValue()
{
    return knob->getValueObject();
}
