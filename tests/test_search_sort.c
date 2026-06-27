#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "product.h"

static int saved_stdout_fd = -1;

static void set_stdout_redirect(const char *filename) {
  fflush(stdout);
  saved_stdout_fd = dup(1);
  if (saved_stdout_fd < 0) {
    perror("dup failed");
    return;
  }
  if (!freopen(filename, "w", stdout)) {
    perror("freopen failed");
  }
}

static void restore_stdout(void) {
  if (saved_stdout_fd >= 0) {
    fflush(stdout);
    if (dup2(saved_stdout_fd, 1) < 0) {
      perror("dup2 failed");
    }
    close(saved_stdout_fd);
    saved_stdout_fd = -1;
  }
}

static void clean_files(void) {
  unlink("test_output.txt");
}

static void test_search_by_name(void) {
  printf("Running test_search_by_name...\n");

  Product list[3] = {
      {101, "iPhone 15 Pro", "Phone", "Apple", 999.99F, 10},
      {102, "ThinkPad X1 Carbon", "Laptop", "Lenovo", 1499.99F, 5},
      {103, "iPad Air", "Tablet", "Apple", 599.99F, 8}
  };

  set_stdout_redirect("test_output.txt");
  search_product_by_name(list, 3, "air");
  restore_stdout();

  FILE *f = fopen("test_output.txt", "r");
  assert(f != NULL);
  char buf[512] = {0};
  int found_ipad = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "iPad Air") != NULL) {
      found_ipad = 1;
    }
  }
  fclose(f);
  assert(found_ipad == 1);

  // Case insensitivity test
  set_stdout_redirect("test_output.txt");
  search_product_by_name(list, 3, "IPHONE");
  restore_stdout();

  f = fopen("test_output.txt", "r");
  assert(f != NULL);
  int found_iphone = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "iPhone 15 Pro") != NULL) {
      found_iphone = 1;
    }
  }
  fclose(f);
  assert(found_iphone == 1);

  // No products found test
  set_stdout_redirect("test_output.txt");
  search_product_by_name(list, 3, "Samsung");
  restore_stdout();

  f = fopen("test_output.txt", "r");
  assert(f != NULL);
  int found_none = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "No products found.") != NULL) {
      found_none = 1;
    }
  }
  fclose(f);
  assert(found_none == 1);

  printf("test_search_by_name passed!\n");
}

static void test_filter_by_category(void) {
  printf("Running test_filter_by_category...\n");

  Product list[3] = {
      {101, "iPhone 15 Pro", "Phone", "Apple", 999.99F, 10},
      {102, "ThinkPad X1 Carbon", "Laptop", "Lenovo", 1499.99F, 5},
      {103, "iPad Air", "Tablet", "Apple", 599.99F, 8}
  };

  set_stdout_redirect("test_output.txt");
  filter_product_by_category(list, 3, "Phone");
  restore_stdout();

  FILE *f = fopen("test_output.txt", "r");
  assert(f != NULL);
  char buf[512] = {0};
  int found_phone = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "Phone") != NULL && strstr(buf, "iPhone 15 Pro") != NULL) {
      found_phone = 1;
    }
  }
  fclose(f);
  assert(found_phone == 1);

  // Invalid category check
  set_stdout_redirect("test_output.txt");
  filter_product_by_category(list, 3, "InvalidCategory");
  restore_stdout();

  f = fopen("test_output.txt", "r");
  assert(f != NULL);
  int found_none = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "No products found.") != NULL) {
      found_none = 1;
    }
  }
  fclose(f);
  assert(found_none == 1);

  printf("test_filter_by_category passed!\n");
}

static void test_filter_by_price(void) {
  printf("Running test_filter_by_price...\n");

  Product list[3] = {
      {101, "iPhone 15 Pro", "Phone", "Apple", 999.99F, 10},
      {102, "ThinkPad X1 Carbon", "Laptop", "Lenovo", 1499.99F, 5},
      {103, "iPad Air", "Tablet", "Apple", 599.99F, 8}
  };

  // Valid price range
  set_stdout_redirect("test_output.txt");
  filter_product_by_price(list, 3, 500.0F, 1000.0F);
  restore_stdout();

  FILE *f = fopen("test_output.txt", "r");
  assert(f != NULL);
  char buf[512] = {0};
  int count = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "iPhone 15 Pro") != NULL || strstr(buf, "iPad Air") != NULL) {
      count++;
    }
  }
  fclose(f);
  assert(count == 2);

  // Negative bounds validation check
  set_stdout_redirect("test_output.txt");
  filter_product_by_price(list, 3, -100.0F, 500.0F);
  restore_stdout();

  f = fopen("test_output.txt", "r");
  assert(f != NULL);
  int error_neg = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "Error: Price boundaries must be non-negative.") != NULL) {
      error_neg = 1;
    }
  }
  fclose(f);
  assert(error_neg == 1);

  // Min price > max price validation check
  set_stdout_redirect("test_output.txt");
  filter_product_by_price(list, 3, 500.0F, 200.0F);
  restore_stdout();

  f = fopen("test_output.txt", "r");
  assert(f != NULL);
  int error_range = 0;
  while (fgets(buf, sizeof(buf), f)) {
    if (strstr(buf, "Error: Minimum price cannot be greater than maximum price.") != NULL) {
      error_range = 1;
    }
  }
  fclose(f);
  assert(error_range == 1);

  printf("test_filter_by_price passed!\n");
}

static void test_bubble_sort_price(void) {
  printf("Running test_bubble_sort_price...\n");

  Product list[3] = {
      {101, "iPhone 15 Pro", "Phone", "Apple", 999.99F, 10},
      {102, "ThinkPad X1 Carbon", "Laptop", "Lenovo", 1499.99F, 5},
      {103, "iPad Air", "Tablet", "Apple", 599.99F, 8}
  };

  set_stdout_redirect("test_output.txt");
  // Sort ascending
  bubble_sort_by_price(list, 3, 1);
  restore_stdout();

  assert(list[0].product_id == 103); // iPad Air ($599.99)
  assert(list[1].product_id == 101); // iPhone 15 Pro ($999.99)
  assert(list[2].product_id == 102); // ThinkPad ($1499.99)

  set_stdout_redirect("test_output.txt");
  // Sort descending
  bubble_sort_by_price(list, 3, 0);
  restore_stdout();

  assert(list[0].product_id == 102); // ThinkPad ($1499.99)
  assert(list[1].product_id == 101); // iPhone 15 Pro ($999.99)
  assert(list[2].product_id == 103); // iPad Air ($599.99)

  printf("test_bubble_sort_price passed!\n");
}

static void test_bubble_sort_name(void) {
  printf("Running test_bubble_sort_name...\n");

  Product list[3] = {
      {101, "iPhone 15 Pro", "Phone", "Apple", 999.99F, 10},
      {102, "ThinkPad X1 Carbon", "Laptop", "Lenovo", 1499.99F, 5},
      {103, "iPad Air", "Tablet", "Apple", 599.99F, 8}
  };

  set_stdout_redirect("test_output.txt");
  bubble_sort_by_name(list, 3);
  restore_stdout();

  assert(strcmp(list[0].product_name, "ThinkPad X1 Carbon") == 0); // T
  assert(strcmp(list[1].product_name, "iPad Air") == 0);          // i
  assert(strcmp(list[2].product_name, "iPhone 15 Pro") == 0);     // iP

  printf("test_bubble_sort_name passed!\n");
}

int main(void) {
  test_search_by_name();
  test_filter_by_category();
  test_filter_by_price();
  test_bubble_sort_price();
  test_bubble_sort_name();
  clean_files();
  printf("All search/sort tests completed successfully!\n");
  return 0;
}
