#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Wordle {
  char *solution;
  char *printdate;
};

struct Wordle wordle;

char *find_json_value(const char *json, const char *field) {
  const char *field_start = strstr(json, field);
  if (field_start == NULL) {
    return NULL;
  }

  const char *value_start = strchr(field_start, ':');
  if (value_start == NULL) {
    return NULL;
  }

  value_start++;
  value_start++;

  const char *value_end = value_start;
  while (*value_end != '\0' && *value_end != '"' && *value_end != '}') {
    value_end++;
  }

  int value_len = value_end - value_start;
  char *value = (char *)malloc(value_len + 1);
  if (value == NULL) {
    return NULL; // Memory allocation failed
  }
  strncpy(value, value_start, value_len);
  value[value_len] = '\0';

  return value;
}

char *run_command(char *command) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  fp = popen(command, "r");
  if (fp == NULL) {
    printf("Failed to run command\n");
    exit(1);
  }
  getline(&line, &len, fp);
  pclose(fp);
  return line;
}

// get current date in a string in YYYY-MM-DD format
char *get_current_date() {
  int timestamp = (int)time(NULL); // Get current Unix timestamp
  char *time = convert_timestamp(timestamp);
  return time;
}

void update_wordle() {
  char *date = get_current_date();
  char *command = (char *)malloc(strlen(date) + 100);
  sprintf(command, "curl https://www.nytimes.com/svc/wordle/v2/%s.json", date);
  char *res = run_command(command);
  char *solution = find_json_value(res, "\"solution\"");
  char *printdate = find_json_value(res, "\"print_date\"");
  wordle.solution = solution;
  wordle.printdate = printdate;
  return;
}
