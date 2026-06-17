#ifndef UTILS_H
#define UTILS_H

int get_safe_int(const char *prompt, const int *out_value);
int get_safe_float(const char *prompt, const float *out_value);
void get_safe_string(const char *prompt, const char *out_str, int max_len);
void clear_input_buffer(void);
void to_lowercase(const char *str);

#endif /* UTILS_H */
