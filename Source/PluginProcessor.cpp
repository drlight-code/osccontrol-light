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

OscsendvstAudioProcessor::
OscsendvstAudioProcessor()
    : AudioProcessor (BusesProperties())
{
    // parameter = new AudioParameterFloat
    //     ("speed", "Speed", 0.0f, 1.0f, 0.5f);
    // addParameter (parameter);
}

OscsendvstAudioProcessor::
~OscsendvstAudioProcessor()
{
}

const String
OscsendvstAudioProcessor::
getName() const
{
    return "oscsend";
}

bool
OscsendvstAudioProcessor::
acceptsMidi() const
{
  return false;
}

bool
OscsendvstAudioProcessor::
producesMidi() const
{
  return false;
}

double
OscsendvstAudioProcessor::
getTailLengthSeconds() const
{
    return 0.0;
}

int
OscsendvstAudioProcessor::
getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell
                // them there are 0 programs, so this should be at
                // least 1, even if you're not really implementing
                // programs.
}

int
OscsendvstAudioProcessor::
getCurrentProgram()
{
    return 0;
}

void
OscsendvstAudioProcessor::
setCurrentProgram
(int index)
{
}

const String
OscsendvstAudioProcessor::
getProgramName
(int index)
{
    return {};
}

void
OscsendvstAudioProcessor::
changeProgramName
(int index, const String& newName)
{
}

void
OscsendvstAudioProcessor::
prepareToPlay
(double sampleRate, int samplesPerBlock)
{
}

void
OscsendvstAudioProcessor::
releaseResources()
{
}

bool
OscsendvstAudioProcessor::
isBusesLayoutSupported
(const BusesLayout& layouts) const
{
    return true;
}

void
OscsendvstAudioProcessor::
processBlock
(AudioBuffer<float>& buffer,
  MidiBuffer& midiMessages)
{
}

bool
OscsendvstAudioProcessor::
hasEditor() const
{
    return true;
}

AudioProcessorEditor*
OscsendvstAudioProcessor::
createEditor()
{
    return new OscsendvstAudioProcessorEditor (*this);
}

void
OscsendvstAudioProcessor::
getStateInformation
(MemoryBlock& destData)
{
    // You should use this method to store your parameters in the
    // memory block.  You could do that either as raw data, or use the
    // XML or ValueTree classes as intermediaries to make it easy to
    // save and load complex data.
//    MemoryOutputStream (destData, true).writeFloat (*parameter);
}

void
OscsendvstAudioProcessor::
setStateInformation
(const void* data, int sizeInBytes)
{
    // parameter->setValueNotifyingHost
    //     (MemoryInputStream
    //         (data, static_cast<size_t> (sizeInBytes), false).readFloat());
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE
createPluginFilter()
{
    return new OscsendvstAudioProcessor();
}
