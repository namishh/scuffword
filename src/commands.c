#include <concord/discord.h>
#include <concord/log.h>

struct command {
  char *command;
  char *usage;
  char *description;
  void (*callback)(struct discord *client,
                   const struct discord_message *message);
};

void on_ping(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;

  struct discord_create_message params = {.content = "pong"};
  discord_create_message(client, msg->channel_id, &params, NULL);
}

static const struct command Commands[] = {{.command = "ping",
                                           .description = "send pong",
                                           .usage = "!ping",
                                           .callback = &on_ping}};

static const int NoOfCommands = sizeof(Commands) / sizeof(Commands[0]);
