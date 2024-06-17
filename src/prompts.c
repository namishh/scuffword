#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// link to character set:
// https://en.wikipedia.org/wiki/ASCII#Printable_characters

// check is a string is at atleast 6 characters long
bool level_01(char *s) {
  if (strlen(s) < 6) {
    return false;
  }
  return true;
}

// check if string contains a number
bool level_02(char *s) {
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      return true;
    }
  }
  return false;
}

// check if string contains an uppercase letter
bool level_03(char *s) {
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] >= 'A' && s[i] <= 'Z') {
      return true;
    }
  }
  return false;
}

// check if string contains a special character
bool level_04(char *s) {
  for (int i = 0; i < strlen(s); i++) {
    if (!((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') ||
          (s[i] >= '0' && s[i] <= '9'))) {
      return true;
    }
  }
  return false;
}

struct Challenge {
  char *name;
  bool (*check)(char *);
};

const struct Challenge Challenges[] = {
    {.name = "Password should have atleast 6 letters.", .check = &level_01},
    {.name = "Password should contain a number.", .check = &level_02},
    {.name = "Password should contain an uppercase letter.",
     .check = &level_03},
    {.name = "Password should contain a special symbol.", .check = &level_04},
};

const int NoOfChallenges = sizeof(Challenges) / sizeof(Challenges[0]);

struct Result {
  bool valid;
  char *message;
};

// make a function that returns an array of results which are not valid

struct Result *validate_password(char *password, int level) {
  struct Result *results = malloc(sizeof(struct Result) * NoOfChallenges);
  for (int i = 0; i < level + 1; i++) {
    bool valid = Challenges[i].check(password);
    results[i].valid = valid;
    results[i].message = Challenges[i].name;
  }
  return results;
}
