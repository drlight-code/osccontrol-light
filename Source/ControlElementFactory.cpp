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

#include "PluginProcessor.h"

#include "ControlElementKnob.h"
#include "ControlElementFactory.h"

ControlElementFactory::
ControlElementFactory
(OSCSender & oscSender,
 OscsendvstAudioProcessor & processor) :
    oscSender(oscSender),
    processor(processor)
{
}

std::unique_ptr<ControlElement>
ControlElementFactory::
createControlElement
(YAML::Node configElement, YAML::Node configInterface)
{
    ControlElement::CreateInfo info;

    auto name = configElement["name"].as<std::string>();
    auto type = configElement["type"].as<std::string>();

    if(!configInterface.IsNull()) {
        auto showNames = configInterface["show-names"];
        info.showNames =
            showNames.IsScalar() ? showNames.as<bool>() : false;
    }

    std::unique_ptr<ControlElement> product;
    if(type == "knob") {

        auto range = configElement["range"];
        info.range = std::make_pair(range[0].as<float>(),
                                    range[1].as<float>());
        info.defaultValue = configElement["default"].as<float>();

        info.name = configElement["name"].as<std::string>();
        info.message = configElement["message"].as<std::string>();

        auto messageMute = configElement["message-mute"];
        info.messageMute =
            messageMute.IsScalar() ? messageMute.as<std::string>() : "";

        product = std::make_unique<ControlElementKnob>(info, oscSender);
        product->registerSendValue();
    }

    return product;
}
