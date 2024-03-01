//
// Timer functions
//
#ifndef TIMER_H
#define TIMER_H

#define time_after(a, b)     ((long) (b) - (long) (a) < 0)
#define time_before(a, b)    time_after(b, a)

#define time_after_eq(a, b)  ((long) (a) - (long) (b) >= 0)
#define time_before_eq(a, b) time_after_eq(b, a)

typedef void (*timerproc_t)(void *arg);

struct timer;

struct timer_link {
    struct timer_link *next;
    struct timer_link *prev;
};

struct timer {
    struct timer_link link;
    unsigned int expires;
    int active;
    timerproc_t handler;
    void *arg;
};

void init_timers();

void run_timer_list();

krnlapi void init_timer(struct timer *timer, timerproc_t handler, void *arg);

krnlapi void add_timer(struct timer *timer);

krnlapi int del_timer(struct timer *timer);

krnlapi int mod_timer(struct timer *timer, unsigned int expires);

krnlapi int msleep(unsigned int millisecs);

#endif
