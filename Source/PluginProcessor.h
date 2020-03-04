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

#pragma once

#include <list>

#include <JuceHeader.h>

class ControlElementHost;
using ControlElementHostUnique =
    std::unique_ptr<ControlElementHost>;

class OSCControlAudioProcessor :
  public AudioProcessor
{
public:
    OSCControlAudioProcessor();
    ~OSCControlAudioProcessor();

    File locatePresetFile(String namePreset);
    void initializeHeadless(File filePreset);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:

    String hashHostControls();

    std::unique_ptr<FileLogger> fileLogger;

    bool hasUserInterface;

    std::list<ControlElementHostUnique> listHostControls;
    std::unique_ptr<OSCSender> oscSender;

    File dirPreset;
    String filenamePlugin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCControlAudioProcessor)
};
