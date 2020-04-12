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

ControlElement::
ControlElement
(const CreateInfo & createInfo,
 OSCSender & oscSender) :
    oscSender(oscSender),
    createInfo(createInfo)
{
    sendValue.addListener(this);
    sendValue.setValue (createInfo.defaultValue);
}

ControlElement::CreateInfo
ControlElement::
getCreateInfo () const
{
    return createInfo;
}

void
ControlElement::
valueChanged(Value & value)
{
    send();
}

void
ControlElement::
send()
{
    auto value = sendValue.getValue();

    switch (createInfo.type) {
    case ControlElement::Type::Float: {
        auto oscMessage =
            OSCMessage(
                String(createInfo.message),
                float(value));
        oscSender.send(oscMessage);
        break;
    }

    case ControlElement::Type::Int: {
        auto oscMessage =
            OSCMessage(
                String(createInfo.message),
                int(value));
        oscSender.send(oscMessage);
        break;
    }

    case ControlElement::Type::Toggle:
        auto oscMessage =
            OSCMessage(
                String(createInfo.message),
                float(value));
        oscSender.send(oscMessage);
        break;
    }
}
