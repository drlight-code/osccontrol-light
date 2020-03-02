/*

  oscsend-light - An audio plugin that speaks OSC.
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

#include "PresetParser.h"

#include "LayoutHints.h"

#include "ControlContainer.h"

#include "ControlElementUI.h"
#include "ControlElementFactory.h"

#include "PresetPage.h"

PresetPage::
PresetPage ()
{
    container = std::make_unique<ControlContainer> ();
    container->setPaintingIsUnclipped (true);

    connected.addListener (this);
}

PresetPage::
~PresetPage ()
{
}

void
PresetPage::
loadFromFile
(File filePreset)
{
    PresetParser preset (filePreset);
    auto presetPath = filePreset.getFullPathName ();

    host = preset.getHost ();
    port = preset.getPort ();
    connected = true;

    container->getControlElements ().clear ();

    ControlElementFactory factory (oscSender);

    int accumulatedHeight = 0;
    for(auto control : preset.getControlElements ()) {

        auto createInfo = preset.getControlElementCreateInfo (control);
        auto element = factory.createControlElementUI (createInfo);

        accumulatedHeight +=
            element->getNumberOfRows () * LayoutHints::heightRow;

        container->addAndMakeVisible (element.get ());
        container->getControlElements ().push_back (std::move (element));
    }

    auto areaContainer = container->getBounds ();
    areaContainer.setHeight (accumulatedHeight);
    container->setBounds (areaContainer);
}

Value &
PresetPage::
getHostValue()
{
    return host;
}

Value &
PresetPage::
getPortValue()
{
    return port;
}

Value &
PresetPage::
getConnectedValue()
{
    return connected;
}

void
PresetPage::
valueChanged
(Value & value)
{
    if(value.getValue()) {
        connectOsc();
    }
    else {
        disconnectOsc();
    }
}

void
PresetPage::
connectOsc ()
{
    auto message = "connecting to "
        + host.toString () + ":" + port.toString();
    Logger::writeToLog(message);

    oscSender.connect(host.getValue (), port.getValue ());

    for (auto & control : container->getControlElements ()) {
        control->setEnabled(true);
    }
}

void
PresetPage::
disconnectOsc()
{
    for (auto & control : container->getControlElements ()) {
        control->setEnabled(false);
    }

    oscSender.disconnect();
}

Component *
PresetPage::
getContainerComponent ()
{
    return container.get();
}
