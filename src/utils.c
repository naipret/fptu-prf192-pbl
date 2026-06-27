#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

void to_lowercase(char *str) {
  if (str == NULL) {
    return;
  }
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = (char)tolower((unsigned char)str[i]);
  }
}

void get_safe_string(const char *prompt, char *out_str, int max_len) {
  if (out_str == NULL || max_len <= 0) {
    return;
  }
  if (prompt != NULL) {
    printf("%s", prompt);
    fflush(stdout);
  }
  if (fgets(out_str, max_len, stdin) == NULL) {
    out_str[0] = '\0';
    return;
  }
  int len = (int)strlen(out_str);
  if (len > 0 && out_str[len - 1] == '\n') {
    out_str[len - 1] = '\0';
  } else {
    clear_input_buffer();
  }
}

int get_safe_int(const char *prompt, int *out_value) {
  char buf[128];
  while (1) {
    get_safe_string(prompt, buf, sizeof(buf));

    if (strcmp(buf, "0") == 0) {
      if (out_value != NULL) {
        *out_value = 0;
      }
      return 0;
    }

    int i = 0;
    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] == '\0') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    int sign = 1;
    if (buf[i] == '-') {
      sign = -1;
      i++;
    } else if (buf[i] == '+') {
      i++;
    }

    if (buf[i] < '0' || buf[i] > '9') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    long long val = 0;
    int overflow = 0;
    while (buf[i] >= '0' && buf[i] <= '9') {
      val = (val * 10) + (buf[i] - '0');
      if (sign == 1 && val > 2147483647LL) {
        val = 2147483647LL;
        overflow = 1;
      } else if (sign == -1 && val > 2147483648LL) {
        val = 2147483648LL;
        overflow = 1;
      }
      i++;
    }

    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] != '\0' || overflow) {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    int final_val;
    if (sign == -1) {
      final_val = (int)(-val);
    } else {
      final_val = (int)val;
    }

    if (out_value != NULL) {
      *out_value = final_val;
    }
    return 1;
  }
}

int get_safe_float(const char *prompt, float *out_value) {
  char buf[128];
  while (1) {
    get_safe_string(prompt, buf, sizeof(buf));

    if (strcmp(buf, "0") == 0) {
      if (out_value != NULL) {
        *out_value = 0.0F;
      }
      return 0;
    }

    int i = 0;
    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] == '\0') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    int sign = 1;
    if (buf[i] == '-') {
      sign = -1;
      i++;
    } else if (buf[i] == '+') {
      i++;
    }

    if ((buf[i] < '0' || buf[i] > '9') && buf[i] != '.') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    double integer_part = 0.0;
    int has_digits = 0;
    while (buf[i] >= '0' && buf[i] <= '9') {
      integer_part = (integer_part * 10.0) + (buf[i] - '0');
      has_digits = 1;
      i++;
    }

    double fractional_part = 0.0;
    double divisor = 1.0;
    if (buf[i] == '.') {
      i++;
      if ((buf[i] < '0' || buf[i] > '9') && !has_digits) {
        printf("Invalid entry, please try again.\n");
        continue;
      }
      while (buf[i] >= '0' && buf[i] <= '9') {
        fractional_part = (fractional_part * 10.0) + (buf[i] - '0');
        divisor *= 10.0;
        has_digits = 1;
        i++;
      }
    }

    if (!has_digits) {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] != '\0') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    double val = sign * (integer_part + (fractional_part / divisor));

    if (out_value != NULL) {
      *out_value = (float)val;
    }
    return 1;
  }
}
void input_string(char *str, int size) {
  if (fgets(str, size, stdin) == NULL) {
    str[0] = '\0';
    return;
  }
  int len = (int)strlen(str);
  for (int i = 0; i < len; i++) {
    if (str[i] == '\n' || str[i] == '\r') {
      str[i] = '\0';
      break;
    }
  }
}

void cont(void) {
  printf("Press enter to continue!");
  (void)getchar();
}

int confirm_action(const char *message) {
  char choice[5];
  printf("%s (Y/N): ", message);
  input_string(choice, sizeof(choice));
  return (choice[0] == 'Y' || choice[0] == 'y');
}
