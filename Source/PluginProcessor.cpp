/*

  osccontrol-light - An audio plugin that speaks OSC.
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
#ifdef EMBED_PRESET
  #include "EmbeddedPreset.h"
#endif


#include "PresetParser.h"

#include "ControlElementHost.h"
#include "ControlElementFactory.h"

#include "PluginEditor.h"

#include "PluginProcessor.h"

namespace  {
}

OSCControlAudioProcessor::
OSCControlAudioProcessor() :
    AudioProcessor (BusesProperties()
        .withInput  ("Input",  AudioChannelSet::stereo())
        .withOutput ("Output", AudioChannelSet::stereo())),
    hasUserInterface(true)
{
    auto filePlugin = File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile);
    filenamePlugin = filePlugin.getFileNameWithoutExtension();
    auto filenameLog = filenamePlugin + ".log";

    namePlugin = filenamePlugin;

    fileLogger = std::make_unique<FileLogger>
        (filePlugin.getParentDirectory().getChildFile(filenameLog),
         "osccontrol-light debug log", 0);

    Logger::setCurrentLogger(fileLogger.get());

    Logger::writeToLog(filePlugin.getFullPathName());
    Logger::writeToLog(filenamePlugin);

    auto pathPreset =
        SystemStats::getEnvironmentVariable("OSCCONTROL_PRESET_PATH", "");
    dirPreset = File (pathPreset);

    auto presetToLoad = namePlugin.fromFirstOccurrenceOf
        ("osc-", false, false).trimCharactersAtStart("-");
    Logger::writeToLog("presetToLoad: " + presetToLoad);

    if (presetToLoad != "osccontrol-light" && presetToLoad.isNotEmpty()) {
        if (!dirPreset.exists()) {
            auto message =
                String("Error loading osccontrol-light with preset: ") + presetToLoad +
                "When running osccontrol-light in headless mode "
                "for DAW integration, make sure that "
                "OSCCONTROL_PRESET_PATH is set properly in "
                "the environment!";

            throw std::runtime_error (message.toStdString ());
        }

        hasUserInterface = false;
        initializeHeadless(presetToLoad);
    }
}

OSCControlAudioProcessor::
~OSCControlAudioProcessor()
{
    Logger::writeToLog ("~OSCControlAudioProcessor");
    Logger::setCurrentLogger (nullptr);
}

File
OSCControlAudioProcessor::
locatePresetFile
(String const & namePreset)
{
    auto filenamePreset = namePreset + ".yaml";
    auto files = dirPreset.findChildFiles
        (File::findFiles, true, filenamePreset);

    if(files.isEmpty()) {
        auto message = "error: could not locate preset file: " + filenamePreset;
        Logger::writeToLog (message );
        throw std::runtime_error (message.toStdString ());
    }

    // TODO resolve possible alternatives
    return files[0];
}

void
OSCControlAudioProcessor::
initializeHeadless
(String const & namePreset)
{
    Logger::writeToLog("initializeHeadless");

#ifdef EMBED_PRESET
    int sourceSize = 0;
    const void * sourceData =
        osccontrol_embedded_preset::getNamedResource(
            osccontrol_embedded_preset::namedResourceList[0], sourceSize);
    MemoryInputStream inputStream(sourceData, sourceSize, false);
#else
    auto presetFile = locatePresetFile(namePreset);
    FileInputStream inputStream(presetFile);
#endif

    PresetParser preset (inputStream);

    oscSender = std::make_unique<OSCSender> ();
    oscSender->connect (preset.getHost (), preset.getPort ());

    ControlElementFactory factory (*oscSender);
    for(auto control : preset.getControlElements ()) {

        auto createInfo = preset.getControlElementCreateInfo (control);
        auto element = factory.createControlElementHost (createInfo, *this);

        listHostControls.push_back (std::move (element));
    }
}

const String
OSCControlAudioProcessor::
getName() const
{
    Logger::writeToLog("getName : " + namePlugin);
    return namePlugin;
}

bool
OSCControlAudioProcessor::
acceptsMidi() const
{
    Logger::writeToLog("acceptsMidi");
    return true;
}

bool
OSCControlAudioProcessor::
producesMidi() const
{
    Logger::writeToLog("producesMidi");
    return true;
}

bool
OSCControlAudioProcessor::
isMidiEffect() const
{
//    Logger::writeToLog("isMidiEffect");
    return true;
}

double
OSCControlAudioProcessor::
getTailLengthSeconds() const
{
//    Logger::writeToLog("getTailLengthSeconds");
    return 0.0;
}

int
OSCControlAudioProcessor::
getNumPrograms()
{
    // Logger::writeToLog("getNumPrograms");
    return 1;   // NB: some hosts don't cope very well if you tell
                // them there are 0 programs, so this should be at
                // least 1, even if you're not really implementing
                // programs.
}

int
OSCControlAudioProcessor::
getCurrentProgram()
{
    // Logger::writeToLog("getCurrentProgram");
    return 0;
}

void
OSCControlAudioProcessor::
setCurrentProgram
(int index)
{
    Logger::writeToLog("setCurrentProgram");
}

const String
OSCControlAudioProcessor::
getProgramName
(int index)
{
    Logger::writeToLog("getProgramName");
    return {};
}

void
OSCControlAudioProcessor::
changeProgramName
(int index, const String& newName)
{
    Logger::writeToLog("changeProgramName");
}

void
OSCControlAudioProcessor::
prepareToPlay
(double sampleRate, int samplesPerBlock)
{
    Logger::writeToLog("prepareToPlay");
}

void
OSCControlAudioProcessor::
releaseResources()
{
    Logger::writeToLog("releaseResources");
}

bool
OSCControlAudioProcessor::
isBusesLayoutSupported
(const BusesLayout& layouts) const
{
    Logger::writeToLog("isBusesLayoutSupported");
    return true;
}

void
OSCControlAudioProcessor::
processBlock
(AudioBuffer<float>& buffer,
  MidiBuffer& midiMessages)
{
    auto mainInputOutput = getBusBuffer (buffer, true, 0);

    // add a hopefully inaudible float epsilon here to circumvent VST3
    // plugin auto-suspend (tested in Bitwig). This is ugly, let's
    // hope we can switch to CLAP soon or find a saner solution by
    // flagging the host that we don't want to get suspended via the
    // JUCE API.
    for (auto j = 0; j < buffer.getNumSamples(); ++j)
        for (auto i = 0; i < mainInputOutput.getNumChannels(); ++i)
            *mainInputOutput.getWritePointer (i, j) =
                *mainInputOutput.getReadPointer (i, j) +
                std::numeric_limits<float>::epsilon();
}

bool
OSCControlAudioProcessor::
hasEditor() const
{
    Logger::writeToLog("hasEditor");
    return hasUserInterface;
}

AudioProcessorEditor*
OSCControlAudioProcessor::
createEditor()
{
    Logger::writeToLog("createEditor");
    return new OSCControlAudioProcessorEditor
        (*this, dirPreset);
}

void
OSCControlAudioProcessor::
getStateInformation
(MemoryBlock& destData)
{
    Logger::writeToLog("getStateInformation");

    if (!hasUserInterface) {
        MemoryOutputStream stream (destData, true);

        stream.writeString (hashHostControls ());

        for (auto & control : listHostControls) {
            control->serialize (stream);
        }
    }
}

void
OSCControlAudioProcessor::
setStateInformation
(const void* data, int sizeInBytes)
{
    Logger::writeToLog("setStateInformation");

    if (!hasUserInterface && sizeInBytes > 0) {
        MemoryInputStream stream
            (data, static_cast<size_t> (sizeInBytes), false);

        auto hashHostControlsStored = stream.readString();
        auto hashHostControlsActive = hashHostControls ();

        if (hashHostControlsStored == hashHostControlsActive) {
            Logger::writeToLog ("restoring plugin state");
            for (auto & control : listHostControls) {
                control->deserialize (stream);
            }
        }
        else {
            Logger::writeToLog
                ("Host control hashes don't match, "
                 "not restoring plugin state.");
            Logger::writeToLog ("stored: " + hashHostControlsStored);
            Logger::writeToLog ("active: " + hashHostControlsActive);
        }
    }
}

String
OSCControlAudioProcessor::
hashHostControls()
{
    MemoryBlock memInput;

    for (auto & control : listHostControls) {
        auto createInfo = control->getCreateInfo ();

        auto charpName = createInfo.name.getCharPointer();
        memInput.append (charpName, sizeof (charpName));
        memInput.append (&createInfo.type, sizeof (createInfo.type));
        memInput.append (&createInfo.range, sizeof (createInfo.range));
    }

    return SHA256 (memInput).toHexString ();
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE
createPluginFilter()
{
    Logger::writeToLog("createPluginFilter");
    return new OSCControlAudioProcessor();
}
