/*
  ==============================================================================

    ControlElementFactory.h
    Created: 8 Feb 2020 3:01:24pm
    Author:  bzk

  ==============================================================================
*/

#pragma once

#include <memory>

#include <yaml-cpp/yaml.h>

#include <JuceHeader.h>

class ControlElement;
class ControlElementFactory  {
public:
    ControlElementFactory(OSCSender & oscSender);

    std::unique_ptr<ControlElement>
    createControlElement
    (YAML::Node configElement,
     YAML::Node configInterface);

private:
    OSCSender & oscSender;
};
