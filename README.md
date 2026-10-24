# arduino-code

The firmware of the [solar cooker](https://github.com/solar-cooker-UHasselt) testing
station. It reads the sensors, writes every measurement to a CSV file on a microSD card
and shows the values on an LCD:

- the temperature in three pots (PT100 probes on MAX31865 boards)
- the solar irradiance (pyranometer)
- the wind speed (anemometer)
- the outside temperature (AM2315C)
- the air pressure and humidity inside the box (BME680)
- the date and time for each measurement (DS3231 clock)

The CSV format and the folder layout on the card are in
[src/main/README.md](src/main/README.md).

## Which board

| Branch | Board | Status |
| ------ | ----- | ------ |
| `main` | Arduino Mega 2560 | the firmware used so far |
| `dev`  | [Arduino UNO R4 WiFi](https://docs.arduino.cc/hardware/uno-r4-wifi/) | for the [testing station shield](https://github.com/solar-cooker-UHasselt/kicad-testing-station), not merged yet: it still needs testing on the hardware |

The board is set in [platformio.ini](platformio.ini). Everything below works the same on
both branches. The Mega version stays available as the tag `mega-2560`
(`git checkout mega-2560`), also once `dev` is merged.

## Installation

### Requirements

- [Visual Studio Code](https://code.visualstudio.com/) with the
  [PlatformIO extension](https://platformio.org/install/ide?install=vscode), or only
  [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html)
  for the command line. PlatformIO installs the compiler and the libraries itself, the
  first build takes a while.
- [git](https://git-scm.com), to clone the repo
- A USB cable for the board

### Clone

```bash
git clone https://github.com/solar-cooker-UHasselt/arduino-code.git
```

For the UNO R4 WiFi, switch to `dev` after cloning:

```bash
git switch dev
```

## Usage

In VS Code, open the cloned folder (the one with `platformio.ini`). The PlatformIO icon
on the left lists the project tasks per environment: under `main`, Build, Upload and
Monitor.

The same from the command line:

```bash
pio run -e main                 # build, ends with SUCCESS
pio run -e main -t upload       # build and flash the board over USB
pio device monitor              # serial output, at the monitor_speed in platformio.ini
```

To test one sensor on its own, each component has its own environment (`AM2315C`,
`anemometer`, `BME680`, `clock`, `GPS`, `LCD`, `microSD`, `Pt100`, `pyranometer`,
`switch`), for example:

```bash
pio run -e BME680 -t upload
```

The code and the wiring diagram for each are in
[src/components/](src/components/). The full station joins the components in
[src/main/](src/main/), one file per sensor and [main.cpp](src/main/main.cpp) calling
them.

## Contributing

- Work on a branch and open a pull request to `main`. It merges once a code owner
  (listed in [.github/CODEOWNERS](.github/CODEOWNERS)) approves it. `main` accepts no
  force pushes.

## Authors and acknowledgment

Written by Lowie Van Vyve, Kato Warson and Ruben Godde for the UHasselt solar cooker
project. The sensors are read with the Adafruit, RTClib and SdFat libraries listed in
[platformio.ini](platformio.ini).
