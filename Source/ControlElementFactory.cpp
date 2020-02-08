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
(OSCSender & sender) :
    sender(sender)
{
}

std::unique_ptr<ControlElement>
ControlElementFactory::
createControlElement
(YAML::Node config)
{
    auto name = config["name"].as<std::string>();
    auto type = config["type"].as<std::string>();

    DBG("creating control " + name + " of type " + type);

    std::unique_ptr<ControlElement> product;
    if(type == "knob") {
        product = std::make_unique<ControlElementKnob>(sender);
    }

    return product;
}
