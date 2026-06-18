#include <stdio.h>
#include <string.h>
#include "bundle.h"

int create_bundle(Bundle bundles[], int *count,const Bundle *new_bundle) {
  if(new_bundle->discount_rate < 0 || new_bundle->discount_rate > 1){
      printf("Discount rate must be between 0 and 1.");
      cont();
      return 0;
  }
  bundles[*count] = *new_bundle;
  (*count)++;
return 0;
}

int add_product_to_bundle(Bundle *bundle, int product_id) {
  (void)bundle;
  (void)product_id;
  return 0;
}

int remove_product_from_bundle(Bundle *bundle, int product_id) {
  (void)bundle;
  (void)product_id;
  return 0;
}

float calculate_bundle_price(const Bundle *bundle, const Product products[],
                             int product_count) {
  (void)bundle;
  (void)products;
  (void)product_count;
  return 0.0F;
}

int get_virtual_bundle_stock(const Bundle *bundle, const Product products[],
                             int product_count) {
  (void)bundle;
  (void)products;
  (void)product_count;
  return 0;
}

void display_all_bundles(const Bundle bundles[], int count,
                         const Product products[], int product_count) {
  (void)bundles;
  (void)count;
  (void)products;
  (void)product_count;
}
