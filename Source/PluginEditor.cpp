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

#include "LayoutHints.h"

#include "ControlElementFactory.h"
#include "ControlElement.h"

//==============================================================================
OscsendvstAudioProcessorEditor::
OscsendvstAudioProcessorEditor
(OscsendvstAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p)
{
    dirPresets =
        File(SystemStats::getEnvironmentVariable("OSCSEND_PRESET_PATH", ""));

    LookAndFeel::getDefaultLookAndFeel()
        .setDefaultSansSerifTypefaceName ("Liberation Mono");

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

    setPaintingIsUnclipped(true);
    viewport.setPaintingIsUnclipped(true);
    controlContainer.setPaintingIsUnclipped(true);

    viewport.setViewedComponent(&controlContainer);
    addAndMakeVisible(viewport);

    setResizable(true, true);
    setResizeLimits(200, 2 * LayoutHints::heightRow, 1920, 1080);
    setSize(300, 300);
}

OscsendvstAudioProcessorEditor::
~OscsendvstAudioProcessorEditor()
{
}

//==============================================================================
void
OscsendvstAudioProcessorEditor::
paint
(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour
        (ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(15.0f);
}

void
OscsendvstAudioProcessorEditor::
resized()
{
    auto heightRow = LayoutHints::heightRow;
    auto sizeIcons = LayoutHints::sizeIcons;
    auto area = getLocalBounds();
    auto areaHeader = Rectangle<int>
        (0, 0, area.getWidth(), heightRow);

    buttonPreset.setBounds(areaHeader.removeFromLeft(sizeIcons));
    buttonPresetFolder.setBounds(areaHeader.removeFromLeft(sizeIcons));

    buttonSend.setBounds(areaHeader.removeFromRight(heightRow));

    auto insetText = LayoutHints::getTextBoxInset();
    auto gap = LayoutHints::sizeGap;

    areaHeader.removeFromTop(insetText);
    areaHeader.removeFromBottom(insetText);

    areaHeader.removeFromRight(gap);
    textPort.setBounds
        (areaHeader.removeFromRight(LayoutHints::widthTextPort));
    areaHeader.removeFromRight(gap);
    areaHeader.removeFromLeft(gap);
    textAddress.setBounds(areaHeader);

    area.removeFromTop(heightRow);

    viewport.setBounds(area);

    auto controlArea = controlContainer.getBounds();
    controlArea.setWidth
        (viewport.getMaximumVisibleWidth());
    controlContainer.setBounds(controlArea);
}

void
OscsendvstAudioProcessorEditor::
buttonClicked
(Button * button)
{
    if (button == &buttonSend) {
        if (button->getToggleState()) {
            connectOsc();
        }
        else {
            disconnectOsc();
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

void
OscsendvstAudioProcessorEditor::
choosePresetFolder()
{
    FileBrowserComponent browser(
        FileBrowserComponent::openMode |
        FileBrowserComponent::canSelectDirectories,
        dirPresets, nullptr, nullptr);

    auto colourBg =
        LookAndFeel::getDefaultLookAndFeel()
        .findColour(ResizableWindow::backgroundColourId);
    FileChooserDialogBox dialogBox("Preset directory",
        "Select the directory containing OSC preset definitions.",
        browser, false, colourBg);

    if (dialogBox.show()) {
        dirPresets = browser.getSelectedFile(0);
    }
}

File
OscsendvstAudioProcessorEditor::
pickPresetFile()
{
    auto files = dirPresets.findChildFiles(File::findFiles, true, "*.yaml");

    PopupMenu popup;
    int id = 1;
    for (auto & file : files) {
        auto name = file.getRelativePathFrom(dirPresets).dropLastCharacters(5);
        name = name.replaceCharacter('\\', '/');
        popup.addItem(id++, name);
    }
    int index = popup.show();

    return files[index-1];
}

void
OscsendvstAudioProcessorEditor::
loadPreset
(File preset)
{
    controlContainer.getElementList().clear();

    auto filename = preset.getFullPathName().toStdString();
    YAML::Node config;

    try {
        config = YAML::LoadFile(filename);
    }
    catch (YAML::BadFile &e) {
        std::string message = "Unable to load config: " + filename;
        throw std::runtime_error(message);
    }

    if (config.IsNull()) {
        return;
    }

    auto configNetwork = config["network"];

    textAddress.setText(configNetwork["host"].as<std::string>());
    textPort.setText(configNetwork["port"].as<std::string>());

    ControlElementFactory factory(oscSender);
    YAML::Node controls = config["controls"];
    YAML::Node interface = config["interface"];
    int accumulatedHeight = 0;
    for(auto control : controls) {
        auto element = factory.createControlElement(control, interface);

        accumulatedHeight +=
            element->getNumberOfRows() * LayoutHints::heightRow;

        element->setEnabled(false);

        controlContainer.addAndMakeVisible(element.get());
        controlContainer.getElementList().push_back(std::move(element));
    }
    controlContainer.setBounds(0, 0, getWidth(), accumulatedHeight);

    buttonSend.setToggleState(false, NotificationType::sendNotification);
    auto autoConnect = configNetwork["auto-connect"];
    if(autoConnect.IsScalar() && autoConnect.as<bool>()) {
        buttonSend.setToggleState(true, NotificationType::sendNotification);
    }
}

void
OscsendvstAudioProcessorEditor::
connectOsc()
{
    auto hostname = textAddress.getText();
    auto port = textPort.getText().getIntValue();

    auto message = String("connecting to ")
        + hostname + String(":") + String(port);
    DBG(message);
    oscSender.connect(hostname, port);

    for (auto & control : controlContainer.getElementList()) {
        control->setEnabled(true);
        control->send();
    }
}

void
OscsendvstAudioProcessorEditor::
disconnectOsc()
{
    for (auto & control : controlContainer.getElementList()) {
        control->setEnabled(false);
    }
    oscSender.disconnect();
}
