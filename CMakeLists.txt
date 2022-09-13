cmake_minimum_required(VERSION 3.12)

project(2048 VERSION 1.7.0)

find_package(JUCE CONFIG REQUIRED)

juce_add_gui_app(2048
  VERSION 1.7.0
  COMPANY_NAME ""
  PRODUCT_NAME "2048"
  MICROPHONE_PERMISSION_ENABLED True)

juce_generate_juce_header(2048)

target_sources(2048
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/GameComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Main.cpp
)

set(project_target_compile_definitions
  JUCE_WEB_BROWSER=0 # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
  JUCE_USE_CURL=0 # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
  JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:2048,JUCE_PRODUCT_NAME>"
  JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:2048,JUCE_VERSION>"
  JUCE_DISPLAY_SPLASH_SCREEN=0

  JUCE_MODAL_LOOPS_PERMITTED=1
)

target_compile_definitions(2048
  PRIVATE
  ${project_target_compile_definitions}
)

target_link_libraries(2048
  PRIVATE
    juce::juce_gui_extra
    juce::juce_audio_devices
    juce::juce_audio_utils
  PUBLIC
)
