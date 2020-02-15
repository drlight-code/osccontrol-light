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

#include <JuceHeader.h>

#include "ControlContainer.h"

#include "PluginProcessor.h"

class OscsendvstAudioProcessorEditor
    : public AudioProcessorEditor,
      public Button::Listener,
      public Value::Listener
{
public:
    OscsendvstAudioProcessorEditor (OscsendvstAudioProcessor&);
    ~OscsendvstAudioProcessorEditor ();

    void paint (Graphics&) override;
    void resized () override;

    void buttonClicked (Button *) override;
    void valueChanged (Value & value) override;

private:
    void initializeMainUIComponents ();

    void handlePresetButton ();
    void createPage (String presetPath);
    void connectActivePageValues ();

    ControlContainer * getActiveControlContainer ();

    void connectOsc (String host, int port);
    void disconnectOsc ();

    void choosePresetFolder ();
    File pickPresetFile ();
    void loadPreset (File preset);
    void switchToPage (String pathName);

    OscsendvstAudioProcessor& processor;

    File dirPresets;

    ImageButton buttonPreset;
    ImageButton buttonPresetFolder;
    ImageButton buttonReset;
    ImageButton buttonConnect;

    TextEditor textHost;
    TextEditor textPort;

    Viewport viewport;

    struct PageInfo {
        Value host;
        Value port;
        Value connected;

        ControlContainerUniq container;
    };

    using PageMap = std::map<String, std::unique_ptr<PageInfo>>;

    PageMap pageMap;
    String activePage = "";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR
    (OscsendvstAudioProcessorEditor)
};
