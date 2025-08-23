# Stalks Remapper

[![CMake on a single platform][badge]][badge-status]

[MOZA Multi-functional stalks][moza-stalks] and [MOZA HBP Handbrake][moza-hbp]
are expensive controllers
and [City Car Driving][city-car-driving] is a niche driving simulator
with a lot of possibilities and VR support.

Regrettably, neither the game developers nor the hardware vendor
do anything to fix the compatibility.

## How to use

**Do not launch the `stalks_remapper.exe` until the game map is loaded.**

The remapper uses XBox 360 gamepad emulator, which supports force feedback.
When [City Car Driving][city-car-driving] initialises force feedback
and detects this virtual gamepad,
the game may use this device as the only force feedback receiver.
This means that, if your steering wheel supports force feedback,
you may not feed it if `stalks_remapper.exe` was running
when the game level was loading.

TL;DR: launch `stalks_remapper.exe` only when the game level is loaded,
and you see your virtual car's cockpit.
There is no need to relaunch the remapper when you switch to another level.

### Setup

1. [Build](#how-to-build) or [download][releases]
   (scroll to the "Assets" section of the latest release to find the executable)
   `stalks_remapper.exe`.
2. Install [ViGEmBus] and restart your computer.
   This is a driver that allows to create a virtual joystick
   the remapper uses to remap the controller keys.
3. Install [HidHide].
4. If you did not connect your
   [MOZA Multi-functional stalks][moza-stalks]
   and [MOZA HBP Handbrake][moza-hbp], connect them now.
   The handbrake must be connected directly via USB.
   Set the handbrake not as an axis but as a button
   in your [MOZA Pit House][moza-pit-house].
5. In [HidHide], either hide the controllers
   from [City Car Driving][city-car-driving]
   (recommended because will not affect the other games)
   or whitelist the `stalks_remapper.exe` (and other games) to use them.

### Play

1. Launch the game.
2. Set up the desired controls (you can do this anytime).
3. Ensure all your levers are in default position.
   You must perform this check before starting/restarting any challenge
   of free driving mode.
   If you forget to do this, press `Esc` in the game
   and set your levers to the default position.
4. Load a challenge or a free driving mode.
5. Launch `stalks_remapper.exe`.
   Do not exit the game while launching the remapper.
   Simply `Alt-Tab` or `Win` from the game to your OS,
   launch `stalks_remapper.exe`, and return to the game.

If you turn off the game, turn off the remapper as well.
If you finish a challenge or free drive and wish to switch to another challenge,
there is **no need** to stop the remapper.

## Features

### Supported keys

- Turning indicators (blinkers): left, right, cancel.
- Light modes: off, park, main.
- Far light: on, off, horn (pull the lever to blink).
- Wipers: off, intermittent, slow, fast, mist
  (hold the lever down to keep the wipers on for a while).
- Handbrake: on, off.
- Cruise control: on/off, increase/decrease.
  Not supported by the game, so can be mapped to something else.

### Mapping

If you wish to check whether the application works right,
these are the buttons that must be emitted
when you push the corresponding levers of your
[MOZA Multi-functional stalks][moza-stalks]
or [MOZA HBP Handbrake][moza-hbp]

- light: D-pad up,
- far light: D-pad down,
- left turn indicator: D-pad left,
- right turn indicator: D-pad right,
- wiper: Start,
- handbrake: Back,
- enable cruise control: A,
- cruise control increase speed: B,
- cruise control decrease speed: X,
- disable cruise control: Y.

[City Car Driving][city-car-driving] allows only one button
to be mapped per each of the following functions:
light, left turn indicator, right turn indicator, and wiper.
This means that, in order to switch from "off" to "slow" wiper mode,
the button must be clicked twice, and `stalks_remapper.exe` takes care of this.
To switch from "slow" to "off", the game needs two more clicks.

## Known issues

### Force feedback priority

If `stalks_remapper.exe` is launched before a game map is loaded,
the game is likely to send force feedback only to the virtual gamepad
instead of your physical steering wheel.
In this case, quit the game (close it completely), stop the remapper,
launch the game again, run a level,
and start the remapper when you see your virtual cockpit
(when the level starts).

This problem is caused by the limitations of the [ViGEmBus]
and the [City Car Driving][city-car-driving]:
- [ViGEmBus] can simulate two gamepad, both supporting force feedback.
  I do not know how to turn this feature off without altering [ViGEmBus].
- [City Car Driving][city-car-driving] seems to use
  only one force feedback sink.
  I do not know how to make it choosing the right device.

### Stalks state is not reset automatically

If you launch a [City Car Driving][city-car-driving] level
and your [MOZA Multi-functional stalks][moza-stalks] levers
are not in the default position (everything is off),
the `stalks_remapper.exe` has no way to know that you just launched the game
because the game does not share telemetry data.
It is the user's responsibility to ensure all stalks are in the default position
once you start a game level.
If you forgot to do this and already started a level,
press `Esc` and set the levers to the proper positions.

Suppose your park light is active and you restart a level.
Then, you notice it and return the stalk to "off" position.
The game will receive two "light" signals and will switch to "main" light.
Press `Esc` to see the game menu
and switch the lever to activate the main light on your controller.
Press `Esc` again &mdash; your turn signal lever
is now synchronised with the game state.

### Game glitched and stalks state is not synchronised anymore

The game may miss signals during glitches.
If it happens, press `Esc` to call the menu
and synchronise the stalks state with the game state.

Suppose the game glitched for 0.1 second,
and you turned on the left turn signal during this time.
Then, you reset the stalk when the game glitch finished.
The left turn indicator will activate in the game.
Press `Esc` to see the game menu
and push the lever to activate the left turn signal on your controller.
Press `Esc` again &mdash; your turn signal lever
is now synchronised with the game state.

## Alternative

This repository is not the first community attempt
to make the controller working in the game.
There is already a [Python script][python-remapper],
which requires [vJoy] and [Joystick Gremlin][joystick-gremlin].

The solution in this repository requires [ViGEmBus] and [HidHide].
It is more low-level and thus complicated, but may be faster.

The projects both solutions rely on are technically dead.
However, they are downloadable and working.

## How to build

The project uses [submodules][git-submodule].
Make sure you clone it right:
```shell
git clone --recursive https://github.com/definability/stalks_remapper
```

Nothing special is required to build the project.
The ordinary [Visual Studio Build Tools][vs-build-tools] suffices.

The project is CMake-based.
You can use an IDE that supports such projects or build it manually:
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build buid
```

The executable should be available at `build/stalks_remapper.exe`.

[badge]: https://github.com/definability/stalks_remapper/actions/workflows/cmake-single-platform.yml/badge.svg
[badge-status]: https://github.com/definability/stalks_remapper/actions/workflows/cmake-single-platform.yml
[city-car-driving]: https://store.steampowered.com/app/493490/City_Car_Driving/
[CMake]: https://cmake.org/download/
[git-submodule]: https://git-scm.com/book/en/v2/Git-Tools-Submodules
[HidHide]: https://github.com/nefarius/HidHide
[joystick-gremlin]: https://whitemagic.github.io/JoystickGremlin/
[moza-stalks]: https://uk.mozaracing.com/products/multi-function-stalks
[moza-hbp]: https://mozaracing.com/products/hbp-handbrake
[moza-pit-house]: https://mozaracing.com/pages/pit-house
[python-remapper]: https://steamcommunity.com/app/493490/discussions/0/599650845622041126/
[releases]: https://github.com/definability/stalks_remapper/releases
[ViGEmBus]: https://github.com/nefarius/ViGEmBus
[vJoy]: https://sourceforge.net/projects/vjoystick/
[vs-build-tools]: https://visualstudio.microsoft.com/visual-cpp-build-tools/
