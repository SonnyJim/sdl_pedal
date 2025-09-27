# SDL2 Pedal Controller

SDL2 app to control **mod-host** on PatchboxOS via a 3.5" TFT touchscreen.  
It provides virtual buttons and sliders that send MIDI CC messages over ALSA to control parameters and effects.

---


## Requirements

Make sure the following libraries are installed (Debian/Raspberry Pi OS style packages):

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  libsdl2-dev \
  libsdl2-ttf-dev \
  libasound2-dev
