//
// Ringing Tones Text Transfer Language (RTTTL) player
//
#include <os.h>

int notefreq[12] = {4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902};

int note2freq(int note) {
    return notefreq[note % 12] / (1 << (9 - (note / 12)));
}

void play(char *song) {
    char *p = song;
    int defdur = 4;
    int defscale = 6;
    int bpm = 63;
    int silence = 0;

    // Skip name
    while (*p && *p != ':') p++;
    if (!*p) return;
    p++;

    // Parse defaults
    while (*p) {
        char param;
        int value;

        while (*p == ' ') p++;
        if (!*p) return;
        if (*p == ':') break;

        param = *p++;
        if (*p != '=') return;

        p++;
        value = 0;
        while (*p >= '0' && *p <= '9') value = value * 10 + (*p++ - '0');

        switch (param) {
            case 'd':
                defdur = 32 / value;
                break;
            case 'o':
                defscale = value;
                break;
            case 'b':
                bpm = value;
                break;
        }

        while (*p == ' ') p++;
        if (*p == ',') p++;
    }
    p++;

    while (*p) {
        int note = -1;
        int scale = defscale;
        int dur = defdur;
        int ms;
        int freq;

        // Skip whitespace
        while (*p == ' ') p++;
        if (!*p) return;

        // Parse duration
        if (*p >= '0' && *p <= '9') {
            int value = 0;
            while (*p >= '0' && *p <= '9') value = value * 10 + (*p++ - '0');

            dur = 32 / value;
        }

        // Parse note
        switch (*p) {
            case 0:
                return;
            case 'C':
            case 'c':
                note = 0;
                break;
            case 'D':
            case 'd':
                note = 2;
                break;
            case 'E':
            case 'e':
                note = 4;
                break;
            case 'F':
            case 'f':
                note = 5;
                break;
            case 'G':
            case 'g':
                note = 7;
                break;
            case 'A':
            case 'a':
                note = 9;
                break;
            case 'H':
            case 'h':
                note = 11;
                break;
            case 'B':
            case 'b':
                note = 11;
                break;
            case 'P':
            case 'p':
                note = -1;
                break;
        }
        p++;
        if (*p == '#') {
            note++;
            p++;
        }
        if (*p == 'b') {
            note--;
            p++;
        }

        // Parse special duration
        if (*p == '.') {
            dur += dur / 2;
            p++;
        }

        // Parse scale
        if (*p >= '0' && *p <= '9') scale = (*p++ - '0');

        // Parse special duration (again...)
        if (*p == '.') {
            dur += dur / 2;
            p++;
        }

        // Skip delimiter
        while (*p == ' ') p++;
        if (*p == ',') p++;

        // Play note
        ms = dur * 60000 / (bpm * 8);
        if (note == -1) {
            freq = 0;
        } else {
            freq = note2freq((scale + 1) * 12 + note);
        }

        if (freq) {
            ioctl(1, IOCTL_SOUND, &freq, 4);
            msleep(ms * 7 / 8);
            ioctl(1, IOCTL_SOUND, &silence, 4);
            msleep(ms / 8);
        } else {
            msleep(ms);
        }
    }
}
