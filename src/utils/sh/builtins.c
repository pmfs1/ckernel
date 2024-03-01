//
// Built-in shell commands
//
#include "sh.h"

builtin(break) {
    struct job *j;
    int levels = 1;
    if (job->args.num > 1) {
        levels = atoi(job->args.first->next->value);
        if (levels < 1) levels = 1;
    }

    for (j = job; j; j = j->parent) {
        j->flags |= J_BREAK;
        if (j->flags & J_LOOP) {
            if (--levels == 0) break;
        }
    }

    return job->shell->lastrc;
}

builtin(chdir) {
    char *path;

    if (job->args.num > 1) {
        path = job->args.first->next->value;
    } else {
        path = "/";
    }

    if (chdir(path) < 0) {
        fprintf(stderr, "%s: %s\n", path, strerror(errno));
        return 1;
    }

    return 0;
}

builtin(continue) {
    struct job *j;
    int levels = 1;
    if (job->args.num > 1) {
        levels = atoi(job->args.first->next->value);
        if (levels < 1) levels = 1;
    }

    for (j = job; j; j = j->parent) {
        if (j->flags & J_LOOP) {
            if (--levels == 0) {
                j->flags |= J_CONTINUE;
                break;
            }
        }
        j->flags |= J_BREAK;
    }

    return job->shell->lastrc;
}

builtin(debug) {
    job->shell->debug = 1;
    return 0;
}

builtin(exit) {
    int rc = 0;
    if (job->args.num > 1) rc = atoi(job->args.first->next->value);
    job->shell->done = 1;
    return rc;
}

builtin(false) {
    return 1;
}

builtin(return) {
    struct job *j;
    int rc = 0;
    if (job->args.num > 1) rc = atoi(job->args.first->next->value);

    for (j = job; j; j = j->parent) {
        j->flags |= J_BREAK;
        if (j->flags & J_FUNCTION) break;
    }

    return rc;
}

builtin(set) {
    struct var *var;
    struct arg *arg;

    if (job->args.num > 1) {
        // Set shell arguments
        struct job *scope = get_arg_scope(job);
        struct args *args = &scope->args;
        char *argv0 = strdup(args->first->value);
        args->first->value = NULL;
        delete_args(args);
        add_arg(args, argv0);
        free(argv0);
        arg = job->args.first->next;
        while (arg) {
            add_arg(args, arg->value);
            arg = arg->next;
        }
    } else {
        // Print variables
        struct job *scope = get_var_scope(job, 1);
        var = scope->vars;
        while (var) {
            printf("%s=%s\n", var->name, var->value);
            var = var->next;
        }
    }

    return 0;
}

builtin(shift) {
    int n = 1;
    struct job *scope = get_arg_scope(job);
    struct arg *arg = scope->args.first;

    if (job->args.num > 1) n = atoi(job->args.first->next->value);
    if (n < 0) return 1;
    if (!arg) return 0;
    while (n > 0 && arg->next) {
        struct arg *a = arg->next;
        arg->next = a->next;
        free(a->value);
        free(a);
        scope->args.num--;
        n--;
    }

    return 0;
}

builtin(true) {
    return 0;
}
