#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "settings.h"

void setSetting(Config* config, char* s) {
    char* token = strtok(s, ":");
    char* value = strtok(NULL, ":");

    if (token == NULL || value == NULL) {
        fprintf(stderr, "setSetting NULL tokenization\n");
        return;
    }

    int n = INT_MAX;
    const char* variableList[] = { "useDVR", "N", "a", "p"};
    for (int i = 0; i != (int)sizeof(variableList)/sizeof(variableList[0]); ++i) {
        if (!strcmp(token, variableList[i])) {
            n = i;
            break;
        }
    }

    switch (n) {
        case 0:
            if (!strcmp(value, "True") || !strcmp(value, "true"))
                config->useDVR = true;
            else
                config->useDVR = false;
            break;
        case 1:
            config->N = atoi(value);
            break;
        case 2:
            config->a = strtod(value, NULL);
            break;
        case 3:
            config->p = strtod(value, NULL);
            break;
    }
}