cmake_minimum_required(VERSION 3.11)

project(osccontrol-light VERSION 0.0.1)

find_package(JUCE CONFIG REQUIRED)
find_package(yaml-cpp REQUIRED)

function(add_osccontrol_target TARGET FORMATS)
    juce_add_plugin(${TARGET}
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME drlight                        # Specify the name of the plugin's author
        # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
        # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
        # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
        # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
        # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE drLt               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE oscL                            # A unique four-character plugin id with at least one upper-case character
        PLUGIN_NAME ${TARGET}
        FORMATS ${FORMATS}                          # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        PRODUCT_NAME ${TARGET})                     # The name of the final executable, which can differ from the target name
endfunction()

function(configure_osccontrol_target TARGET)
    juce_generate_juce_header(${TARGET})

    target_sources(${TARGET} PRIVATE
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

    target_compile_definitions(${TARGET}
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_MODAL_LOOPS_PERMITTED=1 # allow modal loops for now to enable file chooser dialog
        YAML_CPP_STATIC_DEFINE=1
        )

    target_link_libraries(${TARGET} PRIVATE
        osccontrol-light-data
        juce::juce_audio_utils
        juce::juce_osc
        juce::juce_cryptography
        yaml-cpp
        )
endfunction()

# create the binary data target that is shared by all plugin instances
juce_add_binary_data(osccontrol-light-data SOURCES
    Resources/drlight-eyesclosed.png
    Resources/drlight-shout.png
    Resources/list-icon.png
    Resources/preset-folder-icon.png
    Resources/reset-icon.png
    Resources/SourceCodePro-Regular.otf
    )
set_target_properties(osccontrol-light-data PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# we always build the interactive gui version of the plugin as VST3 and standalone application
message("creating build target for osccontrol-light-gui standalone application")
message("creating build target for osccontrol-light-gui VST3 plugin")
add_osccontrol_target(osccontrol-light-gui "Standalone;VST3")
configure_osccontrol_target(osccontrol-light-gui)

set(PRESET_NAMES "" CACHE INTERNAL "list of presets for which a plugin is built")
foreach(PRESET_NAME ${PRESET_NAMES})
    message("creating build target for ${PRESET_NAME} preset VST3 plugin")
    set(PRESET_TARGET "osc-${PRESET_NAME}")
    add_osccontrol_target(${PRESET_TARGET} "VST3")
    configure_osccontrol_target(${PRESET_TARGET})
endforeach()
