# Product Requirements Document (PRD)

## E-Commerce Electronics & Bundle Management System

---

## 1. Introduction & Goals

This system is a terminal-based (CLI) **E-Commerce Electronics & Bundle Management System** designed in C17. It manages electronic products, groups them into dynamic discounted bundles (combos), tracks orders, handles automated stock adjustments, and outputs business intelligence reports.

### Key Success Objectives

* **Educational Rigor**: Strictly conforms to the standard C17 library on 64-bit architecture, compile-checked using Clang with strict diagnostic warnings.
* **UI/UX Clarity**: Standardizes consistent user validation prompts, loop-based retry configurations, and quick escape paths (pressing `0` to cancel).
* **Data Security & Safety**: Implements secure credential generation on first-run and a transactional file persistence system to prevent corruption.

---

## 2. User Roles & Access Control

The system supports two user categories: **Customers** (default interface) and **Administrators** (password-guarded backend).

```text
                      ┌──────────────────────┐
                      │    Program Startup   │
                      └──────────┬───────────┘
                                 │
                    ┌────────────▼────────────┐
                    │ First time opening app? │
                    └──────┬────────────┬─────┘
                           │Yes         │No
             ┌─────────────▼─────┐      │
             │ Prompt Admin Setup│      │
             │ (Create user/pass)│      │
             └─────────────┬─────┘      │
                           └──────┬─────┘
                                  │
                     ┌────────────▼───────────┐
                     │  Show Customer Menu    │
                     │  - Browse Products     │
                     │  - Place Orders        │
                     │  - Switch to Admin     │
                     └────────────┬───────────┘
                                  │
                           [Select Admin]
                                  │
                     ┌────────────▼───────────┐
                     │  Admin Login Prompt    │◄───┐ (Max 3 attempts)
                     │  - Enter Password      │    │
                     │  - Press 0 to Cancel   │────┘
                     └────────────┬───────────┘
                                  │
                           [Login Success]
                                  │
                     ┌────────────▼───────────┐
                     │    Show Admin Menu     │
                     │    - Product CRUD      │
                     │    - Bundle CRUD       │
                     │    - Revenue Reports   │
                     └────────────────────────┘
```

### 2.1. First-Run Setup Process

* **Rule**: On startup, the program scans for `admin.txt` or `admin.bin`.
* **Action**: If not found, the program halts standard startup and launches the **Admin Registration Utility**.
* **Input**: Prompts the user to register an administrator username and a secure password.
* **Validation**: Credentials must be at least 4 characters, non-empty, and free of whitespace.
* **Persistence**: Saves the credentials securely to begin standard execution.

### 2.2. Login Verification & Exit Paths

* **Customer Menu Access**: Default mode. No login is needed to read catalogs, search, sort, or add orders.
* **Admin Login Prompt**: Accessed via selection from the main menu. Prompts for username and password.
* **UX Safety Controls**:
  * The user can enter `0` as the username or password to immediately abort the login process and return to the Customer Menu.
  * Maximum of 3 failed login attempts. On the third failure, the app locks the login function for that session (or exits with code `1`).
* **Session Persistence**: Login persists during the program run. Selecting "Logout" returns the interface to Customer mode.

---

## 3. Detailed Functional Requirements

### 3.1. Product Management (Admin Only)

* **ID Assignment**: Unique positive integer assigned to each product.
* **Product Fields**:
  * Name (up to 100 characters)
  * Category ("Phone", "Laptop", "Tablet", "Accessory")
  * Brand (up to 50 characters)
  * Price (positive floating-point value)
  * Stock Quantity (non-negative integer)
* **Deletion Constraint**: A product cannot be deleted if it is currently inside an active bundle. The system must display a warning listing the active bundle(s) and prompt the admin to edit or delete those bundles first.

### 3.2. Dynamic Bundle Management (Admin Only)

* **Bundle Composition**: A bundle is a grouped collection of up to 10 unique product IDs.
* **Pricing Formula**:
  * Final Bundle Price = $\sum (\text{Individual Product Prices}) \times (1.0 - \text{Discount Rate})$
  * The discount rate is validated to be between `0.0` (0%) and `1.0` (100%).
* **Virtual Stock Logic**:
  * Bundles do **not** store an independent stock variable.
  * The available stock of a bundle is computed dynamically whenever queried:
    $$\text{Bundle Stock} = \min_{p \in \text{Bundle}} (\text{Product } p\text{'s Stock})$$
* **Validation**:
  * A bundle must contain at least 1 valid product ID.
  * Attempting to add a duplicate product to the same bundle is blocked.

### 3.3. Search & Sorting (Customer & Admin)

* **Name Search**: Case-insensitive substring matching on product names (e.g., searching `"tab"` matches `"Galaxy Tab S9"`).
* **Category Filtering**: Returns lists containing products matching the selected category.
* **Price Range Filter**: Displays items within $[MinPrice, MaxPrice]$.
* **Bubble Sort Implementations**:
  * Ascending and descending price sorting.
  * Alphabetical name sorting.

### 3.4. Order Processing (Customer & Admin)

* **Placement Requirements**: Customer inputs Customer Name, selects Item ID, selects Item Type (Product or Bundle), and enters Quantity.
* **Validation Logic**:
  * If a product is ordered: verify $\text{Product Stock} \ge \text{Quantity}$.
  * If a bundle is ordered: verify $\text{Virtual Bundle Stock} \ge \text{Quantity}$.
* **Inventory Deduction**:
  * Product Order: Deduct quantity directly from the product's stock.
  * Bundle Order: Deduct quantity from the stock of **all** products associated with that bundle.
* **Metadata Logging**: Records unique Order ID, transaction timestamp (formatted as `YYYY-MM-DD`), total price, and customer details.

### 3.5. Analytics & Statistical Reports (Admin Only)

* **Revenue Report**: Sums the total price of all completed orders.
* **Best Selling Product**: Identifies the individual product with the highest accumulated quantity sold.
* **Best Selling Bundle**: Identifies the bundle with the highest accumulated quantity sold.
* **Inventory Alerts**: Displays a warning list of all products with a stock quantity below $5$.

---

## 4. UI/UX Rules & Safe Input Processing

To ensure standard C console applications do not crash or loop infinitely on wrong inputs:

### 4.1. Safe Input Processing

* **Method**: Direct use of `scanf` for reading numbers is prohibited. All user strings must be read using a custom `fgets`-based buffer processing wrapper.
* **String Parsing**: Use `strtol` / `strtod` to parse numbers. Check the parsed result to ensure the entire input string represents a valid number.
* **Buffer Clear**: Always strip the trailing newline character `\n` and flush extra characters to prevent buffer leak issues.

### 4.2. UX Navigation Standards

* **Exit Protocol**: Every input screen must support typing `0` to safely abort the action and escape to the parent menu.
* **Input Validation Loops**:

  ```text
  DO
    Display input prompt (or 0 to cancel)
    Read input string
    IF input == "0"
       Return CANCEL
    Validate input format
    IF validation fails
       Print "Invalid entry, please try again."
  WHILE validation is false
  ```

---

## 5. Safe Persistence & Transactional I/O

To avoid data corruption during file saving (due to system crashes or exits):

1. **Write to Temp**: Open and write the latest database state to a `.tmp` file (e.g., `products.tmp`).
2. **Safety Check**: Close the `.tmp` file and verify it was created successfully and is not empty.
3. **Backup Existing**: Rename the current active file (e.g., `products.txt`) to a backup file (`products.bak`).
4. **Final Promote**: Rename the temporary file (`products.tmp`) to the active file name (`products.txt`).
5. **Fail-Safe Recovery**: If loading `products.txt` fails on startup (file empty/missing), check for `products.bak`, copy it over, and load from it with a console warning.

---

## 6. Development & Quality Specifications

* **Language Standard**: C17.
* **Architecture**: 64-bit target only (`-m64`).
* **Style Guidelines**: LLVM formatting as specified in [.clang-format](../.clang-format).
* **Static Analysis Guidelines**: Strict rules defined in [.clang-tidy](../.clang-tidy).
