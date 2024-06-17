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

void on_register(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;

  struct discord_create_message params = {.content = ""};

  unsigned long long int id = msg->author->id;
  char *name = msg->author->username;

  struct user *profile = get_profile(id);
  if (profile != NULL) {
    printf("Profile found - ID: %llu, Name: %s, Level: %d\n", profile->id,
           profile->name, profile->level);
    params.content = "You are already registered!";
  } else {
    profile = create_user(id, name);
    if (profile != NULL) {
      printf("Profile created for ID: %llu\n", id);
      printf("Profile created - ID: %llu, Name: %s, Level: %d\n", profile->id,
             profile->name, profile->level);
      free(profile);
      params.content = "You have been registered!";
    } else {
      printf("Failed to create profile for ID: %llu\n", id);
      params.content = "Failed to register!";
      free(profile);
    }
  }

  discord_create_message(client, msg->channel_id, &params, NULL);
}

void on_profile(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;

  unsigned long long int id = msg->author->id;
  char *name = msg->author->username;

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
  printf("%s lvl %d\n", profile->name, profile->level);
}

void on_reset_profile(struct discord *client,
                      const struct discord_message *msg) {
  if (msg->author->bot)
    return;
  unsigned long long int id = msg->author->id;

  struct user *profile = get_profile(id);
  if (profile == NULL) {
    printf("Profile not found.\n");
    return;
  }
  reset_profile(profile);
}

void on_guess(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;

  unsigned long long int id = msg->author->id;

  struct user *profile = get_profile(id);

  if (profile == NULL) {
    struct discord_create_message params = {
        .content = "You are not registered! Register yourself with `!register` "
                   "or `!profile`"};
    discord_create_message(client, msg->channel_id, &params, NULL);
    return;
  }

  if (profile->level == NoOfChallenges) {
    struct discord_create_message params = {
        .content = "You have completed all the levels! Congratulations!"};
    discord_create_message(client, msg->channel_id, &params, NULL);
    return;
  }

  char *args = msg->content;
  struct Result *valid = validate_password(args, profile->level);

  bool will_proceed = true;

  for (int i = 0; i < profile->level + 1; i++) {
    printf("%s : %d \n", valid[i].message, valid[i].valid);
    if (valid[i].valid == false) {
      will_proceed = false;
    }
  }

  if (will_proceed) {
    progress_user(profile);
    printf("user proceeded to level %d\n", profile->level + 1);
  }

  printf("will proceed: %d\n", will_proceed);
  printf("%s\n", args);
}

const struct command Commands[] = {
    {.command = "ping",
     .description = "send pong",
     .usage = "!ping",
     .callback = &on_ping},
    {
        .command = "register",
        .description = "register user",
        .usage = "!register",
        .callback = &on_register,
    },
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
    },
    {
        .command = "reset",
        .description = "reset user profile",
        .usage = "!reset",
        .callback = &on_reset_profile,
    },
};

const int NoOfCommands = sizeof(Commands) / sizeof(Commands[0]);
