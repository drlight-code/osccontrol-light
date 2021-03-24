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

#include "ControlElementHost.h"


ControlElementHost::
ControlElementHost
(const CreateInfo & createInfo,
 OSCSender & oscSender,
 AudioProcessor & processor) :
    ControlElement (createInfo, oscSender)
{
    createHostParameter ();

    parameter->addListener (this);
    processor.addParameter (parameter);
}

void
ControlElementHost::
createHostParameter ()
{
    switch (createInfo.type) {
    case ControlElement::Type::Float: {
        parameter = new AudioParameterFloat
            (createInfo.name, createInfo.name,
             createInfo.range.first, createInfo.range.second,
             createInfo.defaultValue);
        break;
    }

    case ControlElement::Type::Int: {
        parameter = new AudioParameterInt
            (createInfo.name, createInfo.name,
             createInfo.range.first, createInfo.range.second,
             createInfo.defaultValue);
        break;
    }

    case ControlElement::Type::Toggle: {
        parameter = new AudioParameterBool
            (createInfo.name, createInfo.name,
             createInfo.defaultValue);
        break;
    }}
}

void
ControlElementHost::
parameterValueChanged
(int index, float value)
{
    float outValue = createInfo.range.first +
        (createInfo.range.second - createInfo.range.first) * value;

    if (createInfo.type == Type::Toggle) {
        sendValue.setValue (value >= 0.5f);
    }
    else {
        sendValue.setValue (outValue);
    }

    // temporary fix to work around a bug in JUCE VST3 on Linux
    if(! lastSentValue.equalsWithSameType(sendValue.getValue())) {
        lastSentValue = sendValue.getValue();
        sendValue.getValueSource().sendChangeMessage(true);
    }
}

void
ControlElementHost::
parameterGestureChanged
(int index, bool starting)
{
}

void
ControlElementHost::
serialize
(MemoryOutputStream & stream)
{
    stream.writeFloat (parameter->getValue());
}

void
ControlElementHost::
deserialize
(MemoryInputStream & stream)
{
    parameter->setValueNotifyingHost (stream.readFloat());
}
