# Project Epics Backlog

This backlog splits the requirements into 9 granular Epics, distinguishing core backend logic, user experience (UI/UX), data safety, and development operations.

---

## Epic 1: Repository Setup & Quality Infrastructure

* **Description**: Set up compilation, lint rules, standard formatting configs, and utility functions that support safe inputs.
* **Scope**: Makefile, Clang settings, Github Action configuration, and safe console input wrappers (`utils.h`).

## Epic 2: Admin Security & Default Setup

* **Description**: Manage administrator credentials and first-run prompts.
* **Scope**: Detecting credentials, creating initial username and password, password prompt loops with exit logic (entering `0`).

## Epic 3: Product Inventory Management (CRUD)

* **Description**: Manage individual electronics products in inventory.
* **Scope**: Inserting new items, verifying unique IDs, validating categories/prices, modifying values, and array-shift deletion logic.

## Epic 4: Dynamic Bundle Management & Virtual Stock

* **Description**: Group products into discount bundles with virtual stock calculations.
* **Scope**: Adding products to bundle arrays, calculating final discounted bundle price, and computing minimum stock levels across bundle components dynamically.

## Epic 5: Search, Sorting & Filtering

* **Description**: Help customers and admins locate items efficiently.
* **Scope**: Bubble Sort algorithms for prices and names, case-insensitive substring searching, category filtering, and price boundaries.

## Epic 6: Order Processing & Stock Deduction

* **Description**: Process product and bundle purchases.
* **Scope**: Stock level validation, multi-product stock deductions on bundle orders, and logging order metadata.

## Epic 7: Business Intelligence & Analytics Reports

* **Description**: Display business revenue statistics.
* **Scope**: Calculation of total revenue, identification of best-selling products/bundles based on sold quantities, and alerts for items with stock $< 5$.

## Epic 8: Safe Transactional Persistence

* **Description**: Save and load application state without risking file corruption.
* **Scope**: Safe-saving using `.tmp` and `.bak` files, and loading error handling recovery.

## Epic 9: UI/UX & Main CLI Navigation Menu

* **Description**: Design clean menus and navigational paths.
* **Scope**: The Customer Main Menu, Admin Dashboard Menu, alignments, display grids, and the unified escape key `0`.
