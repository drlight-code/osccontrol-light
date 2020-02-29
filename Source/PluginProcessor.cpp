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

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <dlfcn.h>

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
    fileLogger(File("~/.oscsend-vst.log"), "oscsend-vst debug log", 0),
    hasUserInterface(true)
{
    Logger::setCurrentLogger(&fileLogger);
    Logger::writeToLog("OscsendvstAudioProcessor");

    auto filename = juce_getExecutableFile();
    Logger::writeToLog(filename.getFullPathName());

    pathPreset =
        File(SystemStats::getEnvironmentVariable("OSCSEND_PRESET_PATH", ""));

    auto namePlugin = filename.getFileNameWithoutExtension();
    if(namePlugin.startsWith("oscsend-vst")) {
        auto presetToLoad =
            namePlugin.fromFirstOccurrenceOf
            ("oscsend-vst", false, false).trimCharactersAtStart("-");

        if(presetToLoad.isNotEmpty()) {
            Logger::writeToLog("PRESET NAME: " + presetToLoad);

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
    Logger::writeToLog("~OscsendvstAudioProcessor");
}

File
OscsendvstAudioProcessor::
locatePresetFile
(String namePreset)
{
    auto filenamePreset = namePreset + ".yaml";
    auto files = pathPreset.findChildFiles
        (File::findFiles, true, filenamePreset);

    if(files.isEmpty()) {
        Logger::writeToLog
            ("error: could not locate preset file: " + filenamePreset);
        // TODO freak out!!
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

    // initialize DAW controls from so-named preset
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
        (*this, pathPreset);
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
