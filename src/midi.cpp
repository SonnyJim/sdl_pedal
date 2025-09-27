#include "midi.h"
#include <iostream>

snd_seq_t *seq;
int out_port;

bool init_midi() {
    if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_OUTPUT, 0) < 0) return false;
    snd_seq_set_client_name(seq, "SDL2 MIDI Buttons");
    out_port = snd_seq_create_simple_port(seq, "buttons_out",
                SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                SND_SEQ_PORT_TYPE_APPLICATION);
    return out_port >= 0;
}

void close_midi() { snd_seq_close(seq); }

void send_cc(int cc, int value) {
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, out_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    ev.type = SND_SEQ_EVENT_CONTROLLER;
    ev.data.control.channel = 0;
    ev.data.control.param = cc;
    ev.data.control.value = value;
    snd_seq_event_output(seq,&ev);
    snd_seq_drain_output(seq);
}

