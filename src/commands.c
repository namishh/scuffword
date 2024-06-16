#include "db.c"
#include "prompts.c"
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

void on_profile(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;

  unsigned long long int id = msg->author->id;
  char *name = msg->author->username;

  printf("Hello\n");

  struct user *profile = get_profile(id);
  if (profile != NULL) {
    printf("Profile found - ID: %llu, Name: %s, Level: %d\n", profile->id,
           profile->name, profile->level);
  } else {
    profile = create_user(id, name);
    if (profile != NULL) {
      printf("Profile created for ID: %llu\n", id);
      printf("Profile created - ID: %llu, Name: %s, Level: %d\n", profile->id,
             profile->name, profile->level);
    } else {
      printf("Failed to create profile for ID: %llu\n", id);
    }
  }
  printf("%s\n", profile->name);
}

void on_guess(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;

  // get the arguements to the command
  char *args = msg->content;
  struct Result *valid = validate_password(args);
  printf("%s\n", args);
}

static const struct command Commands[] = {{.command = "ping",
                                           .description = "send pong",
                                           .usage = "!ping",
                                           .callback = &on_ping},
                                          {
                                              .command = "profile",
                                              .description = "get user profile",
                                              .usage = "!profile",
                                              .callback = &on_profile,
                                          },
                                          {
                                              .command = "guess",
                                              .description = "try password",
                                              .usage = "!guess <guess>",
                                              .callback = &on_guess,
                                          }};

static const int NoOfCommands = sizeof(Commands) / sizeof(Commands[0]);
