#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef __RESPONSE_H__
#define __RESPONSE_H__

char *convert_timestamp(int timestamp) {
  time_t ts = (time_t)timestamp;
  struct tm *tm_info;
  char *buffer =
      (char *)malloc(20 * sizeof(char)); // Allocate memory for "yyyy-mm-dd\0"

  tm_info = localtime(&ts);
  strftime(buffer, 20, "%Y-%m-%d", tm_info);

  return buffer;
}
// function to read contents of a file
char *read_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(length + 1);
  if (!buffer) {
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, length, file);
  buffer[length] = '\0';
  fclose(file);

  // remove \n from the last
  if (buffer[length - 1] == '\n') {
    buffer[length - 1] = '\0';
  }

  return buffer;
}

void concatenate_string(char *s, char *s1) {
  int i;
  int j = strlen(s);
  for (i = 0; s1[i] != '\0'; i++) {
    s[i + j] = s1[i];
  }
  s[i + j] = '\0';
  return;
}

// check if two strings are equal
bool are_equal(char *s1, char *s2) {
  if (strcmp(s1, s2) == 0) {
    return true;
  }
  return false;
}

bool is_digit(char c) {
  if (c >= '0' && c <= '9') {
    return true;
  }
  return false;
}

bool is_leap_year(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool has_leap_year(const char *s) {
  int len = strlen(s);

  for (int i = 0; i <= len - 4; i++) {
    if (is_digit(s[i]) && is_digit(s[i + 1]) && is_digit(s[i + 2]) &&
        is_digit(s[i + 3])) {
      int year = (s[i] - '0') * 1000 + (s[i + 1] - '0') * 100 +
                 (s[i + 2] - '0') * 10 + (s[i + 3] - '0');
      if (is_leap_year(year)) {
        return true;
      }
    }
  }

  return false;
}

#endif
