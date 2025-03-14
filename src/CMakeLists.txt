if(DEFINED ENV{AAX_SDK_PATH})
    set(AAX_SDK_PATH $ENV{AAX_SDK_PATH})
endif()
if(DEFINED AAX_SDK_PATH)
    set(AAX_BUILD_FLAG AAX)
    file(TO_CMAKE_PATH "${AAX_SDK_PATH}" AAX_SDK_CMAKE_PATH)
    message(STATUS "AAX Build is enabled. AAX SDK = " ${AAX_SDK_CMAKE_PATH})
    juce_set_aax_sdk_path(${AAX_SDK_CMAKE_PATH})
else()
    message(STATUS "AAX Build is disabled. To enable, set AAX_SDK_PATH to your environment variable or specify -DAAX_SDK_PATH to CMake configure option.")
endif()

juce_add_plugin(ExamplePlugin
    # VERSION ...                                     # Set this if the plugin version is different to the project version
    # ICON_BIG ""   # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    #   ICON_SMALL "${CMAKE_SOURCE_DIR}/Assets/icon16.png " 
    COMPANY_NAME "lsAudioDesign"                    # Specify the name of the plugin's author
    COMPANY_COPYRIGHT "lsAudioDesign"
    COMPANY_EMAIL "liamsmyth2002@gmail.com"
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE CCCC               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE XXXX                            # A unique four-character plugin id with at least one upper-case character
    FORMATS 
    # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
            Standalone 
            AU
            # AUv3 
            VST3 
            # Unity 
            ${AAX_BUILD_FLAG}               
    VST3_CATEGORIES "Fx" 
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    # AU_SANDBOX_SAFE TRUE
    # AAX_CATEGORY ""
    # HARDENED_RUNTIME_ENABLED # macOS app settings
    # HARDENED_RUNTIME_OPTIONS
    # APP_SANDBOX_ENABLED
    # APP_SANDBOX_INHERIT
    # DESCRIPTION "" #when i set this option, the name of AAX plugin became this description(bug?)
    MICROPHONE_PERMISSION_ENABLED TRUE
    MICROPHONE_PERMISSION_TEXT "This applicaion requires a permission to use an audio input device of your computer. By Default, Built-In microphone will be used."

    PRODUCT_NAME "Euclidean Gate")        # The name of the final 

target_compile_features(ExamplePlugin PUBLIC cxx_std_17)


target_compile_definitions(ExamplePlugin
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
    # JUCE_DISPLAY_SPLASH_SCREEN=0 #if your plugin is distributed with GPL license or paid
)

juce_add_binary_data(JuceLogoBinary
SOURCES 
${CMAKE_SOURCE_DIR}/assets/juce-logo.svg
)

target_sources(ExamplePlugin PRIVATE
    PluginEditor.cpp
    PluginProcessor.cpp
    LookAndFeel.cpp
    Gate.cpp
    SequencerUI.cpp
)

target_link_libraries(ExamplePlugin PUBLIC
JuceLogoBinary
juce::juce_audio_basics
juce::juce_audio_devices
juce::juce_audio_formats
juce::juce_audio_plugin_client
juce::juce_audio_processors
juce::juce_audio_utils
juce::juce_core
juce::juce_cryptography
juce::juce_data_structures
juce::juce_dsp
juce::juce_events
juce::juce_graphics
juce::juce_gui_basics
juce::juce_gui_extra
juce::juce_opengl
juce::juce_product_unlocking
)

juce_generate_juce_header(ExamplePlugin)
