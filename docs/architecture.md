# Technical Architecture Design

---

## 1. Directory Structure

The project is structured to enforce modularity and code separation:

```bash
prf192-pbl-e-commerce-electronics-bundle-and-management-system/
├── Makefile
├── Dockerfile
├── .clang-format
├── .clang-tidy
├── CONTRIBUTING.md
├── include/
│   ├── admin.h
│   ├── product.h
│   ├── bundle.h
│   ├── order.h
│   ├── file_io.h
│   ├── utils.h
│   └── menu.h
├── src/
│   ├── admin.c
│   ├── product.c
│   ├── bundle.c
│   ├── order.c
│   ├── file_io.c
│   ├── utils.c
│   ├── menu.c
│   └── program.c
└── docs/
```

---

## 2. Structure Signatures & Global Types

### 2.1. Admin Structures (`include/admin.h`)

```c
#ifndef ADMIN_H
#define ADMIN_H

#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int is_setup;
} AdminCredentials;

// Functions
int is_admin_setup_complete(void);
int setup_admin_credentials(const char *username, const char *password);
int verify_admin_login(const char *username, const char *password);

#endif /* ADMIN_H */
```

### 2.2. Product Structures (`include/product.h`)

```c
#ifndef PRODUCT_H
#define PRODUCT_H

#define MAX_PRODUCTS 100
#define MAX_NAME_LEN 100
#define MAX_CAT_LEN 50
#define MAX_BRAND_LEN 50

typedef struct {
    int product_id;
    char product_name[MAX_NAME_LEN];
    char category[MAX_CAT_LEN];
    char brand[MAX_BRAND_LEN];
    float price;
    int stock_quantity;
} Product;

// Functions
int add_product(Product products[], int *count, const Product *new_prod);
int update_product(Product products[], int count, int id, const Product *updated);
int delete_product(Product products[], int *count, int id, const int active_bundle_prod_ids[], int active_bundle_count);
int find_product_by_id(const Product products[], int count, int id);
void display_all_products(const Product products[], int count);

#endif /* PRODUCT_H */
```

### 2.3. Bundle Structures (`include/bundle.h`)

```c
#ifndef BUNDLE_H
#define BUNDLE_H

#include "product.h"

#define MAX_BUNDLES 50
#define MAX_BUNDLE_ITEMS 10

typedef struct {
    int bundle_id;
    char bundle_name[MAX_NAME_LEN];
    int product_ids[MAX_BUNDLE_ITEMS];
    int product_count;
    float discount_rate;
    float bundle_price;
} Bundle;

// Functions
int create_bundle(Bundle bundles[], int *count, const Bundle *new_bundle);
int add_product_to_bundle(Bundle *bundle, int product_id);
int remove_product_from_bundle(Bundle *bundle, int product_id);
float calculate_bundle_price(const Bundle *bundle, const Product products[], int product_count);
int get_virtual_bundle_stock(const Bundle *bundle, const Product products[], int product_count);
void display_all_bundles(const Bundle bundles[], int count, const Product products[], int product_count);

#endif /* BUNDLE_H */
```

### 2.4. Order Structures (`include/order.h`)

```c
#ifndef ORDER_H
#define ORDER_H

#include "product.h"
#include "bundle.h"

#define MAX_ORDERS 200
#define MAX_DATE_LEN 20

typedef struct {
    int order_id;
    char customer_name[MAX_NAME_LEN];
    int item_id;
    int is_bundle; // 0 = product, 1 = bundle
    int quantity;
    float total_price;
    char order_date[MAX_DATE_LEN];
} Order;

// Functions
int create_order(Order orders[], int *count, Product products[], int product_count, const Bundle bundles[], int bundle_count, const Order *new_order);
void display_order_history(const Order orders[], int count);
void print_revenue_report(const Order orders[], int order_count, const Product products[], int product_count, const Bundle bundles[], int bundle_count);

#endif /* ORDER_H */
```

### 2.5. File I/O (`include/file_io.h`)

```c
#ifndef FILE_IO_H
#define FILE_IO_H

#include "product.h"
#include "bundle.h"
#include "order.h"
#include "admin.h"

// Functions implementing transaction-like saving (.tmp -> .bak -> .txt)
int save_database(const Product products[], int product_count, const Bundle bundles[], int bundle_count, const Order orders[], int order_count, const AdminCredentials *admin);
int load_database(Product products[], int *product_count, Bundle bundles[], int *bundle_count, Order orders[], int *order_count, AdminCredentials *admin);

#endif /* FILE_IO_H */
```

### 2.6. Robust Input Utility (`include/utils.h`)

```c
#ifndef UTILS_H
#define UTILS_H

// Helper functions for reading secure input and clearing buffers
int get_safe_int(const char *prompt, int *out_value);
int get_safe_float(const char *prompt, float *out_value);
void get_safe_string(const char *prompt, char *out_str, int max_len);
void clear_input_buffer(void);
void to_lowercase(char *str);

#endif /* UTILS_H */
```

---

## 3. Dependency Graph

```text
           ┌───────────┐
           │ program.c │ (Entry point)
           └─────┬─────┘
                 │
           ┌─────▼─────┐
           │  menu.h   │ (Navigation & CLI layouts)
           └─────┬─────┘
  ┌──────────────┼──────────────┬──────────────┐
┌─▼─────┐     ┌──▼────┐     ┌──▼────┐     ┌────▼────┐
│admin.h│     │order.h│     │bundle.h     │file_io.h│
└───────┘     └──┬────┘     └──┬────┘     └────┬────┘
                 │             │               │
                 └─────────────┼───────────────┘
                               │
                            ┌──▼──────┐
                            │product.h│
                            └─────────┘
```

---

## 4. Safe Transactional File-Save Protocol

When saving, the program executes the following steps:

1. **Create Temporary Storage**: Writes all records to `database.tmp` (or `products.tmp`, `bundles.tmp`, `orders.tmp`, `admin.tmp`).
2. **Close and Validate**: Safely flushes buffers and closes the handle.
3. **Backup Active Database**: Renames the active databases (e.g., `products.txt`) to `products.bak`.
4. **Promote Temporary File**: Renames `products.tmp` to `products.txt`.
5. **Recovery Check**: If loading the `.txt` fails, the `load_database` module automatically checks for the existence of the `.bak` file, alerts the user, and loads from it.

---

## 5. Build Configuration (Makefile)

The following Makefile standardizes building under C17 using `clang`:

```makefile
CC = clang
CFLAGS = -std=c17 -m64 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 \
         -Wformat-security -Wfloat-equal -Wundef -Wsign-conversion -Wcast-align \
         -Wcast-qual -Wmissing-prototypes -Wmissing-declarations -Wunreachable-code \
         -Wnull-dereference -Wimplicit-fallthrough -Wswitch-enum -Wpointer-arith \
         -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
 $(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
 $(CC) $(CFLAGS) -c -o $@ $<

clean:
 rm -f src/*.o $(TARGET) *.tmp *.bak

format:
 clang-format -i include/*.h src/*.c

tidy:
 clang-tidy src/*.c -- $(CFLAGS)
```
