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

#pragma once

#include <memory>

#include <JuceHeader.h>

class ControlElement :
    public Component,
    public Button::Listener
{
public:
    struct CreateInfo {
        std::pair<float, float> range;
        float value;

        std::string name;
        std::string message;
        std::string messageMute;

        bool showNames;
    };

    ControlElement
    (OSCSender & oscSender);

    void resized() override;
    void buttonClicked(Button* button) override;

    virtual int getNumberOfRows() const = 0;
    virtual void send() = 0;

protected:
    OSCSender & oscSender;

    std::unique_ptr<TextButton> buttonMute;
    std::string messageMute;
};
