#ifndef MENU_H
#define MENU_H

/**
 * @brief Displays the interactive customer menu options loop.
 */
void display_customer_menu(void);

/**
 * @brief Displays the interactive admin menu options loop.
 */
void display_admin_menu(void);

/**
 * @brief Runs the main menu system, loading the DB and starting the customer
 * menu.
 */
void run_menu_system(void);

#endif /* MENU_H */
