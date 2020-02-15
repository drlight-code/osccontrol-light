/*
  ==============================================================================

  This file was auto-generated!

  It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ControlContainer.h"

#include "PluginProcessor.h"

//==============================================================================
/**
 */
class OscsendvstAudioProcessorEditor
    : public AudioProcessorEditor,
      public Button::Listener,
      public Value::Listener
{
public:
    OscsendvstAudioProcessorEditor (OscsendvstAudioProcessor&);
    ~OscsendvstAudioProcessorEditor ();

    //==============================================================================
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
