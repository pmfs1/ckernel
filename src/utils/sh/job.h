//
// Shell jobs
//
#ifndef JOB_H
#define JOB_H

#include "sh.h"

#define STD_HANDLES  3

#define J_VAR_SCOPE     0x0001
#define J_ARG_SCOPE     0x0002
#define J_DEFERRED_VARS 0x0004
#define J_LOOP          0x0008
#define J_BREAK         0x0010
#define J_CONTINUE      0x0020
#define J_FUNCTION      0x0040

//
// Arguments
//

struct arg {
    struct arg *next;
    char *value;
};

struct args {
    struct arg *first;
    struct arg *last;
    int num;
};

//
// Variables
//

struct var {
    struct var *next;
    int hash;
    char *name;
    char *value;
};

//
// Job
//

struct job {
    int flags;
    struct shell *shell;
    struct job *parent;
    struct var *vars;
    struct args args;
    int fd[STD_HANDLES];
    int exitcode;
    int handle;
    int pid;
    main_t main;
    struct job *next;
};

void init_shell(struct shell *shell, int argc, char *argv[], char *env[]);

void clear_shell(struct shell *shell);

struct job *create_job(struct job *parent, int flags);

void remove_job(struct job *job);

int execute_job(struct job *job);

void detach_job(struct job *job);

int wait_for_job(struct job *job);

void init_args(struct args *args);

void delete_args(struct args *args);

struct arg *add_arg(struct args *args, char *value);

char *get_command_line(struct args *args);

void set_var(struct job *job, char *name, char *value);

char *get_var(struct job *job, char *name);

struct job *get_arg_scope(struct job *job);

struct job *get_var_scope(struct job *job, int defer);

void set_fd(struct job *job, int h, int fd);

int get_fd(struct job *job, int h, int own);

#endif
