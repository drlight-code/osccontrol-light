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

#include "BinaryData.h"

#include "LayoutHints.h"

#include "PluginProcessor.h"
#include "PluginEditor.h"

OscsendvstAudioProcessorEditor::
OscsendvstAudioProcessorEditor
(OscsendvstAudioProcessor& p, File dirPreset) :
    AudioProcessorEditor (&p),
    processor (p),
    dirPreset (dirPreset),
    activePage(pageMap.end())
{
    auto scaleFactor = SystemStats::getEnvironmentVariable
        ("OSCSEND_SCALE_FACTOR", "1").getFloatValue();
    setScaleFactor (scaleFactor);

    LookAndFeel::getDefaultLookAndFeel ()
        .setDefaultSansSerifTypefaceName ("Liberation Mono");

    initializeMainUIComponents ();

    setPaintingIsUnclipped (true);
    setResizable (true, true);
    setResizeLimits (245, 2 * LayoutHints::heightRow, 1920, 1080);
    setSize (300, 300);
}

OscsendvstAudioProcessorEditor::
~OscsendvstAudioProcessorEditor ()
{
}

void
OscsendvstAudioProcessorEditor::
initializeMainUIComponents ()
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
    addAndMakeVisible(&buttonConnect);

    textHost.setEnabled(false);
    textHost.setJustification(Justification::right);
    addAndMakeVisible(&textHost);

    textPort.setEnabled(false);
    textPort.setJustification(Justification::right);
    addAndMakeVisible(&textPort);

    viewport.setPaintingIsUnclipped(true);
    addAndMakeVisible(viewport);
}

void
OscsendvstAudioProcessorEditor::
paint
(Graphics& g)
{
    g.fillAll (getLookAndFeel ().findColour
        (ResizableWindow::backgroundColourId));
}

void
OscsendvstAudioProcessorEditor::
resized ()
{
    auto heightRow = LayoutHints::heightRow;
    auto area = getLocalBounds ();
    auto areaHeader = Rectangle<int>
        (0, 0, area.getWidth(), heightRow);

    buttonPreset.setBounds (areaHeader.removeFromLeft (heightRow));
    buttonPresetFolder.setBounds (areaHeader.removeFromLeft (heightRow));
    buttonReset.setBounds (areaHeader.removeFromLeft (heightRow));

    buttonConnect.setBounds (areaHeader.removeFromRight (heightRow));

    auto insetText = LayoutHints::getTextBoxInset ();
    auto gap = LayoutHints::sizeGap;

    areaHeader.removeFromTop (insetText);
    areaHeader.removeFromBottom (insetText);

    textPort.setBounds
        (areaHeader.removeFromRight (LayoutHints::widthTextPort));
    areaHeader.removeFromRight (gap);
    areaHeader.removeFromLeft (gap);
    textHost.setBounds (areaHeader);

    area.removeFromTop(heightRow);
    viewport.setBounds (area);

    if (isPageLoaded ()) {
        auto container = activePage->second->getContainerComponent ();
        auto areaControl = container->getBounds ();
        areaControl.setWidth (viewport.getMaximumVisibleWidth ());
        container->setBounds (areaControl);
    }
}

void
OscsendvstAudioProcessorEditor::
buttonClicked
(Button * button)
{
    if (button == &buttonPresetFolder) {
        choosePresetFolder ();
    }
    else if (button == &buttonPreset) {
        handlePresetButton ();
    }
    else if (button == &buttonReset) {
        activePage->second->loadFromFile (activePage->first);
        activePage->second->getContainerComponent ()->resized ();
        resized ();
    }
}

void
OscsendvstAudioProcessorEditor::
handlePresetButton()
{
    if (!dirPreset.exists ()) {
        choosePresetFolder ();
    }
    else {
        auto filePreset = pickPresetFile ();
        auto dirPreset = filePreset.getFullPathName ();

        if (filePreset.exists ()) {
            buttonConnect.setEnabled (true);
            buttonReset.setEnabled (true);
            textHost.setEnabled (true);
            textPort.setEnabled (true);

            auto pageIter = pageMap.find (dirPreset);
            auto pageExists = pageIter != pageMap.end ();
            if (!pageExists) {
                auto presetPage = std::make_unique<PresetPage> ();
                presetPage->loadFromFile (filePreset);
                pageMap[dirPreset] = std::move (presetPage);
                switchToPage (dirPreset);
            }
            else if (dirPreset != activePage->first) {
                switchToPage (dirPreset);
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
        dirPreset, nullptr, nullptr);

    auto colourBg =
        LookAndFeel::getDefaultLookAndFeel()
        .findColour(ResizableWindow::backgroundColourId);
    FileChooserDialogBox dialogBox("Preset directory",
        "Select the directory containing OSC preset definitions.",
        browser, false, colourBg);

    if (dialogBox.show()) {
        dirPreset = browser.getSelectedFile(0);
    }
}

File
OscsendvstAudioProcessorEditor::
pickPresetFile()
{
    auto files = dirPreset.findChildFiles(File::findFiles, true, "*.yaml");
    files.sort();

    PopupMenu popup;
    int id = 1;
    for (auto & file : files) {
        auto name = file.getRelativePathFrom(dirPreset).dropLastCharacters(5);
        name = name.replaceCharacter('\\', '/');
        popup.addItem(id++, name);
    }

    int index = popup.show();

    return files[index-1];
}

void
OscsendvstAudioProcessorEditor::
switchToPage
(String presetPath)
{
    Logger::writeToLog("switchToPage");

    activePage = pageMap.find (presetPath);

    textHost.getTextValue ().referTo (activePage->second->getHostValue ());
    textPort.getTextValue ().referTo (activePage->second->getPortValue ());
    buttonConnect.getToggleStateValue ()
        .referTo (activePage->second->getConnectedValue ());

    auto container = activePage->second->getContainerComponent ();
    viewport.setViewedComponent(container, false);

    container->resized ();
    resized ();
}

bool
OscsendvstAudioProcessorEditor::
isPageLoaded
() const
{
    return activePage != pageMap.end ();
}
