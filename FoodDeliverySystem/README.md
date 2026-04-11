# Food Delivery System - Low Level Design

## Overview

This project is a simplified **Food Delivery System** (similar to Swiggy/Zomato) implemented in **C++**.

It demonstrates how orders flow from users to restaurants and are delivered using delivery partners.

---

## Functional Requirements

* User can place an order from a restaurant
* Restaurant accepts the order
* Order moves through different states
* Delivery partner is assigned
* Order is delivered to the user

---

## Core Components

### 1. User

Represents a customer placing orders.

**Fields:**

* userName
* userNumber

---

### 2. Item

Represents food items.

**Fields:**

* itemName
* price

---

### 3. Restaurant

Maintains menu and handles order acceptance.

**Responsibilities:**

* store menu items
* validate selected items
* accept/reject orders

---

### 4. Order

Represents a user's order.

**Fields:**

* orderId
* user
* restaurant
* list of items
* state

---

### 5. DeliveryPartner

Represents delivery agents.

**Responsibilities:**

* availability tracking
* assigned to orders

---

### 6. OrderManager (Core Controller)

Acts as the orchestrator.

**Responsibilities:**

* process orders
* manage order lifecycle
* assign delivery partners

---

## Order Lifecycle

```text
CREATED → ACCEPTED → PREPARING → OUT_FOR_DELIVERY → DELIVERED
```

---

## Flow

1. User places order
2. Restaurant accepts
3. Order enters PREPARING state
4. Delivery partner is assigned
5. Order moves to OUT_FOR_DELIVERY
6. Order is DELIVERED

---

## Design Patterns Used

### 1. Facade Pattern

* `OrderManager` acts as a central controller managing the entire flow.

### 2. State Pattern (Basic)

* Order states are modeled using an enum.

---

## Design Patterns That Can Be Added

### 1. Strategy Pattern

* For delivery partner assignment logic (nearest, rating-based, etc.)

### 2. Factory Pattern

* For creating orders or users dynamically

### 3. Full State Pattern

* Replace enum with state classes for better extensibility

---

## Key Design Decisions

* Kept system modular with separate classes
* Used vector and STL containers for simplicity
* Managed order lifecycle centrally
* Used enum for state management

---

## Limitations

* No real-time tracking
* No payment integration
* No concurrency handling
* Simple delivery assignment (first available)

---

## Future Improvements

* Add payment system
* Add location-based delivery assignment
* Support multiple restaurants
* Add order history and tracking
* Convert to distributed system

---

## Tech Stack

* Language: C++
* Concepts: OOP, STL, LLD

---

## Learning Outcome

This project demonstrates:

* real-world system modeling
* state management
* orchestration logic
* extensible system design

---

## Key Takeaway

Food delivery systems are not just about placing orders —
they involve:

* lifecycle management
* resource allocation
* scalable architecture

---

