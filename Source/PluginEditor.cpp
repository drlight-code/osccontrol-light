/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "BinaryData.h"

#include "OSCControlElement.h"

//==============================================================================
OscsendvstAudioProcessorEditor::OscsendvstAudioProcessorEditor (OscsendvstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	heightRow(32)
{
	imageOff = ImageCache::getFromMemory(BinaryData::drlighteyesclosed_png, 
										 BinaryData::drlighteyesclosed_pngSize);
	imageOn  = ImageCache::getFromMemory(BinaryData::drlightshout_png, 
										 BinaryData::drlightshout_pngSize);

	buttonSend.setImages(true, false, true, imageOff, 1.f, Colour(), Image(), 1.f, Colour(),
		imageOn, 1.f, Colour());
	buttonSend.setClickingTogglesState(true);
	buttonSend.addListener(this);
	addAndMakeVisible(&buttonSend);

	addAndMakeVisible(&textAddress);
	addAndMakeVisible(&textPort);

	auto names = {"/track/N/azimuth", "/track/N/vbap", "/track/N/stereo", "/track/N/mono" };
	for (auto name : names) {
		auto element = std::make_unique<OSCControlElement>();
		element->setOSCSender(&oscSender);
		
		auto editor = dynamic_cast<TextEditor*>(element->getChildComponent(1));
		editor->setText(name);
		
		addAndMakeVisible(element.get());
		listControlElements.push_back(std::move(element));
	}

	setResizable(true, true);
	setResizeLimits(100, 2 * heightRow, 1920, 1080);
	setSize(400, 300);
}

OscsendvstAudioProcessorEditor::~OscsendvstAudioProcessorEditor()
{
}

//==============================================================================
void OscsendvstAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);

    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void OscsendvstAudioProcessorEditor::resized()
{
	auto rect = getLocalBounds();
    
	buttonSend.setBounds(0, 0, heightRow, heightRow);

	int heightTextFields = 24;
	int insetText = (heightRow - heightTextFields) / 2;

	Rectangle<int> textBounds(heightRow, insetText, getWidth() - heightRow - 4, heightTextFields);
	textPort.setBounds(textBounds.removeFromRight(48));
	textBounds.removeFromRight(4);
	textAddress.setBounds(textBounds);

	rect.removeFromTop(heightRow);
	for (auto & controlElement : listControlElements) {
		controlElement->setBounds(rect.removeFromTop(heightRow));
	}
}

void OscsendvstAudioProcessorEditor::buttonClicked(Button * button) {
	if (button->getToggleState()) {
		DBG("on");
		auto hostname = textAddress.getText();
		auto port = textPort.getText().getIntValue();

		auto message = String("connecting to ") + hostname + String(":") + String(port);
		DBG(message);
		oscSender.connect(hostname, port);

		for (auto & control : listControlElements) {
			control->send();
		}
	}
	else {
		DBG("off");
		oscSender.disconnect();
	}
}
