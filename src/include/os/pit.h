//
// Programmable Interval Timer functions (PIT i8253)
//
#ifndef PIT_H
#define PIT_H

#define TIMER_FREQ      100

#define TICKS_PER_SEC   TIMER_FREQ
#define CLOCKS_PER_SEC  1000

#define HZ TICKS_PER_SEC

#define CLOCKS_PER_TICK (CLOCKS_PER_SEC / TIMER_FREQ)

#define USECS_PER_TICK  (1000000 / TIMER_FREQ)
#define MSECS_PER_TICK  (1000 / TIMER_FREQ)

extern struct timeval systemclock;
extern volatile unsigned int ticks;
extern volatile unsigned int clocks;

krnlapi unsigned int get_ticks();

krnlapi void udelay(unsigned long us);

krnlapi unsigned char read_cmos_reg(int reg);

krnlapi void write_cmos_reg(int reg, unsigned char val);

void init_pit();

void calibrate_delay();

krnlapi time_t

get_time();

time_t time(time_t * time);

void set_time(struct timeval *tv);

int load_sysinfo(struct loadinfo *info);

#endif
