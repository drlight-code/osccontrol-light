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

class ControlContainer;
using ControlContainerUnique = std::unique_ptr<ControlContainer>;

class PresetPage :
    public Value::Listener
{
public:

    PresetPage ();
    ~PresetPage ();

    void load (InputStream & inputStream);
    void loadFromFile (File const & file);

    Value & getHostValue ();
    Value & getPortValue ();

    Value & getConnectedValue ();
    void valueChanged (Value & value) override;

    Component * getContainerComponent();

    void connectOsc ();
    void disconnectOsc ();

private:

    Value host;
    Value port;
    Value connected;

    OSCSender oscSender;
    ControlContainerUnique container;
};
