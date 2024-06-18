#include "wordle.c"
#include <ctype.h>
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

char *months[] = {"January",   "February", "March",    "April",
                  "May",       "June",     "July",     "August",
                  "September", "October",  "November", "December"};

bool level_05(char *s) {
  for (int i = 0; i < 12; i++) {
    if (strstr(s, months[i]) != NULL) {
      return true;
    }
  }
  return false;
}

// wordle level
bool level_10(char *s) {
  if (strstr(s, wordle.solution) != NULL) {
    return true;
  }
  return false;
}

// is year leap
bool is_leap(char *s) {
  int year = atoi(s);
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      if (year % 400 == 0) {
        return true;
      }
      return false;
    }
    return true;
  }
  return false;
}

// check if string has a leap year in it
bool level_06(char *s) {
  if (has_leap_year(s)) {
    return true;
  }
  return false;
}

// password should contain 🏋️ emoji
bool level_07(char *s) {
  if (strstr(s, "🏋️") != NULL) {
    return true;
  }
  return false;
}

char *fake_sponsers[3] = {"reddit", "discord", "facebook"};
bool level_08(char *s) {
  for (int i = 0; i < 3; i++) {
    if (strstr(s, fake_sponsers[i]) != NULL) {
      return true;
    }
  }
  return false;
}

// password must contain one of the following affirmations: I am loved|I am
// worthy|I am enough
bool level_09(char *s) {
  if (strstr(s, "I am loved") != NULL || strstr(s, "I am worthy") != NULL ||
      strstr(s, "I am enough") != NULL) {
    return true;
  }
  return false;
}

// password must contain the current date in YYYY-MM-DD format
bool level_11(char *s) {
  char *date = get_current_date();
  if (strstr(s, date) != NULL) {
    return true;
  }
  return false;
}

// sum of digits in the password should not be more than 20
bool level_12(char *s) {
  int sum = 0;
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      sum += s[i] - '0';
    }
  }
  if (sum > 20) {
    return false;
  }
  return true;
}

// all vowels should be enclosed within ** **
bool isVowel(char ch) {
  ch = tolower(ch); // Convert to lowercase to handle both cases
  return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
}

bool level_13(char *str) {
  int len = strlen(str);
  bool withinStars = false; // Flag to check if we are within **

  for (int i = 0; i < len; ++i) {
    if (isVowel(str[i])) {
      if (!withinStars) {
        // If a vowel is found and we are not within **, return false
        if (i > 0 && str[i - 1] == '*' && str[i - 2] == '*') {
          withinStars = true;
        } else {
          return false;
        }
      } else {
        // If we are already within **, check if the closing ** is present
        if (!(i + 1 < len && str[i + 1] == '*' && str[i + 2] == '*')) {
          return false;
        }
        withinStars = false; // Reset flag after closing **
        i += 2;              // Skip over closing **
      }
    }
  }

  return !withinStars; // Check if all opened ** have been closed
}

// password should contain the word belarus
bool level_14(char *s) {
  if (strstr(s, "Belarus") != NULL) {
    return true;
  }
  return false;
}

bool level_15(char *s) {
  if (strstr(s, "A9R8$") != NULL) {
    return true;
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
    {.name = "Password should contain name of a month", .check = &level_05},
    {.name = "Password should contain a leap year.", .check = &level_06},
    {.name = "Password is not strong enough.", .check = &level_07},
    {.name =
         "Password should contain one of your fake sponsers. [one of our fake "
         "sponsers](https://iili.io/d9D1dNa.png).",
     .check = &level_08},
    {.name = "Password must contain one of the following affirmations:\n I am "
             "loved | I am worthy | I am enough",
     .check = &level_09},
    {.name = "Password should contain today's wordle answer",
     .check = &level_10},
    {.name = "Password should contain today's date in YYYY-MM-DD format",
     .check = &level_11},
    {.name = "Sum of digits in the password should not be more than 20",
     .check = &level_12},
    {.name = "Each individual vowel should be bold.", .check = &level_13},
    {.name = "Password should contain this country: 🇧🇾", .check = &level_14},
    {.name = "Password should contain this captcha: "
             "[captcha](https://iili.io/d9b7Thl.png) ",
     .check = &level_15},

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
