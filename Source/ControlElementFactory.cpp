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
(YAML::Node config)
{
    auto name = config["name"].as<std::string>();
    auto type = config["type"].as<std::string>();

    std::unique_ptr<ControlElement> product;
    if(type == "knob") {
        ControlElementKnob::CreateInfo info;

        auto range = config["range"];
        info.range = std::make_pair(range[0].as<float>(),
                                    range[1].as<float>());
        info.value = config["default"].as<float>();

        info.message = config["message"].as<std::string>();
        auto messageMute = config["message-mute"];
        if(messageMute.IsScalar())
            info.messageMute = messageMute.as<std::string>();
        else
            info.messageMute = "";

        product = std::make_unique<ControlElementKnob>(info, oscSender);
    }

    return product;
}
