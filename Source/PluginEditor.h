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
      public Button::Listener
{
public:
    OscsendvstAudioProcessorEditor (OscsendvstAudioProcessor&);
    ~OscsendvstAudioProcessorEditor ();

    //==============================================================================
    void paint (Graphics&) override;
    void resized () override;

    void buttonClicked (Button *) override;

private:
    void handlePresetButton();
    void updateActivePageInfo();

    ControlContainer * getActiveControlContainer();

    void connectOsc(String host, int port);
    void disconnectOsc();

    void choosePresetFolder();
    File pickPresetFile();
    void loadPreset(File preset);
    void switchToPage(String pathName);

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
        String host;
        String port;
        bool connected;

        std::unique_ptr<ControlContainer>
        container;
    };

    using PageMap = std::map<String, std::unique_ptr<PageInfo>>;

    PageMap pageMap;
    String activePage = "";

    OSCSender oscSender;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR
    (OscsendvstAudioProcessorEditor)
};
