/*

t  oscsend-vst - An audio plugin that speaks OSC.
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

#include <yaml-cpp/yaml.h>

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "BinaryData.h"

#include "LayoutHints.h"

#include "ControlElementFactory.h"
#include "ControlElement.h"


OscsendvstAudioProcessorEditor::
OscsendvstAudioProcessorEditor
(OscsendvstAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p)
{
    dirPresets =
        File(SystemStats::getEnvironmentVariable("OSCSEND_PRESET_PATH", ""));

    auto scaleFactor = SystemStats::getEnvironmentVariable
        ("OSCSEND_SCALE_FACTOR", "1").getFloatValue();
    Desktop::getInstance().setGlobalScaleFactor(scaleFactor);

    LookAndFeel::getDefaultLookAndFeel()
        .setDefaultSansSerifTypefaceName ("Liberation Mono");

    initializeMainUIComponents();

    setPaintingIsUnclipped(true);
    setResizable(true, true);
    setResizeLimits(245, 2 * LayoutHints::heightRow, 1920, 1080);
    setSize(300, 300);
}

OscsendvstAudioProcessorEditor::
~OscsendvstAudioProcessorEditor()
{
}

void
OscsendvstAudioProcessorEditor::
initializeMainUIComponents()
{
    auto imagePreset = ImageCache::getFromMemory
        (BinaryData::listicon_png, BinaryData::listicon_pngSize);
    auto imagePresetFolder = ImageCache::getFromMemory
        (BinaryData::presetfoldericon_png,
         BinaryData::presetfoldericon_pngSize);
    auto imageReset = ImageCache::getFromMemory
        (BinaryData::reseticon_png, BinaryData::reseticon_pngSize);

    auto opacityNormal = 0.7f;
    auto opacityOver = 1.0f;
    buttonPreset.setImages
        (true, false, true,
         imagePreset, opacityNormal, Colour(),
         Image(), opacityOver, Colour(),
         Image(), 1.0, Colour());
    buttonPreset.setTriggeredOnMouseDown(true);
    buttonPreset.addListener(this);
    addAndMakeVisible(buttonPreset);

    buttonPresetFolder.setImages
        (true, false, true,
         imagePresetFolder, opacityNormal, Colour(),
         Image(), opacityOver, Colour(),
         Image(), 1.0, Colour());
    buttonPresetFolder.setTriggeredOnMouseDown(true);
    buttonPresetFolder.addListener(this);
    addAndMakeVisible(buttonPresetFolder);

    buttonReset.setImages
        (true, false, true,
         imageReset, opacityNormal, Colour(),
         Image(), opacityOver, Colour(),
         Image(), 1.0, Colour());
    buttonReset.setTriggeredOnMouseDown(true);
    buttonReset.addListener(this);
    buttonReset.setEnabled(false);
    addAndMakeVisible(buttonReset);

    auto imageSendOff = ImageCache::getFromMemory
        (BinaryData::drlighteyesclosed_png,
         BinaryData::drlighteyesclosed_pngSize);
    auto imageSendOn  = ImageCache::getFromMemory
        (BinaryData::drlightshout_png,
         BinaryData::drlightshout_pngSize);

    auto opacityOff = 0.9f;
    buttonConnect.setImages
        (true, false, true,
         imageSendOff, opacityOff, Colour(),
         Image(), opacityOver, Colour(),
         imageSendOn, opacityOver, Colour());
    buttonConnect.setClickingTogglesState(true);
    buttonConnect.setTriggeredOnMouseDown(true);
    buttonConnect.setEnabled(false);
    buttonConnect.getToggleStateValue().addListener(this);
    addAndMakeVisible(&buttonConnect);

    textHost.setEnabled(false);
    addAndMakeVisible(&textHost);

    textPort.setEnabled(false);
    addAndMakeVisible(&textPort);

    viewport.setPaintingIsUnclipped(true);
    addAndMakeVisible(viewport);
}

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
    auto area = getLocalBounds();
    auto areaHeader = Rectangle<int>
        (0, 0, area.getWidth(), heightRow);

    buttonPreset.setBounds(areaHeader.removeFromLeft(heightRow));
    buttonPresetFolder.setBounds(areaHeader.removeFromLeft(heightRow));
    buttonReset.setBounds(areaHeader.removeFromLeft(heightRow));

    buttonConnect.setBounds(areaHeader.removeFromRight(heightRow));

    auto insetText = LayoutHints::getTextBoxInset();
    auto gap = LayoutHints::sizeGap;

    areaHeader.removeFromTop(insetText);
    areaHeader.removeFromBottom(insetText);

    textPort.setBounds
        (areaHeader.removeFromRight(LayoutHints::widthTextPort));
    areaHeader.removeFromRight(gap);
    areaHeader.removeFromLeft(gap);
    textHost.setBounds(areaHeader);

    area.removeFromTop(heightRow);

    viewport.setBounds(area);

    auto container = getActiveControlContainer();
    if(container) {
        auto controlArea = container->getBounds();
        controlArea.setWidth
            (viewport.getMaximumVisibleWidth());
        container->setBounds(controlArea);
    }
}

void
OscsendvstAudioProcessorEditor::
buttonClicked
(Button * button)
{
    if (button == &buttonPresetFolder) {
        choosePresetFolder();
    }
    else if (button == &buttonPreset) {
        handlePresetButton();
    }
    else if (button == &buttonReset) {
        pageMap[activePage]->connected = false;
        loadPreset(activePage);
        switchToPage(activePage);
    }
}

void
OscsendvstAudioProcessorEditor::
valueChanged (Value & value)
{
    if(value.getValue()) {
        connectOsc(pageMap[activePage]->host.getValue(),
                   pageMap[activePage]->port.getValue());
    }
    else {
        disconnectOsc();
    }
}

ControlContainer *
OscsendvstAudioProcessorEditor::
getActiveControlContainer()
{
    return activePage != "" ?
        pageMap[activePage]->container.get() :
        nullptr;
}

void
OscsendvstAudioProcessorEditor::
handlePresetButton()
{
    if(!dirPresets.exists()) {
        choosePresetFolder();
    }
    else {
        auto preset = pickPresetFile();
        auto presetPath = preset.getFullPathName();

        if(preset.exists()) {

            buttonConnect.setEnabled(true);
            buttonReset.setEnabled(true);
            textHost.setEnabled(true);
            textPort.setEnabled(true);

            if(presetPath != activePage) {
                auto pageIter = pageMap.find(presetPath);
                auto pageExists = pageIter != pageMap.end();

                if(!pageExists) {
                    createPage(presetPath);
                    loadPreset(preset);
                }

                switchToPage(presetPath);
            }
        }
    }
}

void
OscsendvstAudioProcessorEditor::
createPage(String presetPath)
{
    auto pageInfo = std::make_unique<PageInfo>();

    pageInfo->container = std::make_unique<ControlContainer>();
    pageInfo->container->setPaintingIsUnclipped(true);
    pageMap[presetPath] = std::move(pageInfo);
}

void
OscsendvstAudioProcessorEditor::
connectActivePageValues()
{
    textHost.getTextValue().referTo(pageMap[activePage]->host);
    textPort.getTextValue().referTo(pageMap[activePage]->port);

    buttonConnect.getToggleStateValue()
        .referTo(pageMap[activePage]->connected);
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
    auto presetPath = preset.getFullPathName();
    auto container = pageMap[presetPath]->container.get();

    container->getElementList().clear();

    YAML::Node config;
    try {
        config = YAML::LoadFile(presetPath.toStdString());
    }
    catch (YAML::BadFile &e) {
        std::string message =
            "Unable to load config: " + presetPath.toStdString();
        throw std::runtime_error(message);
    }

    if (config.IsNull()) {
        return;
    }

    auto configNetwork = config["network"];

    pageMap[presetPath]->host = String(configNetwork["host"].as<std::string>());
    pageMap[presetPath]->port = String(configNetwork["port"].as<std::string>());

    auto autoConnect = configNetwork["auto-connect"];
    pageMap[presetPath]->connected =
        autoConnect.IsScalar() ? autoConnect.as<bool>() : true;

    ControlElementFactory factory(container->getOSCSender());
    YAML::Node controls = config["controls"];
    YAML::Node interface = config["interface"];
    int accumulatedHeight = 0;
    for(auto control : controls) {
        auto element = factory.createControlElement(control, interface);

        accumulatedHeight +=
            element->getNumberOfRows() * LayoutHints::heightRow;

        element->setEnabled(pageMap[presetPath]->connected.getValue());

        container->addAndMakeVisible(element.get());
        container->getElementList().push_back(std::move(element));
    }
    container->setBounds(0, 0, getWidth(), accumulatedHeight);
}

void
OscsendvstAudioProcessorEditor::
switchToPage
(String presetPath)
{
    activePage = presetPath;

    connectActivePageValues();

    viewport.setViewedComponent
        (getActiveControlContainer(), false);
    getActiveControlContainer()->resized();

    resized();
}

void
OscsendvstAudioProcessorEditor::
connectOsc(String host, int port)
{
    auto container = getActiveControlContainer();
    auto message = String("connecting to ")
        + host + String(":") + String(port);
    DBG(message);

    container->getOSCSender().connect(host, port);

    for (auto & control :
             container->getElementList()) {
        control->setEnabled(true);
        control->send();
    }
}

void
OscsendvstAudioProcessorEditor::
disconnectOsc()
{
    auto container = getActiveControlContainer();
    for (auto & control : container->getElementList()) {
        control->setEnabled(false);
    }
    container->getOSCSender().disconnect();
}
