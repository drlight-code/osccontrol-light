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

#include "LayoutHints.h"

#include "UIComponentFactory.h"

#include "ControlElementUI.h"

ControlElementUI::
ControlElementUI
(const CreateInfo & info,
 OSCSender & oscSender) :
    ControlElement(info, oscSender)
{
    buttonMute = UIComponentFactory::createControlButton ();
    buttonMute->setButtonText ("m");
    buttonMute->setColour (TextButton::buttonOnColourId, Colours::crimson);
    addAndMakeVisible (buttonMute.get ());

    buttonMute->getToggleStateValue().addListener (this);
    sendValue.addListener (this);
}

void
ControlElementUI::
resized ()
{
    auto area = getLocalBounds ();

    area = area.removeFromRight (LayoutHints::heightRow);
    area = area.withSizeKeepingCentre
        (LayoutHints::sizeButton, LayoutHints::sizeButton);

    buttonMute->setBounds (area);
}

void
ControlElementUI::
registerSendValue()
{
    sendValue.removeListener (this);
    sendValue.referTo (getSpecificSendValue ());
    sendValue.addListener (this);

    sendValue.setValue (createInfo.defaultValue);
}

void
ControlElementUI::
valueChanged
(Value & value)
{
    if(value.refersToSameSourceAs
        (buttonMute->getToggleStateValue ())) {

        auto toggleState = buttonMute->getToggleState ();
        if(createInfo.messageMute != "") {
            auto oscMessage =
                OSCMessage
                (String(createInfo.messageMute),
                    float(toggleState));
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

        switch (createInfo.type) {
        case ControlElement::Type::Float: {
            auto oscMessage =
                OSCMessage(
                    String(createInfo.message),
                    float(value));
            oscSender.send(oscMessage);
            break;
        }

        case ControlElement::Type::Int: {
            auto oscMessage =
                OSCMessage(
                    String(createInfo.message),
                    int(value));
            oscSender.send(oscMessage);
            break;
        }

        case ControlElement::Type::Toggle:
            auto oscMessage =
                OSCMessage(
                    String(createInfo.message),
                    float(value));
            oscSender.send(oscMessage);
            break;
        }
    }
}
