/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OSCControlElement;
using OSCControlElementUniq = std::unique_ptr<OSCControlElement>;

//==============================================================================
/**
*/
class OscsendvstAudioProcessorEditor  : 
	public AudioProcessorEditor,
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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OscsendvstAudioProcessor& processor;

	const int heightRow;

	Image imageOff, imageOn;
	ImageButton buttonSend;

	TextEditor textAddress;
	TextEditor textPort;
	std::list<OSCControlElementUniq> listControlElements;

	OSCSender oscSender;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscsendvstAudioProcessorEditor)
};
