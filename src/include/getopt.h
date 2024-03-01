//
// Command line option parsing
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef GETOPT_H
#define GETOPT_H

#ifndef _GETOPT_DEFINED
#define _GETOPT_DEFINED

int *_opterr();

int *_optind();

int *_optopt();

char **_optarg();

#define opterr (*_opterr())
#define optind (*_optind())
#define optopt (*_optopt())
#define optarg (*_optarg())

#endif

struct option {
    const char *name;
    int has_arg;
    int *flag;
    int val;
};

#define    no_argument       0
#define required_argument 1
#define optional_argument 2

#ifdef  __cplusplus
extern "C" {
#endif

int getopt(int argc, char **argv, char *opts);

int getopt_long(int argc, char **argv, const char *opts, const struct option *longopts, int *index);

#ifdef  __cplusplus
}
#endif

#endif
