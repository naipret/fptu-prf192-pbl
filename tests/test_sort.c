#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "product.h"

static void test_sort_by_price_ascending(void) {
  printf("Running test_sort_by_price_ascending...\n");

  Product products[3] = {
      {1, "Laptop", "Laptop", "BrandA", 1200.0F, 10},
      {2, "Phone", "Phone", "BrandB", 800.0F, 20},
      {3, "Tablet", "Tablet", "BrandC", 500.0F, 15}
  };

  bubble_sort_by_price(products, 3, 1);

  // Expected order: Tablet (500), Phone (800), Laptop (1200)
  assert(products[0].product_id == 3);
  assert(products[1].product_id == 2);
  assert(products[2].product_id == 1);

  printf("test_sort_by_price_ascending passed!\n");
}

static void test_sort_by_price_descending(void) {
  printf("Running test_sort_by_price_descending...\n");

  Product products[3] = {
      {1, "Laptop", "Laptop", "BrandA", 1200.0F, 10},
      {2, "Phone", "Phone", "BrandB", 800.0F, 20},
      {3, "Tablet", "Tablet", "BrandC", 500.0F, 15}
  };

  bubble_sort_by_price(products, 3, 0);

  // Expected order: Laptop (1200), Phone (800), Tablet (500)
  assert(products[0].product_id == 1);
  assert(products[1].product_id == 2);
  assert(products[2].product_id == 3);

  printf("test_sort_by_price_descending passed!\n");
}

static void test_sort_by_name(void) {
  printf("Running test_sort_by_name...\n");

  Product products[3] = {
      {1, "Tablet", "Tablet", "BrandC", 500.0F, 15},
      {2, "Phone", "Phone", "BrandB", 800.0F, 20},
      {3, "Laptop", "Laptop", "BrandA", 1200.0F, 10}
  };

  bubble_sort_by_name(products, 3);

  // Expected alphabetical: Laptop, Phone, Tablet
  assert(strcmp(products[0].product_name, "Laptop") == 0);
  assert(strcmp(products[1].product_name, "Phone") == 0);
  assert(strcmp(products[2].product_name, "Tablet") == 0);

  printf("test_sort_by_name passed!\n");
}

int main(void) {
  test_sort_by_price_ascending();
  test_sort_by_price_descending();
  test_sort_by_name();
  printf("All sort tests passed successfully!\n");
  return 0;
}
