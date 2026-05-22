# Shopping Cart System - Low Level Design (LLD)

## Overview

This project is a simplified implementation of a Shopping Cart System using C++ and Object-Oriented Design principles.

The system allows users to:
- Browse products
- Add products to cart
- Update product quantities
- Remove products from cart
- View cart details
- Checkout successfully if stock is available

The focus of this project is to understand how an e-commerce shopping cart works internally while applying clean OOP design.

---

# Functional Requirements

- User should be able to add items to the cart
- User should be able to remove items from the cart
- User should be able to update item quantity
- System should validate stock before adding items
- User should be able to view cart contents
- User should be able to checkout
- Inventory should be updated after successful checkout

---

# Non Functional Requirements

## Availability

The shopping cart should always be available for users to perform operations.

## Consistency

The system should ensure:

- Users cannot purchase more items than available in stock
- Inventory updates correctly after checkout
- Cart remains consistent during updates

---

# Core Entities

## User

Represents a customer using the system.

### Fields

- userId
- userName

---

## Product

Represents a sellable product.

### Fields

- productId
- productName
- productPrice

---

## Inventory

Maintains stock information for products.

### Fields

- productId → quantity

### Responsibilities

- Add product stock
- Check stock availability
- Reduce stock after checkout
- Display available inventory

---

## CartItem

Represents a product inside the cart.

### Fields

- Product
- Quantity

### Responsibilities

- Store selected product
- Store quantity
- Calculate total price

---

## Cart

Represents a user's shopping cart.

### Fields

- User
- Inventory Reference
- Cart Items

### Responsibilities

- Add item
- Remove item
- Update quantity
- Display cart
- Checkout

---

# Class Relationships

```text
User
  |
  v
Cart
  |
  +---- CartItem
              |
              v
           Product

Inventory
    |
    v
 Product Stock
```

---

# System Flow

## Add Item Flow

```text
User selects product
        ↓
Check Inventory
        ↓
Stock Available?
        ↓
      YES
        ↓
Add Item To Cart

      NO
        ↓
Show Error
```

---

## Checkout Flow

```text
User Clicks Checkout
        ↓
Validate Stock
        ↓
Stock Available?
        ↓
      YES
        ↓
Reduce Inventory
        ↓
Clear Cart
        ↓
Checkout Successful

      NO
        ↓
Checkout Failed
```

---

# Design Decisions

## Why CartItem?

Instead of storing products directly inside the cart:

```cpp
Cart
    -> Product
```

we use:

```cpp
Cart
    -> CartItem
          -> Product
          -> Quantity
```

because the same product can be added multiple times with varying quantities.

---

## Why Separate Inventory?

Inventory management is independent of cart management.

Benefits:

- Better separation of concerns
- Easier scalability
- Easier inventory tracking

---

## Why Validate Stock Before Checkout?

Consider:

```text
Laptop Stock = 1

User A adds Laptop
User B adds Laptop
```

Only one user should be able to purchase successfully.

Therefore stock validation is performed before checkout.

---

# Data Structures Used

```cpp
unordered_map<int,int>
```

Stores:

```text
ProductId -> Quantity
```

for inventory management.

---

```cpp
unordered_map<int,CartItem>
```

Stores:

```text
ProductId -> CartItem
```

for fast cart operations.

---

# Time Complexity

| Operation | Complexity |
|------------|------------|
| Add Item | O(1) |
| Remove Item | O(1) |
| Update Quantity | O(1) |
| Inventory Lookup | O(1) |
| Checkout | O(n) |

where n = number of items in cart.

---

# Future Enhancements

## Inventory Reservation

Reserve inventory when item is added to cart.

Example:

```text
Stock = 1

User A adds to cart
→ reserve item

User B adds to cart
→ cannot reserve
```

---

## Payment Integration

Support:

- UPI
- Credit Card
- Debit Card
- Wallets

using Strategy Pattern.

---

## Coupon System

Apply:

- Flat discounts
- Percentage discounts
- Cashback offers

---

## Wishlist

Allow users to save products for future purchases.

---

## Persistent Storage

Replace in-memory storage with:

- MySQL
- PostgreSQL
- MongoDB

---

## Multi Seller Support

Support multiple vendors selling the same product.

---

## Thread Safety

Add mutexes or distributed locks to handle concurrent purchases.

---

# Design Patterns That Can Be Added

- Strategy Pattern (Payment)
- Strategy Pattern (Discounts)
- Observer Pattern (Stock Notifications)
- Factory Pattern (Product Creation)

---

# Learning Outcomes

This project helped in understanding:

- Object-Oriented Design
- Entity Modeling
- Inventory Management
- Cart Operations
- Checkout Workflow
- Data Consistency
- Scalable LLD Design

---
