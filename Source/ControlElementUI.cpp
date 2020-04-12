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

    buttonMute->addListener (this);
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
buttonClicked
(Button * button)
{
    if (button == buttonMute.get()) {
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
}

void
ControlElementUI::
send()
{
    if (!buttonMute->getToggleState()) {
        ControlElement::send();
    }
}
