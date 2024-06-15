#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
