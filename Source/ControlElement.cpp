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

#include "LayoutHints.h"

#include "ControlElement.h"

ControlElement::
ControlElement
(OSCSender & oscSender) :
    oscSender(oscSender)
{
    buttonMute.reset (new TextButton ("buttonMute"));
    buttonMute->setButtonText("m");
    buttonMute->setColour(TextButton::buttonOnColourId,
                          Colours::crimson);
    buttonMute->setClickingTogglesState(true);
    buttonMute->addListener(this);
    buttonMute->setPaintingIsUnclipped(true);
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
    auto toggleState = buttonMute->getToggleState();
    if(messageMute != "") {
        auto oscMessage = OSCMessage(String(messageMute),
                                     int(toggleState));
        oscSender.send(oscMessage);
    }

    if(!toggleState) {
        send();
    }
}
