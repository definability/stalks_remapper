cmake_minimum_required(VERSION 3.31)
project(stalks_remapper LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(ViGEmClient)

include_directories(include)
add_executable(stalks_remapper
        src/main.cpp
        src/button_state.cpp
        src/direct_input_wrapper.cpp
        src/gamepad_emulator.cpp
        src/gamepad_state.cpp
        src/joystick.cpp
)
target_link_libraries(stalks_remapper PRIVATE ViGEmClient setupapi dinput8 dxguid)
