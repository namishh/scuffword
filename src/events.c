#include <concord/discord.h>
#include <stdio.h>

void on_ready(struct discord *client, const struct discord_ready *event) {
  struct discord_activity activities[] = {
      {
          .name = "with my suicidal ideations.",
          .type = DISCORD_ACTIVITY_GAME,
          .details = "Fixing some bugs",
      },
  };

  struct discord_presence_update status = {
      .activities =
          &(struct discord_activities){
              .size = sizeof(activities) / sizeof *activities,
              .array = activities,
          },
      .status = "idle",
      .afk = false,
      .since = discord_timestamp(client),
  };

  discord_update_presence(client, &status);

  // set the status of the bot
  printf("succesfully connected to discord as %s#%s!\n", event->user->username,
         event->user->discriminator);
}
