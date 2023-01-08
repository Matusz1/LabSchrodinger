#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"

void setSetting(Config* config, char* s) {
    char* token = strtok(s, "=");
    char* value = strtok(NULL, "=");

    if (token == NULL || value == NULL) {
        fprintf(stderr, "setSetting NULL tokenization\n");
        return;
    }

    int n = 1000;
    const char* variableList[] = { "highPrecision", "onlyValues", "N", "a", "omega", "x0", "HO"};
    for (int i = 0; i != (int)sizeof(variableList)/sizeof(variableList[0]); ++i) {
        if (!strcmp(token, variableList[i])) {
            n = i;
            break;
        }
    }

    switch (n) {
        case 0:
            config->highPrecision = !strcmp(value, "True") || !strcmp(value, "true"); break;
        case 1:
            config->onlyValues = !strcmp(value, "True") || !strcmp(value, "true"); break;
        case 2:
            config->N = atoi(value); break;
        case 3:
            config->a = strtod(value, NULL); break;
        case 4:
            config->omega = strtod(value, NULL); break;
        case 5:
            config->x0 = atoi(value); break;
        case 6:
            config->HO = !strcmp(value, "True") || !strcmp(value, "true"); break;
    }
}
