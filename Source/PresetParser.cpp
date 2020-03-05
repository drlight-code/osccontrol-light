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

#include "ControlElement.h"

#include "PresetParser.h"

const std::map<String, ControlElement::Type>
PresetParser::mapTypeNames =
{
    {"float", ControlElement::Type::Float},
    {"int", ControlElement::Type::Int},
    {"toggle", ControlElement::Type::Toggle},
};

PresetParser::
PresetParser
(File filePreset)
{
    auto pathPreset = filePreset.getFullPathName().toStdString();
    try {
        config = YAML::LoadFile(pathPreset);
    }
    catch (YAML::BadFile &e) {
        std::string message = "Unable to load config: " + pathPreset;
        throw std::runtime_error(message);
    }

    if(config.IsNull()) {
        std::string message = "Empty config: " + pathPreset;
        throw std::runtime_error(message);
    }
}

String
PresetParser::
getHost
() const
{
    auto configNetwork = config["network"];
    return String(configNetwork["host"].as<std::string>());
}

int
PresetParser::
getPort
() const
{
    auto configNetwork = config["network"];
    return configNetwork["port"].as<int>();
}

StringArray
PresetParser::
getControlElements
() const
{
    StringArray elements;
    auto controls = config["controls"];
    for(auto control : controls) {
        elements.add(control["name"].as<std::string>());
    }
    return elements;
}

ControlElement::CreateInfo
PresetParser::
getControlElementCreateInfo
(String element) const
{
    Logger::writeToLog("creating element: " + element);

    ControlElement::CreateInfo info;

    auto controls = config["controls"];
    for(auto control : controls) {

        if(control["name"].as<std::string>() != element) {
            continue;
        }

        info.name = control["name"].as<std::string>();

        auto typeName = control["type"].as<std::string>();
        info.type = mapTypeNames.at(typeName);

        auto range = control["range"];
        if (range.IsSequence()) {
            info.range = std::make_pair
                (range[0].as<float>(), range[1].as<float>());
        }
        else {
            info.range = std::make_pair(0.f, 1.f);
        }

        // TODO: change the type mapping to a generic mapping using the
        // external polymorphism idiom: https://stackoverflow.com/a/12653596
        switch (info.type) {
        case ControlElement::Type::Float: {
            info.defaultValue = control["default"].as<float>();
            break;
        }

        case ControlElement::Type::Int: {
            info.defaultValue = control["default"].as<int>();
            break;
        }

        case ControlElement::Type::Toggle: {
            info.defaultValue = control["default"].as<bool>();
            break;
        }

        default:
            break;
        }

        info.message = control["message"].as<std::string>();

        auto messageMute = control["message-mute"];
        info.messageMute =
            messageMute.IsScalar() ? messageMute.as<std::string>() : "";
    }

    return info;
}
