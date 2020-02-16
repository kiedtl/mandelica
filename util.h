#ifndef UTIL_H
#define UTIL_H

float clamp(float value);
u32 parse_uint(char *src);

void handle_u32(void *data, char **pars, const int pars_count);
void handle_float(void *data, char **pars, const int pars_count);
void handle_bool(void *data, char **pars, const int pars_count);

#endif
