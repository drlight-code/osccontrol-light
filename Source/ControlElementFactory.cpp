/*
  ==============================================================================

    ControlElementFactory.cpp
    Created: 8 Feb 2020 3:01:25pm
    Author:  bzk

  ==============================================================================
*/

#include <JuceHeader.h>

#include "ControlElementFactory.h"
#include "ControlElementKnob.h"

ControlElementFactory::
ControlElementFactory
(OSCSender & oscSender) :
    oscSender(oscSender)
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
        info.value = configElement["default"].as<float>();

        info.name = configElement["name"].as<std::string>();
        info.message = configElement["message"].as<std::string>();

        auto messageMute = configElement["message-mute"];
        info.messageMute =
            messageMute.IsScalar() ? messageMute.as<std::string>() : "";

        product = std::make_unique<ControlElementKnob>(info, oscSender);
    }

    return product;
}
