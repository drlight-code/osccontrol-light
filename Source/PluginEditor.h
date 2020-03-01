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

#include "PresetPage.h"

#include "PluginProcessor.h"

class OscsendvstAudioProcessorEditor
    : public AudioProcessorEditor,
      public Button::Listener
{
public:
    OscsendvstAudioProcessorEditor
    (OscsendvstAudioProcessor&, File pathPreset);
    ~OscsendvstAudioProcessorEditor ();

    void paint (Graphics&) override;
    void resized () override;

    void buttonClicked (Button *) override;

private:
    void initializeMainUIComponents ();

    void handlePresetButton ();
    void choosePresetFolder ();
    File pickPresetFile ();

    void switchToPage (String pathName);
    bool isPageLoaded() const;

    OscsendvstAudioProcessor& processor;

    File pathPreset;

    ImageButton buttonPreset;
    ImageButton buttonPresetFolder;
    ImageButton buttonReset;
    ImageButton buttonConnect;

    TextEditor textHost;
    TextEditor textPort;

    Viewport viewport;

    using PageMap = std::map<String, std::unique_ptr<PresetPage>>;
    PageMap pageMap;
    PageMap::iterator activePage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR
    (OscsendvstAudioProcessorEditor)
};
