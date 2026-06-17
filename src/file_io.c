#include <stdio.h>

#include "file_io.h"

int save_database(const Product products[], int product_count,
                  const Bundle bundles[], int bundle_count,
                  const Order orders[], int order_count,
                  const AdminCredentials *admin) {
  (void)products;
  (void)product_count;
  (void)bundles;
  (void)bundle_count;
  (void)orders;
  (void)order_count;
  (void)admin;
  return 0;
}

int load_database(Product products[], const int *product_count,
                  Bundle bundles[], const int *bundle_count, Order orders[],
                  const int *order_count, AdminCredentials *admin) {
  (void)products;
  (void)product_count;
  (void)bundles;
  (void)bundle_count;
  (void)orders;
  (void)order_count;
  (void)admin;
  return 0;
}
