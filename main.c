#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef TYPES_H
#define TYPES_H

#define MAX_PRODUCTS 100
#define MAX_BUNDLES 50
#define MAX_ORDERS 200
#define MAX_BUNDLE_ITEMS 10

#define MAX_NAME_LEN 100
#define MAX_CAT_LEN 50
#define MAX_BRAND_LEN 50
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_DATE_LEN 20

typedef struct {
  char username[MAX_USERNAME_LEN];
  char password[MAX_PASSWORD_LEN];
  int is_setup;
} AdminCredentials;

typedef struct {
  int product_id;
  char product_name[MAX_NAME_LEN];
  char category[MAX_CAT_LEN];
  char brand[MAX_BRAND_LEN];
  float price;
  int stock_quantity;
} Product;

typedef struct {
  int bundle_id;
  char bundle_name[MAX_NAME_LEN];
  int product_ids[MAX_BUNDLE_ITEMS];
  int product_count;
  float discount_rate;
  float bundle_price;
} Bundle;

typedef struct {
  int order_id;
  char customer_name[MAX_NAME_LEN];
  int item_id;
  int is_bundle;
  int quantity;
  float total_price;
  char order_date[MAX_DATE_LEN];
} Order;

#endif /* TYPES_H */

#ifndef ADMIN_H
#define ADMIN_H

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

#ifndef PRODUCT_H
#define PRODUCT_H

/**
 * @brief Adds a new product to the inventory array after validation.
 * @param products Array of product structures.
 * @param count Pointer to the current count of products. Incremented on
 * success.
 * @param new_prod Pointer to the new product structure to be added.
 * @return 1 if product added successfully, 0 if validation fails or limit
 * reached.
 */
int add_product(Product products[], int *count, const Product *new_prod);

/**
 * @brief Updates an existing product's details in the inventory array.
 * @param products Array of product structures.
 * @param count Current number of active products.
 * @param id The ID of the product to update.
 * @param updated Pointer to the product structure containing the updated
 * details.
 * @return 1 if updated successfully, 0 if product ID not found or validation
 * fails.
 */
int update_product(Product products[], int count, int id,
                   const Product *updated);

/**
 * @brief Removes a product from the inventory array, shifting remaining items.
 * @param products Array of product structures.
 * @param count Pointer to the current count of products. Decremented on
 * success.
 * @param id The ID of the product to delete.
 * @param bundles Array of current bundles to check active association.
 * @param bundle_count Current number of bundles.
 * @return 1 if deleted successfully, 0 if product ID not found or deletion is
 * blocked.
 */
int delete_product(Product products[], int *count, int id,
                   const Bundle bundles[], int bundle_count);

/**
 * @brief Searches for a product in the inventory array by its unique ID.
 * @param products Array of product structures.
 * @param count Current number of active products.
 * @param id The ID of the product to search for.
 * @return The array index of the matching product, or -1 if not found.
 */
int find_product_by_id(const Product products[], int count, int id);

/**
 * @brief Displays all products in the inventory array in a clean format.
 * @param products Array of product structures.
 * @param count Current number of active products.
 */
void display_all_products(const Product products[], int count);

/**
 * @brief Searches for products by name (case-insensitive substring match).
 * @param products Array of product structures.
 * @param count Current number of active products.
 * @param query The string to search for.
 */
void search_product_by_name(const Product products[], int count,
                            const char *query);

/**
 * @brief Filters products by category (matching strictly one of "Phone",
 * "Laptop", "Tablet", "Accessory").
 * @param products Array of product structures.
 * @param count Current number of active products.
 * @param category The category string to match.
 */
void filter_product_by_category(const Product products[], int count,
                                const char *category);

/**
 * @brief Filters products by price range (MinPrice <= price <= MaxPrice).
 * @param products Array of product structures.
 * @param count Current number of active products.
 * @param min_price Minimum price boundary.
 * @param max_price Maximum price boundary.
 */
void filter_product_by_price(const Product products[], int count,
                             float min_price, float max_price);

/**
 * @brief Sorts products by price using Bubble Sort and displays the result.
 * @param products Array of product structures.
 * @param count Current number of active products.
 * @param ascending 1 for ascending order, 0 for descending order.
 */
void bubble_sort_by_price(Product products[], int count, int ascending);

/**
 * @brief Sorts products alphabetically by name using Bubble Sort and displays
 * the result.
 * @param products Array of product structures.
 * @param count Current number of active products.
 */
void bubble_sort_by_name(Product products[], int count);

#endif

#ifndef BUNDLE_H
#define BUNDLE_H

/**
 * @brief Creates a new bundle and appends it to the bundles array if valid.
 * @param bundles Array of current bundles in the system.
 * @param count Pointer to the number of bundles. Incremented on success.
 * @param new_bundle Pointer to the new bundle structure to create.
 * @return 1 on success, 0 on failure (e.g. limit reached, invalid discount,
 * empty product list).
 */
int create_bundle(Bundle bundles[], int *count, const Bundle *new_bundle);

/**
 * @brief Adds a product ID to an existing bundle.
 * @param bundle Pointer to the bundle structure to modify.
 * @param product_id The ID of the product to add.
 * @return 1 on success, 0 on failure (e.g. bundle full, product already exists
 * in bundle).
 */
int add_product_to_bundle(Bundle *bundle, int product_id);

/**
 * @brief Removes a product from a bundle, shifting remaining items.
 * @details If removing the product results in an empty bundle (product_count
 * drops to 0), the bundle is deleted from the array. Note: Deleting the bundle
 * from the array shifts elements and invalidates the bundle pointer.
 * @param bundles Array of current bundles in the system.
 * @param bundle_count Pointer to the current count of bundles. Decremented if
 * bundle is deleted.
 * @param bundle Pointer to the bundle structure to modify.
 * @param product_id The ID of the product to remove.
 * @return 1 if product was successfully removed (or bundle deleted), 0 on
 * failure (e.g. product not in bundle).
 */
int remove_product_from_bundle(Bundle bundles[], int *bundle_count,
                               Bundle *bundle, int product_id);

/**
 * @brief Calculates the total price of a bundle after applying its discount.
 * @param bundle Pointer to the bundle to calculate.
 * @param products Array of all products in inventory for lookup.
 * @param product_count Current number of active products.
 * @return The calculated discounted bundle price.
 */
float calculate_bundle_price(const Bundle *bundle, const Product products[],
                             int product_count);

/**
 * @brief Calculates the virtual stock of a bundle based on the minimum stock of
 * its products.
 * @param bundle Pointer to the bundle to query.
 * @param products Array of all products in inventory for lookup.
 * @param product_count Current number of active products.
 * @return The virtual stock of the bundle, or 0 if missing any product or if
 * the bundle is empty.
 */
int get_virtual_bundle_stock(const Bundle *bundle, const Product products[],
                             int product_count);

/**
 * @brief Prints details of all bundles in the catalog in a formatted table.
 * @param bundles Array of bundles.
 * @param count Current number of active bundles.
 * @param products Array of all products in inventory.
 * @param product_count Current number of active products.
 */
void display_all_bundles(const Bundle bundles[], int count,
                         const Product products[], int product_count);

/**
 * @brief Searches for a bundle in the inventory array by its unique ID.
 * @param bundles Array of bundle structures.
 * @param count Current number of active bundles.
 * @param id The ID of the bundle to search for.
 * @return The array index of the matching bundle, or -1 if not found.
 */
int find_bundle_by_id(const Bundle bundles[], int count, int id);

#endif

#ifndef ORDER_H
#define ORDER_H

/**
 * @brief Validates and places a new order.
 *
 * Verifies that the requested product or bundle ID exists, checks that
 * stock levels are sufficient, deducts the stock, calculates the total price,
 * stamps the order date, generates an order ID if needed, and inserts the
 * order into the orders array.
 *
 * @param orders Array of order structures.
 * @param count Pointer to the current number of active orders. Will be
 * incremented on success.
 * @param products Array of product structures.
 * @param product_count Current number of active products.
 * @param bundles Array of bundle structures.
 * @param bundle_count Current number of active bundles.
 * @param new_order Pointer to the order containing details of the order
 * request.
 * @return 1 if the order is successfully placed, 0 otherwise.
 */
int create_order(Order orders[], int *count, Product products[],
                 int product_count, const Bundle bundles[], int bundle_count,
                 const Order *new_order);

/**
 * @brief Displays the history of all placed orders in a structured table.
 *
 * @param orders Array of order structures.
 * @param count Current number of active orders.
 */
void display_order_history(const Order orders[], int count);

/**
 * @brief Prints a revenue report summarizing the total sales.
 *
 * @param orders Array of order structures.
 * @param order_count Current number of active orders.
 * @param products Array of product structures.
 * @param product_count Current number of active products.
 * @param bundles Array of bundle structures.
 * @param bundle_count Current number of active bundles.
 */
void print_revenue_report(const Order orders[], int order_count,
                          const Product products[], int product_count,
                          const Bundle bundles[], int bundle_count);

void print_best_seller_products(const Order orders[], int order_count,
                                const Product products[], int product_count,
                                const Bundle bundles[], int bundle_count);

void print_best_seller_bundles(const Order orders[], int order_count,
                               const Bundle bundles[], int bundle_count);

void alert_low_stock(const Product products[], int product_count);
#endif

#ifndef FILE_IO_H
#define FILE_IO_H

/**
 * @brief Saves the database containing products, bundles, orders, and admin
 * credentials.
 * @param products Array of current products.
 * @param product_count Number of active products.
 * @param bundles Array of current bundles.
 * @param bundle_count Number of active bundles.
 * @param orders Array of current orders.
 * @param order_count Number of active orders.
 * @param admin Pointer to the admin credentials.
 * @return 1 on success, 0 on failure.
 */
int save_database(const Product products[], int product_count,
                  const Bundle bundles[], int bundle_count,
                  const Order orders[], int order_count,
                  const AdminCredentials *admin);

/**
 * @brief Loads the database containing products, bundles, orders, and admin
 * credentials.
 * @param products Array to populate with products.
 * @param product_count Pointer to variable storing loaded products count.
 * @param bundles Array to populate with bundles.
 * @param bundle_count Pointer to variable storing loaded bundles count.
 * @param orders Array to populate with orders.
 * @param order_count Pointer to variable storing loaded orders count.
 * @param admin Pointer to the admin credentials structure to populate.
 * @return 1 on success, 0 on failure.
 */
int load_database(Product products[], int *product_count, Bundle bundles[],
                  int *bundle_count, Order orders[], int *order_count,
                  AdminCredentials *admin);

#endif

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Prompts the user and reads a safe integer value.
 * @param prompt The string prompt to show to the user.
 * @param out_value Pointer to integer where the parsed value will be stored.
 * @return 1 if successfully parsed a valid integer, 0 if canceled by typing
 * '0'.
 */
int get_safe_int(const char *prompt, int *out_value);

/**
 * @brief Prompts the user and reads a safe floating-point value.
 * @param prompt The string prompt to show to the user.
 * @param out_value Pointer to float where the parsed value will be stored.
 * @return 1 if successfully parsed a valid float, 0 if canceled by typing '0'.
 */
int get_safe_float(const char *prompt, float *out_value);

/**
 * @brief Prompts the user and reads a safe string up to max_len - 1 characters.
 * @param prompt The string prompt to show to the user.
 * @param out_str Character buffer where the string will be stored.
 * @param max_len Size of the output buffer.
 */
void get_safe_string(const char *prompt, char *out_str, int max_len);

/**
 * @brief Clears the standard input buffer up to newline or EOF.
 */
void clear_input_buffer(void);

/**
 * @brief Converts a string in-place to all lowercase letters.
 * @param str The string to convert.
 */
void to_lowercase(char *str);

/**
 * @brief Reads a string of characters from stdin up to a given size limit.
 * @param str Character buffer where the string will be stored.
 * @param size Size of the output buffer.
 */
void input_string(char *str, int size);

/**
 * @brief Halts program execution until the user presses Enter.
 */
void cont(void);

/**
 * @brief Prompts the user for a confirmation and returns their choice.
 * @param message The confirmation message to display.
 * @return 1 if the user confirmed (Y/y), 0 otherwise.
 */
int confirm_action(const char *message);

#endif

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

#endif

static int validate_credential_string(const char *str, int max_len) {
  int len = (int)strlen(str);
  if (len < 4 || len >= max_len) {
    return 0;
  }
  for (int i = 0; i < len; i++) {
    if (isspace((unsigned char)str[i])) {
      return 0;
    }
  }
  return 1;
}

int is_admin_setup_complete(void) {
  FILE *f = fopen("admin.txt", "r");
  if (f != NULL) {
    fclose(f);
    return 1;
  }
  return 0;
}

int setup_admin_credentials(const char *username, const char *password) {
  if (username == NULL || password == NULL) {
    return 0;
  }
  FILE *f = fopen("admin.txt", "w");
  if (f == NULL) {
    return 0;
  }
  fprintf(f, "%s\n", username);
  for (int i = 0; password[i] != '\0'; i++) {
    unsigned char c = (unsigned char)password[i];
    unsigned char obfuscated = (unsigned char)(c ^ 0x5A);
    fprintf(f, "%02X", obfuscated);
  }
  fprintf(f, "\n");
  fclose(f);
  return 1;
}

int verify_admin_login(const char *username, const char *password) {
  if (username == NULL || password == NULL) {
    return 0;
  }
  FILE *f = fopen("admin.txt", "r");
  if (f == NULL) {
    return 0;
  }

  char stored_user[MAX_USERNAME_LEN] = {0};
  char stored_hex[(MAX_PASSWORD_LEN * 2) + 2] = {0};

  if (fgets(stored_user, (int)sizeof(stored_user), f) == NULL) {
    fclose(f);
    return 0;
  }
  int user_idx = (int)strcspn(stored_user, "\r\n");
  if (user_idx < (int)sizeof(stored_user)) {
    stored_user[user_idx] = '\0';
  }

  if (fgets(stored_hex, (int)sizeof(stored_hex), f) == NULL) {
    fclose(f);
    return 0;
  }
  int hex_idx = (int)strcspn(stored_hex, "\r\n");
  if (hex_idx < (int)sizeof(stored_hex)) {
    stored_hex[hex_idx] = '\0';
  }
  fclose(f);

  if (strcmp(stored_user, username) != 0) {
    return 0;
  }

  char input_hex[(MAX_PASSWORD_LEN * 2) + 1] = {0};
  int input_len = (int)strlen(password);
  if (input_len >= MAX_PASSWORD_LEN) {
    return 0;
  }
  for (int i = 0; i < input_len; i++) {
    unsigned char c = (unsigned char)password[i];
    unsigned char obfuscated = (unsigned char)(c ^ 0x5A);
    sprintf(input_hex + (size_t)(i * 2), "%02X", obfuscated);
  }

  if (strcmp(stored_hex, input_hex) == 0) {
    return 1;
  }
  return 0;
}

void run_admin_registration_wizard(void) {
  char username[128];
  char password[128];

  while (1) {
    get_safe_string("Register Admin Username (0 to cancel): ", username,
                    sizeof(username));
    if (strcmp(username, "0") == 0) {
      printf("Error: Setup is mandatory! You cannot cancel or bypass this "
             "wizard.\n");
      continue;
    }

    if (!validate_credential_string(username, MAX_USERNAME_LEN)) {
      printf("Invalid entry, please try again.\n");
      continue;
    }
    break;
  }

  while (1) {
    get_safe_string("Register Admin Password: ", password, sizeof(password));

    if (!validate_credential_string(password, MAX_PASSWORD_LEN)) {
      printf("Invalid entry, please try again.\n");
      continue;
    }
    break;
  }

  if (setup_admin_credentials(username, password)) {
    printf("Admin registration successful!\n");
  } else {
    printf("Error: Failed to save admin credentials!\n");
  }
}

int run_admin_login_loop(void) {
  char username[128];
  char password[128];
  int attempts = 0;

  while (attempts < 3) {
    get_safe_string("Enter Admin Username (0 to cancel): ", username,
                    sizeof(username));
    if (strcmp(username, "0") == 0) {
      return 0;
    }

    get_safe_string("Enter Admin Password (0 to cancel): ", password,
                    sizeof(password));
    if (strcmp(password, "0") == 0) {
      return 0;
    }

    if (verify_admin_login(username, password)) {
      printf("Login successful!\n");
      return 1;
    }

    attempts++;
    printf("Invalid username or password.\n");
  }

  printf("Too many failed attempts! Returning to Customer Menu.\n");
  return 0;
}

int create_bundle(Bundle bundles[], int *count, const Bundle *new_bundle) {
  if (bundles == NULL || count == NULL || new_bundle == NULL) {
    return 0;
  }
  if (*count >= MAX_BUNDLES) {
    printf("Error: Maximum bundle limit reached (%d).\n", MAX_BUNDLES);
    cont();
    return 0;
  }
  if (new_bundle->discount_rate < 0.0F || new_bundle->discount_rate > 1.0F) {
    printf("Discount rate must be between 0.0 and 1.0.\n");
    cont();
    return 0;
  }
  if (new_bundle->product_count < 1) {
    printf("Error: A bundle must contain at least 1 product.\n");
    cont();
    return 0;
  }
  if (new_bundle->product_count > MAX_BUNDLE_ITEMS) {
    printf("Error: A bundle can only contain up to %d products.\n",
           MAX_BUNDLE_ITEMS);
    cont();
    return 0;
  }
  for (int i = 0; i < new_bundle->product_count; i++) {
    for (int j = i + 1; j < new_bundle->product_count; j++) {
      if (new_bundle->product_ids[i] == new_bundle->product_ids[j]) {
        printf("Error: Duplicate product ID %d in bundle.\n",
               new_bundle->product_ids[i]);
        cont();
        return 0;
      }
    }
  }

  bundles[*count] = *new_bundle;
  (*count)++;
  return 1;
}

int add_product_to_bundle(Bundle *bundle, int product_id) {
  if (bundle == NULL) {
    return 0;
  }
  if (bundle->product_count >= MAX_BUNDLE_ITEMS) {
    printf("Maximum limit reached: You can only add up to %d products to a "
           "bundle.\n",
           MAX_BUNDLE_ITEMS);
    cont();
    return 0;
  }
  for (int i = 0; i < bundle->product_count; i++) {
    if (bundle->product_ids[i] == product_id) {
      printf("This product is already in the bundle.\n");
      cont();
      return 0;
    }
  }
  bundle->product_ids[bundle->product_count] = product_id;
  bundle->product_count++;
  return 1;
}

int remove_product_from_bundle(Bundle bundles[], int *bundle_count,
                               Bundle *bundle, int product_id) {
  if (bundles == NULL || bundle_count == NULL || bundle == NULL) {
    return 0;
  }
  int cnt = bundle->product_count;
  int index = -1;
  for (int i = 0; i < cnt; i++) {
    if (bundle->product_ids[i] == product_id) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    printf("The bundle does not contain this product.\n");
    cont();
    return 0;
  }
  if (cnt == 1) {
    if (confirm_action(
            "This action will also delete the bundle. Do you want to "
            "continue?")) {
      int delete_idx = -1;
      for (int i = 0; i < *bundle_count; i++) {
        if (bundles[i].bundle_id == bundle->bundle_id) {
          delete_idx = i;
          break;
        }
      }
      if (delete_idx != -1) {
        for (int i = delete_idx; i < *bundle_count - 1; i++) {
          bundles[i] = bundles[i + 1];
        }
        (*bundle_count)--;
        return 1;
      }
    }
    return 0;
  }
  for (int i = index; i < cnt - 1; i++) {
    bundle->product_ids[i] = bundle->product_ids[i + 1];
  }
  bundle->product_count--;
  return 1;
}
float calculate_bundle_price(const Bundle *bundle, const Product products[],
                             int product_count) {
  if (bundle == NULL || products == NULL || product_count < 0) {
    return 0.0F;
  }
  float sum = 0.0F;
  for (int i = 0; i < bundle->product_count; i++) {
    int prod_id = bundle->product_ids[i];
    int prod_idx = find_product_by_id(products, product_count, prod_id);
    if (prod_idx != -1) {
      sum += products[prod_idx].price;
    }
  }
  return sum * (1.0F - bundle->discount_rate);
}

int get_virtual_bundle_stock(const Bundle *bundle, const Product products[],
                             int product_count) {
  if (bundle == NULL || products == NULL || product_count < 0) {
    return 0;
  }
  if (bundle->product_count <= 0) {
    return 0;
  }
  int min_stock = -1;
  for (int i = 0; i < bundle->product_count; i++) {
    int prod_id = bundle->product_ids[i];
    int prod_idx = find_product_by_id(products, product_count, prod_id);
    if (prod_idx == -1) {
      return 0;
    }
    int stock = products[prod_idx].stock_quantity;
    if (min_stock == -1 || stock < min_stock) {
      min_stock = stock;
    }
  }
  return (min_stock == -1) ? 0 : min_stock;
}

void display_all_bundles(const Bundle bundles[], int count,
                         const Product products[], int product_count) {
  if (bundles == NULL || products == NULL || count < 0 || product_count < 0) {
    return;
  }
  if (count == 0) {
    printf("\nNo bundles available in the system.\n");
    cont();
    return;
  }
  char border[] = "+----+---------------------------+-------------+------------"
                  "---+---------------+";

  printf("\n%s\n", border);
  printf("| %-2s | %-25s | %-11s | %-13s | %-13s |\n", "ID", "Bundle Name",
         "Disc. Rate", "Price (Final)", "Virt. Stock");
  printf("%s\n", border);

  for (int i = 0; i < count; i++) {
    float final_price =
        calculate_bundle_price(&bundles[i], products, product_count);
    int virtual_stock =
        get_virtual_bundle_stock(&bundles[i], products, product_count);

    float discount_percentage = bundles[i].discount_rate * 100.0F;

    printf("| %-2d | %-25s | %-10.1f%% | %-13.2f | %-13d |\n",
           bundles[i].bundle_id, bundles[i].bundle_name,
           (double)discount_percentage, (double)final_price, virtual_stock);
  }

  printf("%s\n", border);
  cont();
}

int find_bundle_by_id(const Bundle bundles[], int count, int id) {
  if (bundles == NULL || count < 0 || id <= 0) {
    return -1;
  }
  for (int i = 0; i < count; i++) {
    if (bundles[i].bundle_id == id) {
      return i;
    }
  }
  return -1;
}

static void strip_newline(char *str) {
  size_t len = strlen(str);
  while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
    str[len - 1] = '\0';
    len--;
  }
}

static int read_line_file(FILE *f, char *dest, size_t max_len) {
  if (fgets(dest, (int)max_len, f) == NULL) {
    return 0;
  }
  strip_newline(dest);
  return 1;
}

static int read_int_file(FILE *f, int *out) {
  char buf[128];
  if (!read_line_file(f, buf, sizeof(buf))) {
    return 0;
  }
  char *endptr;
  long val = strtol(buf, &endptr, 10);
  if (endptr == buf || *endptr != '\0') {
    return 0;
  }
  *out = (int)val;
  return 1;
}

static int read_float_file(FILE *f, float *out) {
  char buf[128];
  if (!read_line_file(f, buf, sizeof(buf))) {
    return 0;
  }
  char *endptr;
  double val = strtod(buf, &endptr);
  if (endptr == buf || *endptr != '\0') {
    return 0;
  }
  *out = (float)val;
  return 1;
}

static int is_file_non_empty(const char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    return 0;
  }
  if (fseek(f, 0, SEEK_END) != 0) {
    fclose(f);
    return 0;
  }
  long size = ftell(f);
  fclose(f);
  return size > 0 ? 1 : 0;
}

static int commit_transaction(const char *active_path, const char *tmp_path,
                              const char *bak_path) {
  int active_existed = 0;
  FILE *f = fopen(active_path, "r");
  if (f != NULL) {
    active_existed = 1;
    fclose(f);
    remove(bak_path);
    if (rename(active_path, bak_path) != 0) {
      return 0;
    }
  }
  remove(active_path);
  if (rename(tmp_path, active_path) != 0) {
    if (active_existed != 0) {
      (void)rename(bak_path, active_path);
    }
    return 0;
  }
  return 1;
}

static int ensure_active_file(const char *active_path, const char *bak_path) {
  FILE *f = fopen(active_path, "r");
  if (f != NULL) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    if (size > 0) {
      return 1;
    }
  }

  FILE *fbak = fopen(bak_path, "r");
  if (fbak != NULL) {
    fclose(fbak);
    printf("Warning: Active database file '%s' was missing or corrupt. "
           "Restoring from backup '%s'.\n",
           active_path, bak_path);
    remove(active_path);

    FILE *src = fopen(bak_path, "r");
    if (src == NULL) {
      return 0;
    }
    FILE *dst = fopen(active_path, "w");
    if (dst == NULL) {
      fclose(src);
      return 0;
    }
    int ch;
    while ((ch = fgetc(src)) != EOF) {
      if (fputc(ch, dst) == EOF) {
        fclose(src);
        fclose(dst);
        return 0;
      }
    }
    fclose(src);
    fclose(dst);
    return 1;
  }
  return 0;
}

static int load_products_internal(Product products[], int *product_count,
                                  const char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    *product_count = 0;
    return 1;
  }
  int count = 0;
  if (!read_int_file(f, &count)) {
    fclose(f);
    return 0;
  }
  if (count < 0 || count > MAX_PRODUCTS) {
    fclose(f);
    return 0;
  }
  for (int i = 0; i < count; i++) {
    Product p;
    memset(&p, 0, sizeof(Product));
    if (!read_int_file(f, &p.product_id) ||
        !read_line_file(f, p.product_name, sizeof(p.product_name)) ||
        !read_line_file(f, p.category, sizeof(p.category)) ||
        !read_line_file(f, p.brand, sizeof(p.brand)) ||
        !read_float_file(f, &p.price) || !read_int_file(f, &p.stock_quantity)) {
      fclose(f);
      return 0;
    }
    products[i] = p;
  }
  *product_count = count;
  fclose(f);
  return 1;
}

static int load_bundles_internal(Bundle bundles[], int *bundle_count,
                                 const char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    *bundle_count = 0;
    return 1;
  }
  int count = 0;
  if (!read_int_file(f, &count)) {
    fclose(f);
    return 0;
  }
  if (count < 0 || count > MAX_BUNDLES) {
    fclose(f);
    return 0;
  }
  for (int i = 0; i < count; i++) {
    Bundle b;
    memset(&b, 0, sizeof(Bundle));
    if (!read_int_file(f, &b.bundle_id) ||
        !read_line_file(f, b.bundle_name, sizeof(b.bundle_name)) ||
        !read_float_file(f, &b.discount_rate) ||
        !read_float_file(f, &b.bundle_price) ||
        !read_int_file(f, &b.product_count)) {
      fclose(f);
      return 0;
    }
    if (b.product_count < 0 || b.product_count > MAX_BUNDLE_ITEMS) {
      fclose(f);
      return 0;
    }
    for (int j = 0; j < b.product_count; j++) {
      if (!read_int_file(f, &b.product_ids[j])) {
        fclose(f);
        return 0;
      }
    }
    bundles[i] = b;
  }
  *bundle_count = count;
  fclose(f);
  return 1;
}

static int load_orders_internal(Order orders[], int *order_count,
                                const char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    *order_count = 0;
    return 1;
  }
  int count = 0;
  if (!read_int_file(f, &count)) {
    fclose(f);
    return 0;
  }
  if (count < 0 || count > MAX_ORDERS) {
    fclose(f);
    return 0;
  }
  for (int i = 0; i < count; i++) {
    Order o;
    memset(&o, 0, sizeof(Order));
    if (!read_int_file(f, &o.order_id) ||
        !read_line_file(f, o.customer_name, sizeof(o.customer_name)) ||
        !read_int_file(f, &o.item_id) || !read_int_file(f, &o.is_bundle) ||
        !read_int_file(f, &o.quantity) || !read_float_file(f, &o.total_price) ||
        !read_line_file(f, o.order_date, sizeof(o.order_date))) {
      fclose(f);
      return 0;
    }
    orders[i] = o;
  }
  *order_count = count;
  fclose(f);
  return 1;
}

static int load_admin_internal(AdminCredentials *admin, const char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    admin->is_setup = 0;
    memset(admin->username, 0, sizeof(admin->username));
    memset(admin->password, 0, sizeof(admin->password));
    return 1;
  }
  AdminCredentials a;
  memset(&a, 0, sizeof(AdminCredentials));
  if (!read_line_file(f, a.username, sizeof(a.username)) ||
      !read_line_file(f, a.password, sizeof(a.password))) {
    fclose(f);
    return 0;
  }
  a.is_setup = 1;
  *admin = a;
  fclose(f);
  return 1;
}

int save_database(const Product products[], int product_count,
                  const Bundle bundles[], int bundle_count,
                  const Order orders[], int order_count,
                  const AdminCredentials *admin) {
  if (products == NULL || product_count < 0 || product_count > MAX_PRODUCTS) {
    return 0;
  }
  if (bundles == NULL || bundle_count < 0 || bundle_count > MAX_BUNDLES) {
    return 0;
  }
  if (orders == NULL || order_count < 0 || order_count > MAX_ORDERS) {
    return 0;
  }
  if (admin == NULL) {
    return 0;
  }

  FILE *fp = fopen("products.tmp", "w");
  if (fp == NULL) {
    return 0;
  }
  fprintf(fp, "%d\n", product_count);
  for (int i = 0; i < product_count; i++) {
    fprintf(fp, "%d\n", products[i].product_id);
    fprintf(fp, "%s\n", products[i].product_name);
    fprintf(fp, "%s\n", products[i].category);
    fprintf(fp, "%s\n", products[i].brand);
    fprintf(fp, "%.2f\n", (double)products[i].price);
    fprintf(fp, "%d\n", products[i].stock_quantity);
  }
  fclose(fp);
  if (!is_file_non_empty("products.tmp")) {
    remove("products.tmp");
    return 0;
  }

  FILE *fb = fopen("bundles.tmp", "w");
  if (fb == NULL) {
    remove("products.tmp");
    return 0;
  }
  fprintf(fb, "%d\n", bundle_count);
  for (int i = 0; i < bundle_count; i++) {
    fprintf(fb, "%d\n", bundles[i].bundle_id);
    fprintf(fb, "%s\n", bundles[i].bundle_name);
    fprintf(fb, "%.4f\n", (double)bundles[i].discount_rate);
    fprintf(fb, "%.2f\n", (double)bundles[i].bundle_price);
    fprintf(fb, "%d\n", bundles[i].product_count);
    for (int j = 0; j < bundles[i].product_count; j++) {
      fprintf(fb, "%d\n", bundles[i].product_ids[j]);
    }
  }
  fclose(fb);
  if (!is_file_non_empty("bundles.tmp")) {
    remove("products.tmp");
    remove("bundles.tmp");
    return 0;
  }

  FILE *fo = fopen("orders.tmp", "w");
  if (fo == NULL) {
    remove("products.tmp");
    remove("bundles.tmp");
    return 0;
  }
  fprintf(fo, "%d\n", order_count);
  for (int i = 0; i < order_count; i++) {
    fprintf(fo, "%d\n", orders[i].order_id);
    fprintf(fo, "%s\n", orders[i].customer_name);
    fprintf(fo, "%d\n", orders[i].item_id);
    fprintf(fo, "%d\n", orders[i].is_bundle);
    fprintf(fo, "%d\n", orders[i].quantity);
    fprintf(fo, "%.2f\n", (double)orders[i].total_price);
    fprintf(fo, "%s\n", orders[i].order_date);
  }
  fclose(fo);
  if (!is_file_non_empty("orders.tmp")) {
    remove("products.tmp");
    remove("bundles.tmp");
    remove("orders.tmp");
    return 0;
  }

  FILE *fa = fopen("admin.tmp", "w");
  if (fa == NULL) {
    remove("products.tmp");
    remove("bundles.tmp");
    remove("orders.tmp");
    return 0;
  }
  if (admin->username[0] == '\0') {
    FILE *f_exist = fopen("admin.txt", "r");
    if (f_exist != NULL) {
      char temp_user[MAX_USERNAME_LEN] = {0};
      char temp_pass[(MAX_PASSWORD_LEN * 2) + 2] = {0};
      if (fgets(temp_user, (int)sizeof(temp_user), f_exist) != NULL &&
          fgets(temp_pass, (int)sizeof(temp_pass), f_exist) != NULL) {
        fprintf(fa, "%s", temp_user);
        fprintf(fa, "%s", temp_pass);
      } else {
        fprintf(fa, "\n\n");
      }
      fclose(f_exist);
    } else {
      fprintf(fa, "\n\n");
    }
  } else {
    fprintf(fa, "%s\n", admin->username);
    fprintf(fa, "%s\n", admin->password);
  }
  fclose(fa);
  if (!is_file_non_empty("admin.tmp")) {
    remove("products.tmp");
    remove("bundles.tmp");
    remove("orders.tmp");
    remove("admin.tmp");
    return 0;
  }

  int success = 1;
  if (!commit_transaction("products.txt", "products.tmp", "products.bak")) {
    success = 0;
  }
  if (!commit_transaction("bundles.txt", "bundles.tmp", "bundles.bak")) {
    success = 0;
  }
  if (!commit_transaction("orders.txt", "orders.tmp", "orders.bak")) {
    success = 0;
  }
  if (!commit_transaction("admin.txt", "admin.tmp", "admin.bak")) {
    success = 0;
  }

  return success;
}

int load_database(Product products[], int *product_count, Bundle bundles[],
                  int *bundle_count, Order orders[], int *order_count,
                  AdminCredentials *admin) {
  if (products == NULL || product_count == NULL || bundles == NULL ||
      bundle_count == NULL || orders == NULL || order_count == NULL ||
      admin == NULL) {
    return 0;
  }

  int overall_success = 1;

  (void)ensure_active_file("products.txt", "products.bak");
  if (!load_products_internal(products, product_count, "products.txt")) {
    printf(
        "Warning: 'products.txt' load failed or corrupt. Restoring backup.\n");
    remove("products.txt");
    FILE *src = fopen("products.bak", "r");
    if (src != NULL) {
      FILE *dst = fopen("products.txt", "w");
      if (dst != NULL) {
        int ch;
        while ((ch = fgetc(src)) != EOF) {
          (void)fputc(ch, dst);
        }
        fclose(dst);
      }
      fclose(src);
    }
    if (!load_products_internal(products, product_count, "products.txt")) {
      *product_count = 0;
      overall_success = 0;
    }
  }

  (void)ensure_active_file("bundles.txt", "bundles.bak");
  if (!load_bundles_internal(bundles, bundle_count, "bundles.txt")) {
    printf(
        "Warning: 'bundles.txt' load failed or corrupt. Restoring backup.\n");
    remove("bundles.txt");
    FILE *src = fopen("bundles.bak", "r");
    if (src != NULL) {
      FILE *dst = fopen("bundles.txt", "w");
      if (dst != NULL) {
        int ch;
        while ((ch = fgetc(src)) != EOF) {
          (void)fputc(ch, dst);
        }
        fclose(dst);
      }
      fclose(src);
    }
    if (!load_bundles_internal(bundles, bundle_count, "bundles.txt")) {
      *bundle_count = 0;
      overall_success = 0;
    }
  }

  (void)ensure_active_file("orders.txt", "orders.bak");
  if (!load_orders_internal(orders, order_count, "orders.txt")) {
    printf("Warning: 'orders.txt' load failed or corrupt. Restoring backup.\n");
    remove("orders.txt");
    FILE *src = fopen("orders.bak", "r");
    if (src != NULL) {
      FILE *dst = fopen("orders.txt", "w");
      if (dst != NULL) {
        int ch;
        while ((ch = fgetc(src)) != EOF) {
          (void)fputc(ch, dst);
        }
        fclose(dst);
      }
      fclose(src);
    }
    if (!load_orders_internal(orders, order_count, "orders.txt")) {
      *order_count = 0;
      overall_success = 0;
    }
  }

  (void)ensure_active_file("admin.txt", "admin.bak");
  if (!load_admin_internal(admin, "admin.txt")) {
    printf("Warning: 'admin.txt' load failed or corrupt. Restoring backup.\n");
    remove("admin.txt");
    FILE *src = fopen("admin.bak", "r");
    if (src != NULL) {
      FILE *dst = fopen("admin.txt", "w");
      if (dst != NULL) {
        int ch;
        while ((ch = fgetc(src)) != EOF) {
          (void)fputc(ch, dst);
        }
        fclose(dst);
      }
      fclose(src);
    }
    if (!load_admin_internal(admin, "admin.txt")) {
      admin->is_setup = 0;
      memset(admin->username, 0, sizeof(admin->username));
      memset(admin->password, 0, sizeof(admin->password));
      overall_success = 0;
    }
  }

  return overall_success;
}

static Product products[MAX_PRODUCTS];
static int product_count = 0;
static Bundle bundles[MAX_BUNDLES];
static int bundle_count = 0;
static Order orders[MAX_ORDERS];
static int order_count = 0;
static AdminCredentials admin;

void display_customer_menu(void) {
  int choice = -1;
  while (1) {
    printf("\n==========================================\n");
    printf("              CUSTOMER MENU               \n");
    printf("==========================================\n");
    printf("1. Browse Catalog\n");
    printf("2. Search/Sort Products\n");
    printf("3. Place Order\n");
    printf("4. Switch to Admin Mode\n");
    printf("0. Exit\n");
    printf("------------------------------------------\n");

    if (get_safe_int("Enter selection: ", &choice) == 0) {
      printf("Goodbye!\n");
      break;
    }

    if (choice == 0) {
      printf("Goodbye!\n");
      break;
    }

    switch (choice) {
    case 1:
      printf("\n--- Product Catalog ---\n");
      display_all_products(products, product_count);
      printf("\n--- Bundle Catalog ---\n");
      display_all_bundles(bundles, bundle_count, products, product_count);
      break;
    case 2: {
      int search_choice = -1;
      while (1) {
        printf("\n--- Search & Filter Products ---\n");
        printf("1. Search by Name\n");
        printf("2. Filter by Category\n");
        printf("3. Filter by Price Range\n");
        printf("4. Sort by Price (Ascending)\n");
        printf("5. Sort by Price (Descending)\n");
        printf("6. Sort by Name (A-Z)\n");
        printf("0. Back\n");
        printf("--------------------------------\n");

        if (get_safe_int("Enter selection: ", &search_choice) == 0 ||
            search_choice == 0) {
          break;
        }

        switch (search_choice) {
        case 1: {
          char query[MAX_NAME_LEN] = {0};
          get_safe_string("Enter search query (0 to cancel): ", query,
                          MAX_NAME_LEN);
          if (strcmp(query, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }
          search_product_by_name(products, product_count, query);
          break;
        }
        case 2: {
          char category[MAX_CAT_LEN] = {0};
          get_safe_string("Enter category (0 to cancel): ", category,
                          MAX_CAT_LEN);
          if (strcmp(category, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }
          filter_product_by_category(products, product_count, category);
          break;
        }
        case 3: {
          float min_price = 0.0F;
          float max_price = 0.0F;
          if (get_safe_float("Enter min price (0 to cancel): ", &min_price) ==
              0) {
            printf("Action cancelled.\n");
            break;
          }
          if (get_safe_float("Enter max price (0 to cancel): ", &max_price) ==
              0) {
            printf("Action cancelled.\n");
            break;
          }
          filter_product_by_price(products, product_count, min_price,
                                  max_price);
          break;
        }
        case 4: {
          Product temp_list[MAX_PRODUCTS];
          memcpy(temp_list, products, (size_t)product_count * sizeof(Product));
          printf("\n--- Products Sorted by Price (Ascending) ---\n");
          bubble_sort_by_price(temp_list, product_count, 1);
          break;
        }
        case 5: {
          Product temp_list[MAX_PRODUCTS];
          memcpy(temp_list, products, (size_t)product_count * sizeof(Product));
          printf("\n--- Products Sorted by Price (Descending) ---\n");
          bubble_sort_by_price(temp_list, product_count, 0);
          break;
        }
        case 6: {
          Product temp_list[MAX_PRODUCTS];
          memcpy(temp_list, products, (size_t)product_count * sizeof(Product));
          printf("\n--- Products Sorted by Name (Alphabetical) ---\n");
          bubble_sort_by_name(temp_list, product_count);
          break;
        }
        default:
          printf("Invalid option. Please try again.\n");
          break;
        }
      }
      break;
    }
    case 3: {
      printf("\n--- Place Order ---\n");
      Order new_ord;
      memset(&new_ord, 0, sizeof(new_ord));

      get_safe_string("Enter Customer Name (0 to cancel): ",
                      new_ord.customer_name, MAX_NAME_LEN);
      if (strcmp(new_ord.customer_name, "0") == 0) {
        printf("Action cancelled.\n");
        break;
      }

      int is_bundle_choice = -1;
      int cancelled = 0;
      while (1) {
        if (get_safe_int("Is this a Bundle order? (1 for Bundle, 2 for "
                         "Product, 0 to cancel): ",
                         &is_bundle_choice) == 0 ||
            is_bundle_choice == 0) {
          printf("Action cancelled.\n");
          cancelled = 1;
          break;
        }
        if (is_bundle_choice == 1 || is_bundle_choice == 2) {
          new_ord.is_bundle = (is_bundle_choice == 1) ? 1 : 0;
          break;
        }
        printf("Invalid entry, please try again.\n");
      }
      if (cancelled != 0) {
        break;
      }

      if (get_safe_int("Enter Item ID to order (0 to cancel): ",
                       &new_ord.item_id) == 0 ||
          new_ord.item_id == 0) {
        printf("Action cancelled.\n");
        break;
      }

      if (get_safe_int("Enter Quantity (0 to cancel): ", &new_ord.quantity) ==
              0 ||
          new_ord.quantity == 0) {
        printf("Action cancelled.\n");
        break;
      }

      if (create_order(orders, &order_count, products, product_count, bundles,
                       bundle_count, &new_ord) != 0) {
        printf("Order placed successfully!\n");
      } else {
        printf("Failed to place order due to validation errors.\n");
      }
      cont();
      break;
    }
    case 4: {
      printf("\n--- Admin Login ---\n");
      char username[MAX_USERNAME_LEN] = {0};
      char password[MAX_PASSWORD_LEN] = {0};
      int login_success = 0;
      int attempts = 0;
      while (attempts < 3) {
        get_safe_string("Enter Username (0 to cancel): ", username,
                        MAX_USERNAME_LEN);
        if (strcmp(username, "0") == 0) {
          break;
        }
        get_safe_string("Enter Password (0 to cancel): ", password,
                        MAX_PASSWORD_LEN);
        if (strcmp(password, "0") == 0) {
          break;
        }

        if (verify_admin_login(username, password) != 0) {
          printf("Login successful!\n");
          login_success = 1;
          break;
        }

        attempts++;
        printf("Invalid credentials. Attempt %d/3 failed.\n", attempts);
      }

      if (attempts >= 3) {
        printf("Too many failed attempts. Entry to Admin Mode locked for this "
               "session.\n");
      } else if (login_success != 0) {
        display_admin_menu();
      }
      break;
    }
    default:
      printf("Invalid option. Please try again.\n");
      break;
    }
  }
}

void display_admin_menu(void) {
  int choice = -1;
  while (1) {
    printf("\n==========================================\n");
    printf("               ADMIN MENU                 \n");
    printf("==========================================\n");
    printf("1. Manage Products\n");
    printf("2. Manage Bundles\n");
    printf("3. View Revenue & Reports\n");
    printf("4. Save Changes\n");
    printf("0. Logout\n");
    printf("------------------------------------------\n");

    if (get_safe_int("Enter selection: ", &choice) == 0) {
      printf("Logging out of admin mode...\n");
      break;
    }

    if (choice == 0) {
      printf("Logging out of admin mode...\n");
      break;
    }

    switch (choice) {
    case 1: {
      int prod_choice = -1;
      while (1) {
        printf("\n--- Product Management Sub-Menu ---\n");
        printf("1. View All Products\n");
        printf("2. Add Product\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("0. Back\n");
        printf("-----------------------------------\n");

        if (get_safe_int("Enter selection: ", &prod_choice) == 0 ||
            prod_choice == 0) {
          break;
        }

        switch (prod_choice) {
        case 1:
          display_all_products(products, product_count);
          break;
        case 2: {
          printf("\n--- Add Product ---\n");
          Product new_prod;
          memset(&new_prod, 0, sizeof(new_prod));

          if (get_safe_int("Enter Product ID (0 to cancel): ",
                           &new_prod.product_id) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          get_safe_string("Enter Product Name (0 to cancel): ",
                          new_prod.product_name, MAX_NAME_LEN);
          if (strcmp(new_prod.product_name, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }

          get_safe_string(
              "Enter Category (Phone/Laptop/Tablet/Accessory, 0 to cancel): ",
              new_prod.category, MAX_CAT_LEN);
          if (strcmp(new_prod.category, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }

          get_safe_string("Enter Brand (0 to cancel): ", new_prod.brand,
                          MAX_BRAND_LEN);
          if (strcmp(new_prod.brand, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }

          if (get_safe_float("Enter Price (0 to cancel): ", &new_prod.price) ==
              0) {
            printf("Action cancelled.\n");
            break;
          }

          if (get_safe_int("Enter Stock Quantity (0 to cancel): ",
                           &new_prod.stock_quantity) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          if (add_product(products, &product_count, &new_prod) != 0) {
            printf("Product added successfully!\n");
          } else {
            printf("Failed to add product due to validation errors.\n");
          }
          break;
        }
        case 3: {
          printf("\n--- Update Product ---\n");
          int update_id = 0;
          if (get_safe_int("Enter Product ID to Update (0 to cancel): ",
                           &update_id) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          int index = find_product_by_id(products, product_count, update_id);
          if (index == -1) {
            printf("Error: Product ID %d not found.\n", update_id);
            break;
          }

          Product updated;
          memset(&updated, 0, sizeof(updated));

          if (get_safe_int("Enter New Product ID (0 to cancel): ",
                           &updated.product_id) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          get_safe_string("Enter New Product Name (0 to cancel): ",
                          updated.product_name, MAX_NAME_LEN);
          if (strcmp(updated.product_name, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }

          get_safe_string("Enter New Category (Phone/Laptop/Tablet/Accessory, "
                          "0 to cancel): ",
                          updated.category, MAX_CAT_LEN);
          if (strcmp(updated.category, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }

          get_safe_string("Enter New Brand (0 to cancel): ", updated.brand,
                          MAX_BRAND_LEN);
          if (strcmp(updated.brand, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }

          if (get_safe_float("Enter New Price (0 to cancel): ",
                             &updated.price) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          if (get_safe_int("Enter New Stock Quantity (0 to cancel): ",
                           &updated.stock_quantity) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          if (update_product(products, product_count, update_id, &updated) !=
              0) {
            printf("Product updated successfully!\n");
          } else {
            printf("Failed to update product due to validation errors.\n");
          }
          break;
        }
        case 4: {
          printf("\n--- Delete Product ---\n");
          int delete_id = 0;
          if (get_safe_int("Enter Product ID to Delete (0 to cancel): ",
                           &delete_id) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          if (delete_product(products, &product_count, delete_id, bundles,
                             bundle_count) != 0) {
            printf("Product deleted successfully!\n");
          } else {
            printf("Failed to delete product.\n");
          }
          break;
        }
        default:
          printf("Invalid option. Please try again.\n");
          break;
        }
      }
      break;
    }
    case 2: {
      int bundle_choice = -1;
      while (1) {
        printf("\n--- Bundle Management Sub-Menu ---\n");
        printf("1. View All Bundles\n");
        printf("2. Create Bundle\n");
        printf("3. Add Product to Bundle\n");
        printf("4. Remove Product from Bundle\n");
        printf("0. Back\n");
        printf("----------------------------------\n");

        if (get_safe_int("Enter selection: ", &bundle_choice) == 0 ||
            bundle_choice == 0) {
          break;
        }

        switch (bundle_choice) {
        case 1:
          display_all_bundles(bundles, bundle_count, products, product_count);
          break;
        case 2: {
          printf("\n--- Create Bundle ---\n");
          int b_id = 0;
          if (get_safe_int("Enter Bundle ID (0 to cancel): ", &b_id) == 0 ||
              b_id == 0) {
            printf("Action cancelled.\n");
            break;
          }
          if (find_bundle_by_id(bundles, bundle_count, b_id) != -1) {
            printf("Error: Bundle ID %d already exists.\n", b_id);
            cont();
            break;
          }

          char b_name[MAX_NAME_LEN] = {0};
          get_safe_string("Enter Bundle Name (0 to cancel): ", b_name,
                          MAX_NAME_LEN);
          if (strcmp(b_name, "0") == 0) {
            printf("Action cancelled.\n");
            break;
          }

          float discount_rate = 0.0F;
          if (get_safe_float("Enter Discount Rate (0.0 to 1.0, 0 to cancel): ",
                             &discount_rate) == 0) {
            printf("Action cancelled.\n");
            break;
          }

          int product_ids[MAX_BUNDLE_ITEMS];
          int p_count = 0;
          while (p_count < MAX_BUNDLE_ITEMS) {
            int prod_id = 0;
            printf("Adding product %d/%d to the bundle.\n", p_count + 1,
                   MAX_BUNDLE_ITEMS);
            if (get_safe_int("Enter Product ID (0 to finish/cancel if empty): ",
                             &prod_id) == 0 ||
                prod_id == 0) {
              break;
            }

            int p_idx = find_product_by_id(products, product_count, prod_id);
            if (p_idx == -1) {
              printf("Error: Product ID %d not found in inventory.\n", prod_id);
              continue;
            }

            int already_added = 0;
            for (int k = 0; k < p_count; k++) {
              if (product_ids[k] == prod_id) {
                already_added = 1;
                break;
              }
            }
            if (already_added != 0) {
              printf("Error: Product already added to this bundle.\n");
              continue;
            }

            product_ids[p_count] = prod_id;
            p_count++;
          }

          if (p_count < 1) {
            printf("Error: A bundle must contain at least 1 product.\n");
            cont();
            break;
          }

          Bundle new_b;
          memset(&new_b, 0, sizeof(new_b));
          new_b.bundle_id = b_id;
          strncpy(new_b.bundle_name, b_name, MAX_NAME_LEN - 1);
          new_b.discount_rate = discount_rate;
          new_b.product_count = p_count;
          for (int k = 0; k < p_count; k++) {
            new_b.product_ids[k] = product_ids[k];
          }
          new_b.bundle_price =
              calculate_bundle_price(&new_b, products, product_count);

          if (create_bundle(bundles, &bundle_count, &new_b) != 0) {
            printf("Bundle created successfully!\n");
          } else {
            printf("Failed to create bundle.\n");
          }
          break;
        }
        case 3: {
          printf("\n--- Add Product to Bundle ---\n");
          int b_id = 0;
          if (get_safe_int("Enter Bundle ID (0 to cancel): ", &b_id) == 0 ||
              b_id == 0) {
            printf("Action cancelled.\n");
            break;
          }
          int b_idx = find_bundle_by_id(bundles, bundle_count, b_id);
          if (b_idx == -1) {
            printf("Error: Bundle ID %d not found.\n", b_id);
            cont();
            break;
          }

          if (bundles[b_idx].product_count >= MAX_BUNDLE_ITEMS) {
            printf("Error: Bundle is full.\n");
            cont();
            break;
          }

          int prod_id = 0;
          if (get_safe_int("Enter Product ID to add (0 to cancel): ",
                           &prod_id) == 0 ||
              prod_id == 0) {
            printf("Action cancelled.\n");
            break;
          }

          int p_idx = find_product_by_id(products, product_count, prod_id);
          if (p_idx == -1) {
            printf("Error: Product ID %d not found in inventory.\n", prod_id);
            cont();
            break;
          }

          if (add_product_to_bundle(&bundles[b_idx], prod_id) != 0) {
            bundles[b_idx].bundle_price = calculate_bundle_price(
                &bundles[b_idx], products, product_count);
            printf("Product added to bundle successfully!\n");
          } else {
            printf("Failed to add product to bundle.\n");
          }
          break;
        }
        case 4: {
          printf("\n--- Remove Product from Bundle ---\n");
          int b_id = 0;
          if (get_safe_int("Enter Bundle ID (0 to cancel): ", &b_id) == 0 ||
              b_id == 0) {
            printf("Action cancelled.\n");
            break;
          }
          int b_idx = find_bundle_by_id(bundles, bundle_count, b_id);
          if (b_idx == -1) {
            printf("Error: Bundle ID %d not found.\n", b_id);
            cont();
            break;
          }

          int prod_id = 0;
          if (get_safe_int("Enter Product ID to remove (0 to cancel): ",
                           &prod_id) == 0 ||
              prod_id == 0) {
            printf("Action cancelled.\n");
            break;
          }

          int old_count = bundle_count;
          if (remove_product_from_bundle(bundles, &bundle_count,
                                         &bundles[b_idx], prod_id) != 0) {
            printf("Product removed successfully.\n");
            if (bundle_count == old_count) {
              bundles[b_idx].bundle_price = calculate_bundle_price(
                  &bundles[b_idx], products, product_count);
            }
          } else {
            printf("Failed to remove product.\n");
          }
          break;
        }
        default:
          printf("Invalid option. Please try again.\n");
          break;
        }
      }
      break;
    }
    case 3: {
      int report_choice = -1;
      while (1) {
        printf("\n--- Revenue & Reports Sub-Menu ---\n");
        printf("1. View Revenue Report\n");
        printf("2. View Best-Selling Product\n");
        printf("3. View Best-Selling Bundle\n");
        printf("4. View Low-Stock Warning Alerts\n");
        printf("0. Back\n");
        printf("----------------------------------\n");

        if (get_safe_int("Enter selection: ", &report_choice) == 0 ||
            report_choice == 0) {
          break;
        }

        switch (report_choice) {
        case 1:
          print_revenue_report(orders, order_count, products, product_count,
                               bundles, bundle_count);
          break;
        case 2:
          print_best_seller_products(orders, order_count, products,
                                     product_count, bundles, bundle_count);
          break;
        case 3:
          print_best_seller_bundles(orders, order_count, bundles, bundle_count);
          break;
        case 4:
          alert_low_stock(products, product_count);
          break;
        default:
          printf("Invalid option. Please try again.\n");
          break;
        }
      }
      break;
    }
    case 4:
      if (save_database(products, product_count, bundles, bundle_count, orders,
                        order_count, &admin) != 0) {
        printf("Database saved successfully!\n");
      } else {
        printf("Database save completed.\n");
      }
      break;
    default:
      printf("Invalid option. Please try again.\n");
      break;
    }
  }
}

void run_menu_system(void) {
  load_database(products, &product_count, bundles, &bundle_count, orders,
                &order_count, &admin);

  if (product_count == 0) {
    Product p1 = {101, "iPhone 15 Pro", "Phone", "Apple", 999.99F, 10};
    Product p2 = {102, "ThinkPad X1 Carbon", "Laptop", "Lenovo", 1499.99F, 5};
    Product p3 = {103, "iPad Air", "Tablet", "Apple", 599.99F, 8};
    add_product(products, &product_count, &p1);
    add_product(products, &product_count, &p2);
    add_product(products, &product_count, &p3);
  }

  if (is_admin_setup_complete() == 0) {
    printf("==========================================\n");
    printf("        ADMIN CREDENTIALS SETUP           \n");
    printf("==========================================\n");
    char username[MAX_USERNAME_LEN] = {0};
    char password[MAX_PASSWORD_LEN] = {0};
    int setup_done = 0;
    while (setup_done == 0) {
      get_safe_string("Create Admin Username (0 to cancel): ", username,
                      MAX_USERNAME_LEN);
      if (strcmp(username, "0") == 0) {
        printf("Setup cannot be cancelled. Admin setup is mandatory.\n");
        continue;
      }
      get_safe_string("Create Admin Password (0 to cancel): ", password,
                      MAX_PASSWORD_LEN);
      if (strcmp(password, "0") == 0) {
        printf("Setup cannot be cancelled. Admin setup is mandatory.\n");
        continue;
      }

      if (strlen(username) < 4 || strchr(username, ' ') != NULL) {
        printf("Invalid username. Must be at least 4 characters and contain no "
               "spaces.\n");
        continue;
      }
      if (strlen(password) < 4 || strchr(password, ' ') != NULL) {
        printf("Invalid password. Must be at least 4 characters and contain no "
               "spaces.\n");
        continue;
      }

      if (setup_admin_credentials(username, password) != 0) {
        printf("Admin credentials saved successfully!\n");
        setup_done = 1;
      } else {
        printf("Error: Failed to write credentials file. Try again.\n");
      }
    }
  }

  display_customer_menu();
}

static int process_product_order(Product products[], int product_count,
                                 const Order *new_order, float *item_price);
static int process_bundle_order(Product products[], int product_count,
                                const Bundle bundles[], int bundle_count,
                                const Order *new_order, float *item_price);
static void finalize_order_metadata(Order *ord, const Order *orders, int count,
                                    const Order *new_order, float item_price);

static int process_product_order(Product products[], int product_count,
                                 const Order *new_order, float *item_price) {
  int prod_idx =
      find_product_by_id(products, product_count, new_order->item_id);
  if (prod_idx == -1) {
    printf("Error: Product ID %d does not exist.\n", new_order->item_id);
    return 0;
  }
  Product *prod = &products[prod_idx];
  if (prod->stock_quantity < new_order->quantity) {
    printf("Error: Insufficient stock available.\n");
    return 0;
  }
  prod->stock_quantity -= new_order->quantity;
  *item_price = prod->price;
  return 1;
}

static int process_bundle_order(Product products[], int product_count,
                                const Bundle bundles[], int bundle_count,
                                const Order *new_order, float *item_price) {
  int bundle_idx = -1;
  for (int i = 0; i < bundle_count; i++) {
    if (bundles[i].bundle_id == new_order->item_id) {
      bundle_idx = i;
      break;
    }
  }
  if (bundle_idx == -1) {
    printf("Error: Bundle ID %d does not exist.\n", new_order->item_id);
    return 0;
  }
  const Bundle *bundle = &bundles[bundle_idx];
  int virtual_stock = get_virtual_bundle_stock(bundle, products, product_count);
  if (virtual_stock < new_order->quantity) {
    printf("Error: Insufficient stock available.\n");
    for (int i = 0; i < bundle->product_count; i++) {
      int prod_id = bundle->product_ids[i];
      int prod_idx = find_product_by_id(products, product_count, prod_id);
      if (prod_idx != -1) {
        int stock = products[prod_idx].stock_quantity;
        if (stock < new_order->quantity) {
          printf("  - Product '%s' (ID %d) has insufficient stock. Requested: "
                 "%d, Available: %d.\n",
                 products[prod_idx].product_name, prod_id, new_order->quantity,
                 stock);
        }
      } else {
        printf("  - Product ID %d in bundle does not exist.\n", prod_id);
      }
    }
    return 0;
  }
  for (int i = 0; i < bundle->product_count; i++) {
    int prod_id = bundle->product_ids[i];
    int prod_idx = find_product_by_id(products, product_count, prod_id);
    if (prod_idx != -1) {
      products[prod_idx].stock_quantity -= new_order->quantity;
    }
  }
  *item_price = calculate_bundle_price(bundle, products, product_count);
  return 1;
}

static void finalize_order_metadata(Order *ord, const Order *orders, int count,
                                    const Order *new_order, float item_price) {
  ord->order_id = new_order->order_id;
  if (ord->order_id <= 0) {
    int max_id = 0;
    for (int i = 0; i < count; i++) {
      if (orders[i].order_id > max_id) {
        max_id = orders[i].order_id;
      }
    }
    ord->order_id = max_id + 1;
  }
  strncpy(ord->customer_name, new_order->customer_name,
          sizeof(ord->customer_name) - 1);
  ord->customer_name[sizeof(ord->customer_name) - 1] = '\0';
  ord->item_id = new_order->item_id;
  ord->is_bundle = new_order->is_bundle;
  ord->quantity = new_order->quantity;
  ord->total_price = item_price * (float)new_order->quantity;

  if (strlen(new_order->order_date) > 0) {
    strncpy(ord->order_date, new_order->order_date,
            sizeof(ord->order_date) - 1);
    ord->order_date[sizeof(ord->order_date) - 1] = '\0';
  } else {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if (tm_info != NULL) {
      strftime(ord->order_date, sizeof(ord->order_date), "%Y-%m-%d", tm_info);
    } else {
      strncpy(ord->order_date, "2026-06-19", sizeof(ord->order_date) - 1);
      ord->order_date[sizeof(ord->order_date) - 1] = '\0';
    }
  }
}

int create_order(Order orders[], int *count, Product products[],
                 int product_count, const Bundle bundles[], int bundle_count,
                 const Order *new_order) {
  if (orders == NULL || count == NULL || products == NULL || bundles == NULL ||
      new_order == NULL) {
    return 0;
  }
  if (*count < 0 || *count >= MAX_ORDERS) {
    printf("Error: Order limit reached or invalid count.\n");
    return 0;
  }

  if (new_order->quantity <= 0) {
    printf("Error: Order quantity must be greater than 0.\n");
    return 0;
  }

  if (strlen(new_order->customer_name) == 0) {
    printf("Error: Customer name cannot be empty.\n");
    return 0;
  }

  float item_price = 0.0F;

  if (new_order->is_bundle == 0) {
    if (!process_product_order(products, product_count, new_order,
                               &item_price)) {
      return 0;
    }
  } else if (new_order->is_bundle == 1) {
    if (!process_bundle_order(products, product_count, bundles, bundle_count,
                              new_order, &item_price)) {
      return 0;
    }
  } else {
    printf("Error: Invalid item type flag (is_bundle must be 0 or 1).\n");
    return 0;
  }

  finalize_order_metadata(&orders[*count], orders, *count, new_order,
                          item_price);
  (*count)++;
  return 1;
}

void display_order_history(const Order orders[], int count) {
  if (orders == NULL || count <= 0) {
    printf("\nNo orders found in history.\n");
    return;
  }
  printf("\n=================================== ORDER HISTORY "
         "===================================\n");
  printf("%-8s | %-20s | %-8s | %-10s | %-8s | %-12s | %-10s\n", "Order ID",
         "Customer Name", "Item ID", "Item Type", "Quantity", "Total Price",
         "Date");
  printf("---------------------------------------------------------------------"
         "----------------\n");
  for (int i = 0; i < count; i++) {
    printf("%-8d | %-20s | %-8d | %-10s | %-8d | $%-11.2f | %-10s\n",
           orders[i].order_id, orders[i].customer_name, orders[i].item_id,
           orders[i].is_bundle ? "Bundle" : "Product", orders[i].quantity,
           (double)orders[i].total_price, orders[i].order_date);
  }
  printf("====================================================================="
         "===========-----\n");
}

void print_revenue_report(const Order orders[], int order_count,
                          const Product products[], int product_count,
                          const Bundle bundles[], int bundle_count) {
  int product_sold = 0;
  int bundle_sold = 0;
  float total_revenue = 0.0F;

  (void)products;
  (void)product_count;
  (void)bundles;
  (void)bundle_count;

  if (orders == NULL || order_count <= 0) {
    printf("\n--- REVENUE REPORT ---\n");
    printf("Total Orders: 0\n");
    printf("Total Revenue: $0.00\n");
    return;
  }

  for (int i = 0; i < order_count; i++) {
    if (orders[i].is_bundle == 0) {
      product_sold += orders[i].quantity;
    } else {
      bundle_sold += orders[i].quantity;
    }
    total_revenue += orders[i].total_price;
  }

  printf("\n--- REVENUE REPORT ---\n");
  printf("Total Orders: %d\n", order_count);
  printf("Total Revenue: $%.2f\n", (double)total_revenue);
  printf("Total count of products sold: %d\n", product_sold);
  printf("Total count of bundles sold: %d\n", bundle_sold);
}

void print_best_seller_products(const Order orders[], int order_count,
                                const Product products[], int product_count,
                                const Bundle bundles[], int bundle_count) {
  int product_sales[MAX_PRODUCTS] = {0};
  int best_seller = 0;
  int stt = 1;

  if (orders == NULL || order_count <= 0 || products == NULL ||
      product_count <= 0) {
    printf("No products or bundles have been sold yet.\n");
    cont();
    return;
  }

  for (int i = 0; i < product_count; i++) {
    int prod_id = products[i].product_id;
    int total = 0;
    for (int j = 0; j < order_count; j++) {
      if (orders[j].is_bundle == 0) {
        if (orders[j].item_id == prod_id) {
          total += orders[j].quantity;
        }
      } else {
        int b_idx = find_bundle_by_id(bundles, bundle_count, orders[j].item_id);
        if (b_idx != -1) {
          for (int k = 0; k < bundles[b_idx].product_count; k++) {
            if (bundles[b_idx].product_ids[k] == prod_id) {
              total += orders[j].quantity;
              break;
            }
          }
        }
      }
    }
    product_sales[i] = total;
    if (total > best_seller) {
      best_seller = total;
    }
  }

  if (best_seller == 0) {
    printf("No products have been sold yet.\n");
    cont();
    return;
  }

  printf("\n+-----+-----------------------------------+------------+\n");
  printf("| %-3s | %-33s | %-10s |\n", "No", "Product Name", "Total Sold");
  printf("+-----+-----------------------------------+------------+\n");

  for (int i = 0; i < product_count; i++) {
    if (product_sales[i] == best_seller) {
      printf("| %-3d | %-33s | %-10d |\n", stt, products[i].product_name,
             best_seller);
      stt++;
    }
  }
  printf("+-----+-----------------------------------+------------+\n");

  cont();
}

void print_best_seller_bundles(const Order orders[], int order_count,
                               const Bundle bundles[], int bundle_count) {
  int bundle_sales[MAX_BUNDLES] = {0};
  int best_seller = 0;
  int stt = 1;

  if (orders == NULL || order_count <= 0 || bundles == NULL ||
      bundle_count <= 0) {
    printf("No bundles have been sold yet.\n");
    cont();
    return;
  }

  for (int i = 0; i < bundle_count; i++) {
    int b_id = bundles[i].bundle_id;
    int total = 0;
    for (int j = 0; j < order_count; j++) {
      if (orders[j].is_bundle == 1 && orders[j].item_id == b_id) {
        total += orders[j].quantity;
      }
    }
    bundle_sales[i] = total;
    if (total > best_seller) {
      best_seller = total;
    }
  }

  if (best_seller == 0) {
    printf("No bundles have been sold yet.\n");
    cont();
    return;
  }

  printf("\n+-----+-----------------------------------+------------+\n");
  printf("| %-3s | %-33s | %-10s |\n", "No", "Bundle Name", "Total Sold");
  printf("+-----+-----------------------------------+------------+\n");
  for (int i = 0; i < bundle_count; i++) {
    if (bundle_sales[i] == best_seller) {
      printf("| %-3d | %-33s | %-10d |\n", stt, bundles[i].bundle_name,
             best_seller);
      stt++;
    }
  }
  printf("+-----+-----------------------------------+------------+\n");

  cont();
}

void alert_low_stock(const Product products[], int product_count) {
  int check = 1;
  int stt = 1;
  const char *border = "+------+--------+--------------------------------------"
                       "--------------+--------------+";

  printf("%s\n", border);
  printf("| %-4s | %-6s | %-50s | %-12s |\n", "NO", "ID", "NAME",
         "CURRENT STOCK");
  printf("%s\n", border);

  for (int i = 0; i < product_count; i++) {
    if (products[i].stock_quantity < 5) {
      check = 0;

      printf("| %-4d | %-6d | %-50.50s | %-12d |\n", stt,
             products[i].product_id, products[i].product_name,
             products[i].stock_quantity);
      stt++;
    }
  }

  if (!check) {
    printf("%s\n", border);
  } else {
    printf("\nAll products have sufficient stock levels.\n");
  }

  cont();
}

static int is_valid_category(const char *category);

static int is_valid_category(const char *category) {
  if (category == NULL) {
    return 0;
  }
  return (strcmp(category, "Phone") == 0 || strcmp(category, "Laptop") == 0 ||
          strcmp(category, "Tablet") == 0 ||
          strcmp(category, "Accessory") == 0);
}

int add_product(Product products[], int *count, const Product *new_prod) {
  if (products == NULL || count == NULL || new_prod == NULL) {
    return 0;
  }
  if (*count >= MAX_PRODUCTS) {
    printf("Error: Maximum product limit reached (%d).\n", MAX_PRODUCTS);
    return 0;
  }
  if (new_prod->product_id <= 0) {
    printf("Error: Product ID must be a positive integer.\n");
    return 0;
  }
  for (int i = 0; i < *count; i++) {
    if (products[i].product_id == new_prod->product_id) {
      printf("Error: Product ID %d already exists.\n", new_prod->product_id);
      return 0;
    }
  }
  if (!is_valid_category(new_prod->category)) {
    printf("Error: Invalid category '%s'. Category must strictly be one of: "
           "Phone, Laptop, Tablet, Accessory.\n",
           new_prod->category);
    return 0;
  }
  if (new_prod->price <= 0.0F) {
    printf("Error: Price must be a positive float value (> 0.0).\n");
    return 0;
  }
  if (new_prod->stock_quantity < 0) {
    printf("Error: Stock quantity must be non-negative (>= 0).\n");
    return 0;
  }

  products[*count].product_id = new_prod->product_id;

  strncpy(products[*count].product_name, new_prod->product_name,
          MAX_NAME_LEN - 1);
  products[*count].product_name[MAX_NAME_LEN - 1] = '\0';

  strncpy(products[*count].category, new_prod->category, MAX_CAT_LEN - 1);
  products[*count].category[MAX_CAT_LEN - 1] = '\0';

  strncpy(products[*count].brand, new_prod->brand, MAX_BRAND_LEN - 1);
  products[*count].brand[MAX_BRAND_LEN - 1] = '\0';

  products[*count].price = new_prod->price;
  products[*count].stock_quantity = new_prod->stock_quantity;
  (*count)++;
  return 1;
}

int update_product(Product products[], int count, int id,
                   const Product *updated) {
  if (products == NULL || updated == NULL || count < 0) {
    return 0;
  }
  int index = find_product_by_id(products, count, id);
  if (index == -1) {
    printf("Error: Product ID %d not found.\n", id);
    return 0;
  }
  if (updated->product_id != id) {
    if (updated->product_id <= 0) {
      printf("Error: Product ID must be a positive integer.\n");
      return 0;
    }
    for (int i = 0; i < count; i++) {
      if (i != index && products[i].product_id == updated->product_id) {
        printf("Error: Product ID %d already exists.\n", updated->product_id);
        return 0;
      }
    }
  }
  if (!is_valid_category(updated->category)) {
    printf("Error: Invalid category '%s'. Category must strictly be one of: "
           "Phone, Laptop, Tablet, Accessory.\n",
           updated->category);
    return 0;
  }
  if (updated->price <= 0.0F) {
    printf("Error: Price must be a positive float value (> 0.0).\n");
    return 0;
  }
  if (updated->stock_quantity < 0) {
    printf("Error: Stock quantity must be non-negative (>= 0).\n");
    return 0;
  }

  products[index].product_id = updated->product_id;

  strncpy(products[index].product_name, updated->product_name,
          MAX_NAME_LEN - 1);
  products[index].product_name[MAX_NAME_LEN - 1] = '\0';

  strncpy(products[index].category, updated->category, MAX_CAT_LEN - 1);
  products[index].category[MAX_CAT_LEN - 1] = '\0';

  strncpy(products[index].brand, updated->brand, MAX_BRAND_LEN - 1);
  products[index].brand[MAX_BRAND_LEN - 1] = '\0';

  products[index].price = updated->price;
  products[index].stock_quantity = updated->stock_quantity;

  return 1;
}

int delete_product(Product products[], int *count, int id,
                   const Bundle bundles[], int bundle_count) {
  if (products == NULL || count == NULL || bundles == NULL || *count < 0 ||
      bundle_count < 0) {
    return 0;
  }
  int index = find_product_by_id(products, *count, id);
  if (index == -1) {
    printf("Error: Product ID %d not found.\n", id);
    return 0;
  }

  int is_blocked = 0;
  for (int i = 0; i < bundle_count; i++) {
    for (int j = 0; j < bundles[i].product_count; j++) {
      if (bundles[i].product_ids[j] == id) {
        if (!is_blocked) {
          printf("Cannot delete product. It is part of active bundles:\n");
          is_blocked = 1;
        }
        printf("- Bundle ID: %d, Name: %s\n", bundles[i].bundle_id,
               bundles[i].bundle_name);
        break;
      }
    }
  }

  if (is_blocked) {
    return 0;
  }

  for (int i = index; i < *count - 1; i++) {
    products[i] = products[i + 1];
  }
  (*count)--;
  return 1;
}

int find_product_by_id(const Product products[], int count, int id) {
  if (products == NULL || count < 0 || id <= 0) {
    return -1;
  }
  for (int i = 0; i < count; i++) {
    if (products[i].product_id == id) {
      return i;
    }
  }
  return -1;
}

void display_all_products(const Product products[], int count) {
  if (products == NULL || count < 0) {
    return;
  }
  if (count == 0) {
    printf("No products currently in inventory.\n");
    return;
  }
  printf("%-6s | %-30s | %-12s | %-20s | %-10s | %-8s\n", "ID", "Name",
         "Category", "Brand", "Price", "Stock");
  printf("---------------------------------------------------------------------"
         "--------------------------------\n");
  for (int i = 0; i < count; i++) {
    printf("%-6d | %-30s | %-12s | %-20s | $%-9.2f | %-8d\n",
           products[i].product_id, products[i].product_name,
           products[i].category, products[i].brand, (double)products[i].price,
           products[i].stock_quantity);
  }
}

void search_product_by_name(const Product products[], int count,
                            const char *query) {
  if (products == NULL || count < 0 || query == NULL) {
    return;
  }
  char query_lower[MAX_NAME_LEN];
  strncpy(query_lower, query, sizeof(query_lower) - 1);
  query_lower[sizeof(query_lower) - 1] = '\0';
  to_lowercase(query_lower);

  int found = 0;
  for (int i = 0; i < count; i++) {
    char name_lower[MAX_NAME_LEN];
    strncpy(name_lower, products[i].product_name, sizeof(name_lower) - 1);
    name_lower[sizeof(name_lower) - 1] = '\0';
    to_lowercase(name_lower);

    if (strstr(name_lower, query_lower) != NULL) {
      if (!found) {
        printf("%-6s | %-30s | %-12s | %-20s | %-10s | %-8s\n", "ID", "Name",
               "Category", "Brand", "Price", "Stock");
        printf("---------------------------------------------------------------"
               "------"
               "--------------------------------\n");
        found = 1;
      }
      printf("%-6d | %-30s | %-12s | %-20s | $%-9.2f | %-8d\n",
             products[i].product_id, products[i].product_name,
             products[i].category, products[i].brand, (double)products[i].price,
             products[i].stock_quantity);
    }
  }
  if (!found) {
    printf("No products found.\n");
  }
}

void filter_product_by_category(const Product products[], int count,
                                const char *category) {
  if (products == NULL || count < 0 || category == NULL) {
    return;
  }
  if (strcmp(category, "Phone") != 0 && strcmp(category, "Laptop") != 0 &&
      strcmp(category, "Tablet") != 0 && strcmp(category, "Accessory") != 0) {
    printf("No products found.\n");
    return;
  }

  int found = 0;
  for (int i = 0; i < count; i++) {
    if (strcmp(products[i].category, category) == 0) {
      if (!found) {
        printf("%-6s | %-30s | %-12s | %-20s | %-10s | %-8s\n", "ID", "Name",
               "Category", "Brand", "Price", "Stock");
        printf("---------------------------------------------------------------"
               "------"
               "--------------------------------\n");
        found = 1;
      }
      printf("%-6d | %-30s | %-12s | %-20s | $%-9.2f | %-8d\n",
             products[i].product_id, products[i].product_name,
             products[i].category, products[i].brand, (double)products[i].price,
             products[i].stock_quantity);
    }
  }
  if (!found) {
    printf("No products found.\n");
  }
}

void filter_product_by_price(const Product products[], int count,
                             float min_price, float max_price) {
  if (products == NULL || count < 0) {
    return;
  }
  if (min_price < 0.0F || max_price < 0.0F) {
    printf("Error: Price boundaries must be non-negative.\n");
    return;
  }
  if (min_price > max_price) {
    printf("Error: Minimum price cannot be greater than maximum price.\n");
    return;
  }
  int found = 0;
  for (int i = 0; i < count; i++) {
    if (products[i].price >= min_price && products[i].price <= max_price) {
      if (!found) {
        printf("%-6s | %-30s | %-12s | %-20s | %-10s | %-8s\n", "ID", "Name",
               "Category", "Brand", "Price", "Stock");
        printf("---------------------------------------------------------------"
               "------"
               "--------------------------------\n");
        found = 1;
      }
      printf("%-6d | %-30s | %-12s | %-20s | $%-9.2f | %-8d\n",
             products[i].product_id, products[i].product_name,
             products[i].category, products[i].brand, (double)products[i].price,
             products[i].stock_quantity);
    }
  }
  if (!found) {
    printf("No products found.\n");
  }
}

void bubble_sort_by_price(Product products[], int count, int ascending) {
  if (products == NULL || count < 0) {
    return;
  }
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      int condition = 0;
      if (ascending != 0) {
        condition = (products[j].price > products[j + 1].price);
      } else {
        condition = (products[j].price < products[j + 1].price);
      }
      if (condition != 0) {
        Product temp = products[j];
        products[j] = products[j + 1];
        products[j + 1] = temp;
      }
    }
  }
  display_all_products(products, count);
}

void bubble_sort_by_name(Product products[], int count) {
  if (products == NULL || count < 0) {
    return;
  }
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (strcmp(products[j].product_name, products[j + 1].product_name) > 0) {
        Product temp = products[j];
        products[j] = products[j + 1];
        products[j + 1] = temp;
      }
    }
  }
  display_all_products(products, count);
}

void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

void to_lowercase(char *str) {
  if (str == NULL) {
    return;
  }
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = (char)tolower((unsigned char)str[i]);
  }
}

void get_safe_string(const char *prompt, char *out_str, int max_len) {
  if (out_str == NULL || max_len <= 0) {
    return;
  }
  if (prompt != NULL) {
    printf("%s", prompt);
    fflush(stdout);
  }
  if (fgets(out_str, max_len, stdin) == NULL) {
    out_str[0] = '\0';
    return;
  }
  int len = (int)strlen(out_str);
  if (len > 0 && out_str[len - 1] == '\n') {
    out_str[len - 1] = '\0';
  } else {
    clear_input_buffer();
  }
}

int get_safe_int(const char *prompt, int *out_value) {
  char buf[128];
  while (1) {
    get_safe_string(prompt, buf, sizeof(buf));

    if (strcmp(buf, "0") == 0) {
      if (out_value != NULL) {
        *out_value = 0;
      }
      return 0;
    }

    int i = 0;
    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] == '\0') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    int sign = 1;
    if (buf[i] == '-') {
      sign = -1;
      i++;
    } else if (buf[i] == '+') {
      i++;
    }

    if (buf[i] < '0' || buf[i] > '9') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    long long val = 0;
    int overflow = 0;
    while (buf[i] >= '0' && buf[i] <= '9') {
      val = (val * 10) + (buf[i] - '0');
      if (sign == 1 && val > 2147483647LL) {
        val = 2147483647LL;
        overflow = 1;
      } else if (sign == -1 && val > 2147483648LL) {
        val = 2147483648LL;
        overflow = 1;
      }
      i++;
    }

    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] != '\0' || overflow) {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    int final_val;
    if (sign == -1) {
      final_val = (int)(-val);
    } else {
      final_val = (int)val;
    }

    if (out_value != NULL) {
      *out_value = final_val;
    }
    return 1;
  }
}

int get_safe_float(const char *prompt, float *out_value) {
  char buf[128];
  while (1) {
    get_safe_string(prompt, buf, sizeof(buf));

    if (strcmp(buf, "0") == 0) {
      if (out_value != NULL) {
        *out_value = 0.0F;
      }
      return 0;
    }

    int i = 0;
    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] == '\0') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    int sign = 1;
    if (buf[i] == '-') {
      sign = -1;
      i++;
    } else if (buf[i] == '+') {
      i++;
    }

    if ((buf[i] < '0' || buf[i] > '9') && buf[i] != '.') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    double integer_part = 0.0;
    int has_digits = 0;
    while (buf[i] >= '0' && buf[i] <= '9') {
      integer_part = (integer_part * 10.0) + (buf[i] - '0');
      has_digits = 1;
      i++;
    }

    double fractional_part = 0.0;
    double divisor = 1.0;
    if (buf[i] == '.') {
      i++;
      if ((buf[i] < '0' || buf[i] > '9') && !has_digits) {
        printf("Invalid entry, please try again.\n");
        continue;
      }
      while (buf[i] >= '0' && buf[i] <= '9') {
        fractional_part = (fractional_part * 10.0) + (buf[i] - '0');
        divisor *= 10.0;
        has_digits = 1;
        i++;
      }
    }

    if (!has_digits) {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '\t' ||
                              buf[i] == '\n' || buf[i] == '\r')) {
      i++;
    }

    if (buf[i] != '\0') {
      printf("Invalid entry, please try again.\n");
      continue;
    }

    double val = sign * (integer_part + (fractional_part / divisor));

    if (out_value != NULL) {
      *out_value = (float)val;
    }
    return 1;
  }
}
void input_string(char *str, int size) {
  if (fgets(str, size, stdin) == NULL) {
    str[0] = '\0';
    return;
  }
  int len = (int)strlen(str);
  for (int i = 0; i < len; i++) {
    if (str[i] == '\n' || str[i] == '\r') {
      str[i] = '\0';
      break;
    }
  }
}

void cont(void) {
  printf("Press enter to continue!");
  (void)getchar();
}

int confirm_action(const char *message) {
  char choice[5];
  printf("%s (Y/N): ", message);
  input_string(choice, sizeof(choice));
  return (choice[0] == 'Y' || choice[0] == 'y');
}

int main(void) {
  printf("E-Commerce Electronics & Bundle Management System\n");
  run_menu_system();
  return 0;
}
