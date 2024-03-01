//
// Console I/O
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef CONIO_H
#define CONIO_H

#ifdef  __cplusplus
extern "C" {
#endif

int cputs(char *string);

int getch();

int putch(int ch);

int kbhit();

#ifdef  __cplusplus
}
#endif

#endif
