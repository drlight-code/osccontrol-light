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
    buttonConnect.addListener(this);
    addAndMakeVisible(&buttonConnect);

    addAndMakeVisible(&textHost);
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

    Rectangle<int> areaButton;
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
    if (button == &buttonConnect) {
        if (button->getToggleState()) {
            connectOsc(pageMap[activePage]->host,
                       pageMap[activePage]->port.getIntValue());
        }
        else {
            disconnectOsc();
        }
    }
    else if (button == &buttonPresetFolder) {
        choosePresetFolder();
    }
    else if (button == &buttonPreset) {
        handlePresetButton();
    }
    else if (button == &buttonReset) {
        updateActivePageInfo();
        loadPreset(activePage);
        switchToPage(activePage);
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

            if(activePage != "")
                updateActivePageInfo();

            if(presetPath != activePage) {
                auto pageIter = pageMap.find(presetPath);
                auto pageExists = pageIter != pageMap.end();

                if(!pageExists) {
                    auto pageInfo = std::make_unique<PageInfo>();

                    pageInfo->container = std::make_unique<ControlContainer>();
                    pageInfo->container->setPaintingIsUnclipped(true);
                    pageMap[presetPath] = std::move(pageInfo);

                    loadPreset(preset);
                }

                buttonConnect.setEnabled(true);
                buttonReset.setEnabled(true);
                switchToPage(presetPath);
            }
        }
    }
}

void
OscsendvstAudioProcessorEditor::
updateActivePageInfo()
{
    auto & pageInfo = pageMap[activePage];

    pageInfo->host = textHost.getText();
    pageInfo->port = textPort.getText();
    pageInfo->connected = buttonConnect.getToggleState();
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

    pageMap[presetPath]->host = configNetwork["host"].as<std::string>();
    pageMap[presetPath]->port = configNetwork["port"].as<std::string>();

    auto autoConnect = configNetwork["auto-connect"];
    pageMap[presetPath]->connected =
        autoConnect.IsScalar() ? autoConnect.as<bool>() : true;

    ControlElementFactory factory(oscSender);
    YAML::Node controls = config["controls"];
    YAML::Node interface = config["interface"];
    int accumulatedHeight = 0;
    for(auto control : controls) {
        auto element = factory.createControlElement(control, interface);

        accumulatedHeight +=
            element->getNumberOfRows() * LayoutHints::heightRow;

        element->setEnabled(pageMap[presetPath]->connected);

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

    textHost.setText(pageMap[activePage]->host);
    textPort.setText(pageMap[activePage]->port);

    buttonConnect.setToggleState
        (false, NotificationType::sendNotification);
    buttonConnect.setToggleState
        (pageMap[activePage]->connected,
            NotificationType::sendNotification);

    viewport.setViewedComponent
        (getActiveControlContainer(), false);
    getActiveControlContainer()->resized();

    resized();
}

void
OscsendvstAudioProcessorEditor::
connectOsc(String host, int port)
{
    auto message = String("connecting to ")
        + host + String(":") + String(port);
    DBG(message);
    oscSender.connect(host, port);

    for (auto & control :
             getActiveControlContainer()->getElementList()) {
        control->setEnabled(true);
        control->send();
    }
}

void
OscsendvstAudioProcessorEditor::
disconnectOsc()
{
    for (auto & control :
             getActiveControlContainer()->getElementList()) {
        control->setEnabled(false);
    }
    oscSender.disconnect();
}
