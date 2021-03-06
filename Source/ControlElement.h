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

#pragma once

#include <memory>

#include <JuceHeader.h>

class ControlElement :
    public Value::Listener
{
public:

    enum class Type {
        Float,
        Int,
        Toggle
    };

    struct CreateInfo {
        String name;
        Type type;

        std::pair<float, float> range;
        float defaultValue;

        String message;
        String messageMute;
    };

    ControlElement
    (const CreateInfo & info,
     OSCSender & oscSender);

    CreateInfo getCreateInfo () const;

    void valueChanged(Value & value) override;

    virtual void send();

private:
    template <class T>
    void sendTyped() {
        var valueToSend = T(sendValue.getValue());
        if(! lastSentValue.equalsWithSameType(valueToSend)) {
            auto oscMessage = OSCMessage(String(createInfo.message), T(valueToSend));
            oscSender.send(oscMessage);
            lastSentValue = valueToSend;
        }
    }
    var lastSentValue;

protected:

    OSCSender & oscSender;
    CreateInfo createInfo;
    Value sendValue;
};
