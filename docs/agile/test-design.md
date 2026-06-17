# System Test Plan & Scenarios

This test design outlines the manual validation cases required to guarantee system correctness. Every team member must run these verification scripts before submitting a Pull Request for merge.

---

## 1. Authentication & Security Tests

| Test ID | Test Scenario | Inputs / Steps | Expected Outcome |
|---|---|---|---|
| **TC-SEC-01** | First-Run Registration Setup | 1. Open app when `admin.txt` is deleted.<br>2. Prompt triggers automatically. | User must enter username & password. Short inputs (<4) or empty lines are rejected. Setup completes and launches Customer Menu. |
| **TC-SEC-02** | Cancel Registration | 1. Trigger setup.<br>2. Type `0` for username. | Setup terminates immediately. Program exits or fails safe. |
| **TC-SEC-03** | Admin Login Success | 1. Select Admin Mode.<br>2. Enter correct username & password. | Console displays "Login successful!" and switches to Admin Menu dashboard. |
| **TC-SEC-04** | Admin Login Cancel | 1. Select Admin Mode.<br>2. Enter `0` at username or password prompt. | Aborts login safely. Console switches back to Customer Menu. |
| **TC-SEC-05** | Admin Login Max Retries | 1. Select Admin Mode.<br>2. Enter invalid credentials 3 consecutive times. | System locks login for that session and returns to Customer Menu with alert. |

---

## 2. Product Inventory (CRUD) & Input Tests

| Test ID | Test Scenario | Inputs / Steps | Expected Outcome |
|---|---|---|---|
| **TC-PRD-01** | Product Creation ID Check | 1. Create product.<br>2. Try duplicate ID. | Rejected with message: "Product ID already exists." |
| **TC-PRD-02** | Product Creation Invalid Categories | 1. Enter category "Gadget" (not Phone/Laptop/Tablet/Accessory). | Rejected with message listing valid options. User prompted to re-enter. |
| **TC-PRD-03** | Product Creation Cancel | 1. Trigger Add Product.<br>2. Type `0` at ID prompt. | Aborts addition and returns to Admin menu. |
| **TC-PRD-04** | Product Deletion Active Bundle Gate | 1. Delete product ID 101.<br>2. Product ID 101 is currently in "Combo Starter Kit". | Deletion is blocked with warning: "Cannot delete product. It is part of active bundles." |
| **TC-PRD-05** | Input Buffer Handling | 1. At int prompt, type "abc".<br>2. At float prompt, type "12.34xyz". | System prints validation warning, clears buffer, and requests input again. No infinite loops. |

---

## 3. Dynamic Bundle Stock & Calculation Tests

| Test ID | Test Scenario | Inputs / Steps | Expected Outcome |
|---|---|---|---|
| **TC-BDL-01** | Dynamic Bundle Stock Calculation | 1. Bundle contains Prod A (stock=5) and Prod B (stock=2). | Querying bundle stock returns dynamically calculated stock of `2`. |
| **TC-BDL-02** | Dynamic Price Calculation | 1. Prod A price = $10.00, Prod B price = $20.00.<br>2. Bundle discount_rate = 0.15 (15%). | Bundle price calculated dynamically is $\text{Total} = (10 + 20) \times 0.85 = \$25.50$. |
| **TC-BDL-03** | Duplicate Item Prevention | 1. Create bundle.<br>2. Attempt to add Product ID 101 twice to the same bundle. | Duplicate addition is rejected with warning. |

---

## 4. Checkout & Inventory Deduction Tests

| Test ID | Test Scenario | Inputs / Steps | Expected Outcome |
|---|---|---|---|
| **TC-ORD-01** | Product Purchase Stock Deduction | 1. Order Product A (stock=10) with qty=3. | Order completes. Product A stock is reduced to 7. |
| **TC-ORD-02** | Bundle Purchase Stock Deduction | 1. Order Bundle X (Prod A stock=10, Prod B stock=5) with qty=2. | Order completes. Stock of Prod A drops to 8, Stock of Prod B drops to 3. |
| **TC-ORD-03** | Out of Stock Block | 1. Order Product A (stock=2) with qty=3. | Rejected with warning: "Insufficient stock available." |
| **TC-ORD-04** | Out of Stock Bundle Block | 1. Order Bundle X (Prod A stock=1, Prod B stock=10) with qty=2. | Rejected with warning listing which sub-items have insufficient stock. |

---

## 5. Safe Persistence & Transaction Recovery Tests

| Test ID | Test Scenario | Inputs / Steps | Expected Outcome |
|---|---|---|---|
| **TC-IO-01** | Transactional Save Lifecycle | 1. Trigger database save. | System generates `.tmp` files. Old active files are renamed to `.bak`. `.tmp` are promoted to active `.txt`. |
| **TC-IO-02** | Active File Failure Load Recovery | 1. Delete `products.txt` or corrupt it.<br>2. Launch program. | App detects missing/corrupt active file, finds `products.bak`, copies/renames it back to `products.txt`, and loads successfully. |
