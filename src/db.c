#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

sqlite3 *DB;
static const char *DB_FILE = "./private/data.db";

struct user {
  unsigned long long int id;
  char *name;
  int level;
  char *previous_answer;
  char *captcha;
  int passed_level_20;
};

void seed_db() {
  char *err_msg = 0;
  sqlite3_open(DB_FILE, &DB);
  int rc = sqlite3_exec(DB,
                        "CREATE TABLE IF NOT EXISTS users (id BIGINT PRIMARY "
                        "KEY, name TEXT, level INTEGER, previous_answer TEXT, "
                        "captcha TEXT, passed_level_20 INTEGER);",
                        0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to create table: %s\n", err_msg);
    sqlite3_free(err_msg);
  }
}

char random_char(int index) {
  char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456"
                   "789!@#$%^&*()_-+=~`{}[]|:;<>?,./";
  return charset[index];
}

char random_captcha() { return random_char(rand() % 80); }

char *generate_captcha() {
  srand(time(NULL));
  char *captcha = malloc(7 * sizeof(char));
  int i;
  for (i = 0; i < 6; i++) {
    captcha[i] = random_captcha();
  }
  captcha[6] = '\0';
  return captcha;
}

// get profile by id
struct user *get_profile(int id) {
  sqlite3_stmt *stmt;
  struct user *result = NULL;

  sqlite3_open(DB_FILE, &DB);
  int rc = sqlite3_prepare_v2(
      DB,
      "SELECT id, name, level,previous_answer,captcha,passed_level_20 FROM "
      "users WHERE id = ?",
      -1, &stmt, NULL);

  sqlite3_bind_int(stmt, 1, id);
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    // If the query returns a row, allocate memory for the result
    result = (struct user *)malloc(sizeof(struct user));
    if (result == NULL) {
      fprintf(stderr, "Failed to allocate memory for user\n");
      sqlite3_finalize(stmt); // Clean up the statement
      return NULL;
    }

    // Retrieve data from the result set

    result->id = sqlite3_column_int(stmt, 0);
    result->name = strdup(
        (const char *)sqlite3_column_text(stmt, 1)); // Allocate memory for name
    result->level = sqlite3_column_int(stmt, 2);
    result->previous_answer =
        strdup((const char *)sqlite3_column_text(stmt, 3));
    result->captcha = strdup((const char *)sqlite3_column_text(stmt, 4));
    result->passed_level_20 = sqlite3_column_int(stmt, 5);
  } else {
    // make profile if not found
  }

  // Finalize the statement
  sqlite3_finalize(stmt);

  return result;
}

// set passed_level_20 = 1 for a profile
void setl20(struct user *profile) {
  sqlite3_stmt *stmt;
  sqlite3_open(DB_FILE, &DB);
  int rc = sqlite3_prepare_v2(DB, "UPDATE users SET level = 1 WHERE id = ?", -1,
                              &stmt, NULL);
  char *new_captcha = generate_captcha();
  sqlite3_bind_text(stmt, 1, new_captcha, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 2, profile->id);
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(DB);
}

// create profile for the user
struct user *create_user(int id, char *username) {
  sqlite3_stmt *stmt;
  struct user *result = NULL;
  char *captcha = generate_captcha();
  sqlite3_open(DB_FILE, &DB);
  int rc = sqlite3_prepare_v2(DB, "INSERT INTO users VALUES(?, ?, 1, '', ?, 0)",
                              -1, &stmt, NULL);

  sqlite3_bind_int(stmt, 1, id);
  sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, captcha, -1, SQLITE_STATIC);
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return NULL;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(DB);
  result = malloc(sizeof(struct user));
  result->id = id;
  result->name = strdup(username);
  result->level = 0;
  result->previous_answer = "";
  result->captcha = strdup(captcha);
  result->passed_level_20 = 0;

  return result;
}

void progress_user(struct user *profile) {
  sqlite3_stmt *stmt;
  sqlite3_open(DB_FILE, &DB);
  int rc = sqlite3_prepare_v2(
      DB, "UPDATE users SET level = level + 1 WHERE id = ?", -1, &stmt, NULL);

  sqlite3_bind_int(stmt, 1, profile->id);
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(DB);
}

// update player's previous previous_answer
void update_previous_answer(struct user *profile, char *answer) {
  sqlite3_stmt *stmt;
  sqlite3_open(DB_FILE, &DB);
  int rc = sqlite3_prepare_v2(
      DB, "UPDATE users SET previous_answer = ? WHERE id = ?", -1, &stmt, NULL);

  sqlite3_bind_text(stmt, 1, answer, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 2, profile->id);
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(DB);
}

void reset_profile(struct user *profile) {
  sqlite3_stmt *stmt;
  sqlite3_open(DB_FILE, &DB);
  int rc = sqlite3_prepare_v2(DB,
                              "UPDATE users SET level = 0, previous_answer = "
                              "'', captcha = ? WHERE id = ?",
                              -1, &stmt, NULL);
  char *new_captcha = generate_captcha();
  sqlite3_bind_text(stmt, 1, new_captcha, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 2, profile->id);
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(DB);
}
