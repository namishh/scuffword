#include <concord/discord.h>
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

int main(void) {
  char *key = read_file("KEY.txt");
  struct discord *client = discord_init(key);
  free(key);

  if (!client) {
    fprintf(stderr, "Failed to initialize Discord client\n");
    free(key);
    return 1;
  }

  discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT);
  discord_run(client);

  discord_cleanup(client);
  return 0;
}
