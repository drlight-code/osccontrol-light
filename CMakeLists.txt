cmake_minimum_required(VERSION 3.15)

project(osccontrol-light VERSION 0.0.1)

find_package(JUCE CONFIG REQUIRED)

if(NOT PRESET_NAME)
    set(PRESET_NAME "light")
endif()

juce_add_plugin(osccontrol-light
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE drlight            # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE oscL                            # A unique four-character plugin id with at least one upper-case character
    PLUGIN_NAME "osccontrol-${PRESET_NAME}"
    FORMATS VST3 Standalone                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "osccontrol-${PRESET_NAME}")            # The name of the final executable, which can differ from the target name

juce_generate_juce_header(osccontrol-light)

target_sources(osccontrol-light PRIVATE
    Source/ControlContainer.cpp
    Source/ControlContainer.h
    Source/ControlElement.cpp
    Source/ControlElementFactory.cpp
    Source/ControlElementFactory.h
    Source/ControlElement.h
    Source/ControlElementHost.cpp
    Source/ControlElementHost.h
    Source/ControlElementKnob.cpp
    Source/ControlElementKnob.h
    Source/ControlElementToggle.cpp
    Source/ControlElementToggle.h
    Source/ControlElementUI.cpp
    Source/ControlElementUI.h
    Source/LayoutHints.cpp
    Source/LayoutHints.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PresetPage.cpp
    Source/PresetPage.h
    Source/PresetParser.cpp
    Source/PresetParser.h
    Source/UIComponentFactory.cpp
    Source/UIComponentFactory.h
    )

target_compile_definitions(osccontrol-light
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
    )

juce_add_binary_data(osccontrol-light-data SOURCES
    Resources/drlight-eyesclosed.png
    Resources/drlight-shout.png
    Resources/list-icon.png
    Resources/preset-folder-icon.png
    Resources/reset-icon.png
    Resources/SourceCodePro-Regular.otf
    )
set_target_properties(osccontrol-light-data PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(osccontrol-light PRIVATE
    osccontrol-light-data
    juce::juce_audio_utils
    juce::juce_osc
    juce::juce_cryptography
    yaml-cpp
    )
