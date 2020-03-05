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

#include "ControlElementToggle.h"

ControlElementToggle::
ControlElementToggle
(const CreateInfo & createInfo,
 OSCSender & oscSender) :
    ControlElementUI(createInfo, oscSender)
{
    toggle = UIComponentFactory::createControlButton ();
    addAndMakeVisible(toggle.get());

    textEditor = UIComponentFactory::createControlTextEditor ();
    textEditor->setText (createInfo.name);
    addAndMakeVisible (textEditor.get());
}

int
ControlElementToggle::
getNumberOfRows() const
{
    return 1;
}

void
ControlElementToggle::
resized()
{
    ControlElementUI::resized ();

    auto area = getLocalBounds ();

    auto areaToggle = area.removeFromLeft (LayoutHints::heightRow);
    areaToggle = areaToggle.withSizeKeepingCentre
        (LayoutHints::sizeButton, LayoutHints::sizeButton);
    toggle->setBounds (areaToggle);

    area.removeFromRight (LayoutHints::heightRow);
    area.removeFromTop (LayoutHints::getTextBoxInset());
    area.removeFromBottom (LayoutHints::getTextBoxInset());
    textEditor->setBounds (area);
}

Value &
ControlElementToggle::
getSpecificSendValue()
{
    return toggle->getToggleStateValue();
}
