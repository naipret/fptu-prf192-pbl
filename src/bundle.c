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
    if(bundle->product_count == MAX_BUNDLE_ITEMS){
        printf("Maximum limit reached: You can only add up to 10 products to a bundle.\n");
        cont();
        return 0;
    }
    for(int i = 0;i < bundle->product_count;i++){
        if(bundle->product_ids[i] == product_id){
            printf("This product is already in the bundle.\n");
            cont();
            return 0;
        }
    }
    bundle->product_ids[bundle->product_count++] = product_id;
    return 0;
}

int remove_product_from_bundle(Bundle bundles[],int *bundle_count, Bundle *bundle, int product_id) {
    int cnt = bundle->product_count;
    int index = -1;
    for(int i = 0;i < cnt;i++){
        if(bundle->product_ids[i] == product_id){
            index = i;
            break;
        }
    }
    if(cnt == 1){
        if(confirmAction("This action will also delete the bundle. Do you want to continue?")){
            int index1 = -1;
            for(int i = 0;i < *bundle_count;i++){
                if(bundles[i].bundle_id == bundle->bundle_id){
                    index1 = i;
                    break;
                }
            }
            for(int i = index1;i < *bundle_count - 1;i++){
                bundles[i] = bundles[i + 1];
            }
            (*bundle_count)--;
            return 0;
        }
        return 0;
        
    }
    if(index == -1){
        printf("The bundle does not contain this product.\n");
        cont();
        return 0;
    }
    for(int i = index;i < cnt - 1;i++){
        bundle->product_ids[i] = bundle->product_ids[i + 1];
    }
    bundle->product_count--;
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
