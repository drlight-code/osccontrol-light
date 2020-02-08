/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <list>

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OSCControlElement;
using OSCControlElementUniq = std::unique_ptr<OSCControlElement>;

//==============================================================================
/**
*/
class OscsendvstAudioProcessorEditor
  : public AudioProcessorEditor,
    public Button::Listener
{
public:
    OscsendvstAudioProcessorEditor (OscsendvstAudioProcessor&);
    ~OscsendvstAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked(Button *) override;

private:
    void connectOsc();
    void choosePresetFolder();
    File pickPresetFile();
    void loadPreset(File preset);

  OscsendvstAudioProcessor& processor;

    File dirPresets;

    const int heightRow;

    ImageButton buttonPreset;
    ImageButton buttonPresetFolder;
    ImageButton buttonSend;

    TextEditor textAddress;
    TextEditor textPort;
    std::list<OSCControlElementUniq> listControlElements;

    OSCSender oscSender;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR
    (OscsendvstAudioProcessorEditor)
};
