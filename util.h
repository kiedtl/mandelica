#ifndef UTIL_H
#define UTIL_H

float clamp(float value);

void handle_uint(void *data, char **pars, const int pars_count);
void handle_bool(void *data, char **pars, const int pars_count);

#endif
