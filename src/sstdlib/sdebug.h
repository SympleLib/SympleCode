#ifndef _SSTDB_LIB_SDEBUG_H_
#define _SSTDB_LIB_SDEBUG_H_
#include <stdio.h>
#include <assert.h>

#include "../sparser.h"

#define SEXCEPT(m) { printf("Exception at (%s:%i): %s\n", sGetCurrentFile(), sGetCurrentLine(), m); abort(); }
#define SEXCEPTF(m, ...) { printf("Exception at (%s:%i): ",  sGetCurrentFile(), sGetCurrentLine()); printf(m, ##__VA_ARGS__); printf("\n"); abort(); }
#define SERROR(m) { printf("Error at (%s:%i): %s\n", sGetCurrentFile(), sGetCurrentLine(), m); abort(); }
#define SERRORF(m, ...) { printf("Error at (%s:%i): ", sGetCurrentFile(), sGetCurrentLine()); printf(m, ##__VA_ARGS__); printf("\n"); abort(); }

#if !_SRELEASE
#define SASSERT(c, m) if (!(c)) { printf("Assertion Error at (%s:%i): %s\n", __FILE__, __LINE__, m); abort(); }
#define SCALL(c) c
#else
#define SASSERT(c, m)
#define SCALL(c)
#endif
#endif