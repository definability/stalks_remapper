cmake_minimum_required(VERSION 3.31)
project(stalks_remapper)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(ViGEmClient)

include_directories(include)
add_executable(stalks_remapper src/main.cpp src/gamepad_state.cpp
        src/direct_input_wrapper.cpp
        src/joystick.cpp src/button_state.cpp
        src/gamepad_emulator.cpp
)
target_link_libraries(stalks_remapper PRIVATE ViGEmClient setupapi dinput8 dxguid)
