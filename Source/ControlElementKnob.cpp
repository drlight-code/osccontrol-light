/*

  osccontrol-light - An audio plugin that speaks OSC.
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
#include "UIComponentFactory.h"

#include "ControlElementKnob.h"

ControlElementKnob::
ControlElementKnob
(const CreateInfo & createInfo,
 OSCSender & oscSender) :
    ControlElementUI(createInfo, oscSender)
{
    knob = UIComponentFactory::createControlKnob ();

    auto increment =
        createInfo.type == ControlElement::Type::Int ? 1 : 0;
    knob->setRange (createInfo.range.first,
        createInfo.range.second, increment);
    addAndMakeVisible (knob.get());

    textEditor = UIComponentFactory::createControlTextEditor ();
    textEditor->setText (createInfo.name);
    addAndMakeVisible (textEditor.get());

    knob->setValue (createInfo.defaultValue);
    knob->addListener (this);
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
    ControlElementUI::resized();

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

void
ControlElementKnob::
sliderValueChanged
(Slider * slider)
{
    sendValue.setValue(slider->getValue());
}
