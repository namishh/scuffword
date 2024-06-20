#include "prompts.c"
#include "string.h"
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

  discord_create_message(client, msg->channel_id, &params, NULL);
  unsigned long long int id = msg->author->id;
  char *name = msg->author->username;

  struct user *profile = get_profile(id);
  if (profile != NULL) {
    printf("Profile found - ID: %llu, Name: %s, Level: %d, Captcha: %s\n",
           profile->id, profile->name, profile->level, profile->captcha);
    params.content = "You are already registered!";
  } else {
    profile = create_user(id, name);
    if (profile != NULL) {
      printf("Profile created for ID: %llu\n", id);
      printf("Profile created - ID: %llu, Name: %s, Level: %d, Captcha: %s\n",
             profile->id, profile->name, profile->level, profile->captcha);
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

void on_prompt(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;
  unsigned long long int id = msg->author->id;

  struct user *profile = get_profile(id);
  if (profile == NULL) {
    printf("Profile not found.\n");
    return;
  }
  char message[512];
  snprintf(message, sizeof(message),
           "**%s**, your current prompt is: %s.\nPrevious answer: %s",
           profile->name, Challenges[profile->level].name,
           profile->previous_answer);
  struct discord_create_message params = {.content = message};
  discord_create_message(client, msg->channel_id, &params, NULL);
}

void on_profile(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;

  unsigned long long int id = msg->author->id;
  char *name = msg->author->username;

  struct discord_create_message params = {.content = "hi"};
  struct user *profile = get_profile(id);
  if (profile != NULL) {
    char message[512];
    snprintf(message, sizeof(message),
             "Profile found -```ID: %llu\nName: %s\nLevel: %d\nCaptcha: %s\n"
             "Prev Answer: %s```",
             profile->id, profile->name, profile->level, profile->captcha,
             profile->previous_answer);
    struct discord_create_message params = {
        .content = message,
    };
    discord_create_message(client, msg->channel_id, &params, NULL);
    printf("Profile found - ID: %llu, Name: %s, Level: %d, Captcha: %s, Prev "
           "Answer: %s\n",
           profile->id, profile->name, profile->level, profile->captcha,
           profile->previous_answer);
  } else {
    profile = create_user(id, name);
    char message[512];
    snprintf(message, sizeof(message),
             "Profile found -```ID: %llu\nName: %s\nLevel: %d\nCaptcha: %s\n"
             "Prev Answer: %s```",
             profile->id, profile->name, profile->level, profile->captcha,
             profile->previous_answer);
    struct discord_create_message params = {
        .content = message,
    };
    discord_create_message(client, msg->channel_id, &params, NULL);
    if (profile != NULL) {
      printf("Profile created for ID: %llu\n", id);
      printf("Profile created - ID: %llu, Name: %s, Level: %d, Captcha: %s\n",
             profile->id, profile->name, profile->level, profile->captcha);
    } else {
      printf("Failed to create profile for ID: %llu\n", id);
    }
  }
  printf("%s lvl %d\n", profile->name, profile->level);
}

void on_element(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;
  unsigned long long int id = msg->author->id;

  struct user *profile = get_profile(id);
  if (profile == NULL) {
    printf("Profile not found.\n");
    return;
  }
  char message[100];
  snprintf(message, sizeof(message), "Element of the date %s is %s",
           ElementOfTheDay.date, ElementOfTheDay.symbol);
  struct discord_create_message params = {.content = message};
  discord_create_message(client, msg->channel_id, &params, NULL);
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

void on_captcha(struct discord *client, const struct discord_message *msg) {
  if (msg->author->bot)
    return;
  unsigned long long int id = msg->author->id;

  struct user *profile = get_profile(id);
  if (profile == NULL) {
    printf("Profile not found.\n");
    return;
  }
  char message[100];
  sprintf(message, "Captcha for %s is `%s`", profile->name, profile->captcha);
  struct discord_create_message params = {.content = profile->captcha};
  discord_create_message(client, msg->channel_id, &params, NULL);
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
  struct Result *valid = validate_password(args, profile);

  struct discord_embed embed = {
      .title = "Password Validation",
      .author =
          &(struct discord_embed_author){
              .name = profile->name,
          },
      .color = 0x3498DB,
      .timestamp = discord_timestamp(client),
      .footer =
          &(struct discord_embed_footer){
              .text = "Made with ❤️ by chadcat7",
              .icon_url =
                  "https://avatars.githubusercontent.com/u/68964499?v=4",
          },
      .url = "https://github.com/chadcat7/scuffword",
  };

  bool will_proceed = true;

  for (int i = 0; i < profile->level + 1; i++) {
    printf("%s : %d \n", valid[i].message, valid[i].valid);
    if (valid[i].valid == false) {
      will_proceed = false;
    }
    char *s = (valid[i].valid == true) ? "`✅` Passed" : "`🔴` Failed";
    discord_embed_add_field(&embed, valid[i].message, s, false);
  }

  if (will_proceed) {
    char message[200];
    if (profile->level == NoOfChallenges - 1) {
      snprintf(message, sizeof(message),
               "Correct **%s** ! You have completed all the levels! "
               "Congratulations!",
               profile->name);
      struct discord_create_message params = {.content = message};
      discord_create_message(client, msg->channel_id, &params, NULL);
      return;
    }
    snprintf(message, sizeof(message),
             "Correct **%s** ! You have proceeded to level\nYour next prompt "
             "is: %s.",
             profile->name, Challenges[profile->level + 1].name);
    struct discord_create_message params = {.content = message};
    discord_create_message(client, msg->channel_id, &params, NULL);
    progress_user(profile);
    printf("user proceeded to level %d\n", profile->level + 1);
  }
  char prev_answer_message[100];
  sprintf(prev_answer_message, "Previous answer: %s", args);
  struct discord_create_message params = {
      .content = prev_answer_message,
      .embeds =
          &(struct discord_embeds){
              .size = 1,
              .array = &embed,
          },
  };

  discord_create_message(client, msg->channel_id, &params, NULL);

  update_previous_answer(profile, args);
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
    {
        .command = "captcha",
        .description = "captcha",
        .usage = "!captcha",
        .callback = &on_captcha,
    },
    {
        .command = "element",
        .description = "get today's element",
        .usage = "!element",
        .callback = &on_element,
    },
    {
        .command = "prompt",
        .description = "get your current prompt",
        .usage = "!prompt",
        .callback = &on_prompt,
    },
};

const int NoOfCommands = sizeof(Commands) / sizeof(Commands[0]);
