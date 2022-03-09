# Orbbec Astra Depth Sensor Recording Tool

This software records the RGB and depth frames captured with Orbbec depth sensors.
Depth is measured in millimeters and is encoded as a single channel of uint16.  

## Getting started

0. Make sure the host system see the camera when it is plugged.
`lsusb` should have two new lines without manufacturer information.
If you don't see any change in the table the problem might be the cable.
Make sure if cable is of good quality and can transfer data and enough power.
1. In case the host system needs driver: `sudo install.sh` and reboot
2. `mkdir data`
3. `xhost +local:docker`. Grant graphical access to docker  
4. `docker-compose up`

Press `BACKSPACE`, `ESC`, `SPACE` or `q` when video is in focus to stop the program.
Avoid `Ctrl+C` as drivers might not be released properly.

## Astra SDK (v2.1.0)

### Supported systems
* Windows 7, 8 and 10, 32-bit and 64-bit
* Ubuntu 14.04, 64-bit
* Ubuntu 16.04, 64-bit, arm and aarch64
* Ubuntu 18.04, 64-bit, arm and aarch64
* Ubuntu 20.04, 64-bit, arm and aarch64
* Android 4.4.2 and 5.1 (armeabi-v7a 32-bit, arm64-v8a)
* Orbbec Persee
* raspbian for Raspberry Pi 4b

### Supported Orbbec Camera Models
* Astra
* Astra nh
* Astra pro
* Astra pro nh
* Astra mini
* Astra s
* Astra pro plus
* Astra Deeyea (Astra Embedded S)
* Astra DaBai
* Astra Stereo S
* Gemini (Astra Stereo S (U3))
* Projector (Astra SL1200L(S_C/S_CF))

### Possibly working systems - these may work but are untested/unsupported at this time
* Android 6.0, Android 7.0, Android 8.1. **Does not seems to work an Android +9.0**
* Unity3D 2017, Unity3D 2018, Unity 4.7

## Known Issues

* It's not possible to connect several sensors to the same machine due to drivers issues.

* There is no error message if no sensor is found or plugged in. (But, it doesn't crash!)
  For some devices hot-plugging (plugging in or removing the sensor while an app is running) is not currently supported.

* If the code crashes or you forcibly terminate an app that uses Astra SDK before it is given a chance to
  properly shutdown the Astra sensor, the sensor driver may exhibit strange behavior.
  To FIX, simply unplug and re-plug the sensor into the USB port. (This is primarily an OpenNI issue.)
