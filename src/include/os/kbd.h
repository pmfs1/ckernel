//
// Keyboard driver
//
#ifndef KBD_H
#define KBD_H

#define KBSTATE_NORMAL          0
#define KBSTATE_SHIFT           1
#define KBSTATE_CTRL            2
#define KBSTATE_ALT             3
#define KBSTATE_NUMLOCK         4
#define KBSTATE_CAPSLOCK        5
#define KBSTATE_SHIFTCAPS       6
#define KBSTATE_SHIFTNUM        7
#define KBSTATE_ALTGR           8
#define KBSTATE_SHIFTCTRL       9

#define MAX_KBSTATES            10

#define MAX_SCANCODES           0x80
#define MAX_KEYTABLES           16

struct keytable {
    char *name;
    unsigned short keys[MAX_SCANCODES][MAX_KBSTATES];
};

extern int ctrl_alt_del_enabled;
extern int keymap;

krnlapi struct keytable *keytables[MAX_KEYTABLES];

void init_keyboard(dev_t devno, int reset);

int change_keyboard_map_id(int id);

int getch(unsigned int timeout);

int kbhit();

void kbd_reboot();

#endif
