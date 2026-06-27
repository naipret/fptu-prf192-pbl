#ifndef ADMIN_H
#define ADMIN_H

#include "types.h"

/**
 * @brief Checks if the admin credentials configuration file exists.
 * @return 1 if setup is complete (file exists), 0 otherwise.
 */
int is_admin_setup_complete(void);

/**
 * @brief Obfuscates the password and saves credentials to the admin config
 * file.
 * @param username The plain-text admin username to register.
 * @param password The plain-text admin password to obfuscate and register.
 * @return 1 on success, 0 on failure.
 */
int setup_admin_credentials(const char *username, const char *password);

/**
 * @brief Reads credentials, obfuscates the input password, and compares both.
 * @param username The plain-text username input.
 * @param password The plain-text password input.
 * @return 1 if credentials verify successfully, 0 otherwise.
 */
int verify_admin_login(const char *username, const char *password);

/**
 * @brief Runs an interactive CLI wizard loop to set up admin credentials.
 */
void run_admin_registration_wizard(void);

/**
 * @brief Runs an interactive CLI login loop allowing up to 3 attempts.
 * @return 1 on successful login, 0 if cancelled or locked out.
 */
int run_admin_login_loop(void);

#endif
