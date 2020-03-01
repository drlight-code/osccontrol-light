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

#include <dlfcn.h>

#include "PresetParser.h"

#include "ControlElementHost.h"
#include "ControlElementFactory.h"

#include "PluginEditor.h"

#include "PluginProcessor.h"

namespace  {
File juce_getExecutableFile();
File juce_getExecutableFile()
{
  struct DLAddrReader
  {
    static String getFilename()
    {
      Dl_info exeInfo;

      auto localSymbol = (void*) juce_getExecutableFile;
      dladdr (localSymbol, &exeInfo);
      return CharPointer_UTF8 (exeInfo.dli_fname);
    }
  };

  static String filename = DLAddrReader::getFilename();
  return File::getCurrentWorkingDirectory().getChildFile (filename);
}
}

OscsendvstAudioProcessor::
OscsendvstAudioProcessor() :
    AudioProcessor (BusesProperties()),
    hasUserInterface(true)
{
    auto filePlugin = juce_getExecutableFile();
    auto filenamePlugin = filePlugin.getFileNameWithoutExtension();
    auto filenameLog = filenamePlugin + ".log";
    std::cout << filenameLog.toStdString() << std::endl;
    fileLogger = std::make_unique<FileLogger>
        (filePlugin.getParentDirectory().getChildFile(filenameLog),
         "oscsend-vst debug log", 0);

    Logger::setCurrentLogger(fileLogger.get());
    Logger::writeToLog(filePlugin.getFullPathName());
    Logger::writeToLog("OscsendvstAudioProcessor");

    auto pathPreset =
        SystemStats::getEnvironmentVariable("OSCSEND_PRESET_PATH", "");
    dirPreset = File (pathPreset);

    if(filenamePlugin.startsWith("oscsend-vst")) {
        auto presetToLoad =
            filenamePlugin.fromFirstOccurrenceOf
            ("oscsend-vst", false, false).trimCharactersAtStart("-");

        if(presetToLoad.isNotEmpty()) {
            Logger::writeToLog("PRESET NAME: " + presetToLoad);

            // if (pathPreset == "") {
            //     AlertWindow::showMessageBox
            //         (AlertWindow::AlertIconType::WarningIcon,
            //             "Error loading oscsend-vst",
            //             "When running oscsend-vst in headless mode "
            //             "for DAW integration, make sure that "
            //             "OSCSEND_PRESET_PATH is set properly in "
            //             "the environment!");
            //     // TODO freak out!!
            // }

            auto presetFile = locatePresetFile(presetToLoad);

            hasUserInterface = false;
            initializeHeadless(presetFile);
        }
    }
    else {
        // TODO freak out!!
    }
}

OscsendvstAudioProcessor::
~OscsendvstAudioProcessor()
{
    Logger::writeToLog ("~OscsendvstAudioProcessor");
    Logger::setCurrentLogger (nullptr);
}

File
OscsendvstAudioProcessor::
locatePresetFile
(String namePreset)
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
OscsendvstAudioProcessor::
initializeHeadless
(File filePreset)
{
    Logger::writeToLog("initializeHeadless");

    PresetParser preset (filePreset);
    ControlElementFactory factory (*oscSender);

    for(auto control : preset.getControlElements ()) {

        auto createInfo = preset.getControlElementCreateInfo (control);
        auto element = factory.createControlElementHost (createInfo, *this);

        listHostControls.push_back (std::move (element));
    }
}

const String
OscsendvstAudioProcessor::
getName() const
{
    Logger::writeToLog("getName");
    return "oscsend";
}

bool
OscsendvstAudioProcessor::
acceptsMidi() const
{
    Logger::writeToLog("acceptsMidi");
    return false;
}

bool
OscsendvstAudioProcessor::
producesMidi() const
{
    Logger::writeToLog("producesMidi");
    return false;
}

double
OscsendvstAudioProcessor::
getTailLengthSeconds() const
{
    Logger::writeToLog("getTailLengthSeconds");
    return 0.0;
}

int
OscsendvstAudioProcessor::
getNumPrograms()
{
    // Logger::writeToLog("getNumPrograms");
    return 1;   // NB: some hosts don't cope very well if you tell
                // them there are 0 programs, so this should be at
                // least 1, even if you're not really implementing
                // programs.
}

int
OscsendvstAudioProcessor::
getCurrentProgram()
{
    // Logger::writeToLog("getCurrentProgram");
    return 0;
}

void
OscsendvstAudioProcessor::
setCurrentProgram
(int index)
{
    Logger::writeToLog("setCurrentProgram");
}

const String
OscsendvstAudioProcessor::
getProgramName
(int index)
{
    Logger::writeToLog("getProgramName");
    return {};
}

void
OscsendvstAudioProcessor::
changeProgramName
(int index, const String& newName)
{
    Logger::writeToLog("changeProgramName");
}

void
OscsendvstAudioProcessor::
prepareToPlay
(double sampleRate, int samplesPerBlock)
{
    Logger::writeToLog("prepareToPlay");
}

void
OscsendvstAudioProcessor::
releaseResources()
{
    Logger::writeToLog("releaseResources");
}

bool
OscsendvstAudioProcessor::
isBusesLayoutSupported
(const BusesLayout& layouts) const
{
    Logger::writeToLog("isBusesLayoutSupported");
    return true;
}

void
OscsendvstAudioProcessor::
processBlock
(AudioBuffer<float>& buffer,
  MidiBuffer& midiMessages)
{
//    Logger::writeToLog("processBlock");
}

bool
OscsendvstAudioProcessor::
hasEditor() const
{
    Logger::writeToLog("hasEditor");
    return hasUserInterface;
}

AudioProcessorEditor*
OscsendvstAudioProcessor::
createEditor()
{
    Logger::writeToLog("createEditor");
    return new OscsendvstAudioProcessorEditor
        (*this, dirPreset);
}

void
OscsendvstAudioProcessor::
getStateInformation
(MemoryBlock& destData)
{
    Logger::writeToLog("getStateInformation");
    // You should use this method to store your parameters in the
    // memory block.  You could do that either as raw data, or use the
    // XML or ValueTree classes as intermediaries to make it easy to
    // save and load complex data.

    // MemoryOutputStream (destData, true).writeString("test");
}

void
OscsendvstAudioProcessor::
setStateInformation
(const void* data, int sizeInBytes)
{
    Logger::writeToLog("setStateInformation");
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE
createPluginFilter()
{
    Logger::writeToLog("createPluginFilter");
    return new OscsendvstAudioProcessor();
}
