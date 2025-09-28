#pragma once
#include <alsa/asoundlib.h>

extern bool verbose;
bool init_midi();
void close_midi();
void send_cc(int cc, int value);
void send_pc(int pc);

