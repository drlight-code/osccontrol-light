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

#include "ControlElementUI.h"

ControlElementUI::
ControlElementUI
(const CreateInfo & info,
    OSCSender & oscSender) :
    ControlElement(info, oscSender)
{
    buttonMute.reset (new TextButton ("buttonMute"));
    buttonMute->setButtonText("m");
    buttonMute->setColour(TextButton::buttonOnColourId,
        Colours::crimson);
    buttonMute->setClickingTogglesState(true);
    buttonMute->setPaintingIsUnclipped(true);
    buttonMute->getToggleStateValue().addListener(this);
    addAndMakeVisible(buttonMute.get());

    sendValue.addListener(this);
}

void
ControlElementUI::
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
ControlElementUI::
registerSendValue()
{
    sendValue.removeListener(this);
    sendValue.referTo
        (getSpecificSendValue());
    sendValue.addListener(this);

    if (sendValue.getValue().isDouble() &&
        float(sendValue.getValue()) == 0.f) {
        send();
    }
}

void
ControlElementUI::
valueChanged
(Value & value)
{
    if(value == buttonMute->getToggleStateValue()) {
        auto toggleState = buttonMute->getToggleState();
        if(createInfo.messageMute != "") {
            auto oscMessage =
                OSCMessage(String(createInfo.messageMute), int(toggleState));
            oscSender.send(oscMessage);
        }

        if(!toggleState) {
            send();
        }
    }
    else if(value.refersToSameSourceAs(sendValue)) {
        send();
    }
}

void
ControlElementUI::
send()
{
    if (!buttonMute->getToggleState()) {
        auto value = sendValue.getValue();
        auto oscMessage =
            OSCMessage(String(createInfo.message), float(value));
        oscSender.send(oscMessage);
    }
}
