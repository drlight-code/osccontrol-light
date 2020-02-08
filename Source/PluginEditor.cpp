/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include <yaml-cpp/yaml.h>

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "BinaryData.h"

#include "OSCControlElement.h"

//==============================================================================
OscsendvstAudioProcessorEditor::
OscsendvstAudioProcessorEditor
(OscsendvstAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    heightRow(32)
{
    dirPresets =
        File(SystemStats::getEnvironmentVariable("OSCSEND_PRESET_PATH", ""));

    auto imagePreset = ImageCache::getFromMemory
        (BinaryData::listicon_png, BinaryData::listicon_pngSize);
    auto imagePresetFolder = ImageCache::getFromMemory
        (BinaryData::presetfoldericon_png,
         BinaryData::presetfoldericon_pngSize);

    auto colourOverlay = Colour(uint8(66), uint8(162), uint8(200), 0.5f);
    buttonPreset.setImages(true, false, true,
        imagePreset, 1.0, Colour(),
        Image(), 1.0, colourOverlay,
        Image(), 1.0, Colour());
    buttonPreset.setTriggeredOnMouseDown(true);
    buttonPreset.addListener(this);
    addAndMakeVisible(buttonPreset);

    buttonPresetFolder.setImages(true, false, true,
        imagePresetFolder, 1.0, Colour(),
        Image(), 1.0, colourOverlay,
        Image(), 1.0, Colour());
    buttonPresetFolder.setTriggeredOnMouseDown(true);
    buttonPresetFolder.addListener(this);
    addAndMakeVisible(buttonPresetFolder);

    auto imageSendOff = ImageCache::getFromMemory
        (BinaryData::drlighteyesclosed_png,
         BinaryData::drlighteyesclosed_pngSize);
    auto imageSendOn  = ImageCache::getFromMemory
        (BinaryData::drlightshout_png,
         BinaryData::drlightshout_pngSize);

    buttonSend.setImages
        (true, false, true,
         imageSendOff, 1.f, Colour(),
         Image(), 1.f, Colour(),
         imageSendOn, 1.f, Colour());
    buttonSend.setClickingTogglesState(true);
    buttonSend.setTriggeredOnMouseDown(true);
    buttonSend.addListener(this);
    addAndMakeVisible(&buttonSend);

    addAndMakeVisible(&textAddress);
    addAndMakeVisible(&textPort);

    //auto names = {"/track/N/azimuth", "/track/N/vbap", "/track/N/stereo", "/track/N/mono" };
    //for (auto name : names) {
    //  auto element = std::make_unique<OSCControlElement>();
    //  element->setOSCSender(&oscSender);
    //
    //  auto editor = dynamic_cast<TextEditor*>(element->getChildComponent(1));
    //  editor->setText(name);
    //
    //  addAndMakeVisible(element.get());
    //  listControlElements.push_back(std::move(element));
    //}

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
    auto rectHeader = Rectangle<int>(0, 0, rect.getWidth(), heightRow);

    buttonPreset.setBounds(rectHeader.removeFromLeft(heightRow));
    buttonPresetFolder.setBounds(rectHeader.removeFromLeft(heightRow));

    buttonSend.setBounds(rectHeader.removeFromRight(heightRow));

    int heightTextFields = 24;
    int insetText = (heightRow - heightTextFields) / 2;

    rectHeader.removeFromTop(insetText);
    rectHeader.removeFromBottom(insetText);

    rectHeader.removeFromRight(4);
    textPort.setBounds(rectHeader.removeFromRight(48));
    rectHeader.removeFromRight(4);
    rectHeader.removeFromLeft(4);
    textAddress.setBounds(rectHeader);

    /*rect.removeFromTop(heightRow);
    for (auto & controlElement : listControlElements) {
        controlElement->setBounds(rect.removeFromTop(heightRow));
    }*/
}

void OscsendvstAudioProcessorEditor::buttonClicked(Button * button) {
    if (button == &buttonSend) {
        if (button->getToggleState()) {
            connectOsc();
        }
        else {
            oscSender.disconnect();
        }
    }
    else if (button == &buttonPresetFolder) {
        choosePresetFolder();
    }
    else if (button == &buttonPreset) {
        if(!dirPresets.exists()) {
            choosePresetFolder();
        }
        else {
            auto preset = pickPresetFile();
            if(preset.exists()) {
                loadPreset(preset);
            }
        }
    }
}

void OscsendvstAudioProcessorEditor::choosePresetFolder() {
    FileBrowserComponent browser(
        FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
        dirPresets, nullptr, nullptr);

    auto colourBg = LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    FileChooserDialogBox dialogBox("Preset directory",
        "Select the directory containing OSC preset definitions.",
        browser, false, colourBg);

    if (dialogBox.show())
    {
        dirPresets = browser.getSelectedFile(0);
    }
}

File OscsendvstAudioProcessorEditor::pickPresetFile() {
    PopupMenu popup;

    auto files = dirPresets.findChildFiles(File::findFiles, true, "*.yaml");

    int id = 1;
    for (auto & file : files) {
        auto name = file.getRelativePathFrom(dirPresets).dropLastCharacters(5);
        name = name.replaceCharacter('\\', '/');
        popup.addItem(id++, name);
    }
    int index = popup.show();

    return files[index-1];
}

void OscsendvstAudioProcessorEditor::loadPreset(File preset) {
    auto filename = preset.getFullPathName().toStdString();
    YAML::Node root;

    try {
        root = YAML::LoadFile(filename);
    }
    catch (YAML::BadFile &e) {
        std::string message = "Unable to load config: " + filename;
        throw std::runtime_error(message);
    }

    if (root.IsNull()) {
        return;
    }
    
    using map = std::map<std::string, YAML::Node>;
    for (auto & pair : root.as<map>()) {
        auto name = pair.first;
        DBG(name);
    }
}

void OscsendvstAudioProcessorEditor::connectOsc() {
    auto hostname = textAddress.getText();
    auto port = textPort.getText().getIntValue();

    auto message = String("connecting to ") + hostname + String(":") + String(port);
    DBG(message);
    oscSender.connect(hostname, port);

    for (auto & control : listControlElements) {
        control->send();
    }
}
