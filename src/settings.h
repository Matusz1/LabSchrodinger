#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdbool.h>
#include <stdlib.h>

/* Defines under what configuration the program will run */
typedef struct {
    bool highPrecision;
    bool onlyValues;
    int N;
    double a;
    double omega;
    int x0;
    bool HO;
} Config;


/* String s in form: "VAR=VALUE" (f.e. "a=0.124"), string will be tokenized */
void setSetting(Config* config, char* s);

#endif /* _SETTINGS_H_ */
