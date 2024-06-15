#include "src/commands.c"
#include "src/utils.c"
#include <concord/discord.h>
#include <concord/log.h>
#include <stdio.h>
#include <stdlib.h>

#define PREFIX "!"

// EVENTS
void on_ready(struct discord *client, const struct discord_ready *event) {
  // set the status of the bot
  printf("succesfully connected to Discord as %s#%s!\n", event->user->username,
         event->user->discriminator);
}

int main(void) {
  char *key = read_file("./private/KEY.txt");
  struct discord *client = discord_init(key);
  free(key);

  if (!client) {
    fprintf(stderr, "Failed to initialize Discord client\n");
    free(key);
    return 1;
  }

  discord_add_intents(client, DISCORD_GATEWAY_GUILDS);
  discord_set_on_ready(client, &on_ready);

  // setting the commands
  for (size_t i = 0; i < NoOfCommands; i++) {
    char s[] = PREFIX;
    concatenate_string(s, Commands[i].command);
    discord_set_on_command(client, s, Commands[i].callback);
  }

  discord_run(client);
  discord_cleanup(client);
  return 0;
}
