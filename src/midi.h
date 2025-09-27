#pragma once
#include <alsa/asoundlib.h>

bool init_midi();
void close_midi();
void send_cc(int cc, int value);

