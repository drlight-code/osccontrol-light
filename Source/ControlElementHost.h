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

#include <JuceHeader.h>

#include "ControlElement.h"

class ControlElementHost :
    public ControlElement,
    public AudioProcessorParameter::Listener,
    public Value::Listener
{
public:

    ControlElementHost
    (const CreateInfo & createInfo,
     OSCSender & oscSender,
     AudioProcessor & processor);

    void parameterValueChanged (int index, float value) override;
    void parameterGestureChanged (int index, bool starting) override;

    void valueChanged (Value & value) override;

    void send () override;

    void serialize (MemoryOutputStream & stream);
    void deserialize (MemoryInputStream & stream);

private:

    void createHostParameter ();

    AudioProcessor & processor;
    AudioProcessorParameter * parameter;

    Value sendValue;
};
using ControlElementHostUnique = std::unique_ptr<ControlElementHost>;
