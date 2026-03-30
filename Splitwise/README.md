# Splitwise System - Low Level Design

## Overview

This project is a simplified **Splitwise System** implemented in **C++** to practice Low Level Design concepts such as expense splitting, balance tracking, transaction history, and system orchestration.

The system supports:

* adding users
* adding expenses
* splitting expenses equally
* tracking balances between users
* storing transaction history
* settling up between users

---

## Core Components

### 1. User

Represents a user in the system.

**Fields:**

* `userId`
* `userName`

**Responsibilities:**

* store user identity
* provide user information

---

### 2. Expense

Represents a single expense.

**Fields:**

* `amount`
* `paidBy`
* `participants`

**Responsibilities:**

* store raw expense data
* maintain list of users involved in the expense

---

### 3. Transaction

Represents a debt relationship between two users.

**Fields:**

* `fromUserId`
* `toUserId`
* `amount`

**Responsibilities:**

* represent who owes whom and how much
* act as a record of expense splits or settlements

---

### 4. Splitwise

Acts as the main controller of the system.

**Fields:**

* `unordered_map<int, User> users`
* `unordered_map<int, unordered_map<int, int>> balances`
* `vector<Transaction> transactions`

**Responsibilities:**

* manage users
* process expenses
* maintain balances
* store transaction history
* handle settlements

---

## Data Structure Design

### Balance Storage

```cpp
balances[A][B] = X
```

Meaning:
**User B owes X amount to User A**

This structure allows:

* fast updates
* easy lookup
* efficient settlement logic

---

## Flow

### Add Expense (Equal Split)

Example:

```
A paid 3000 for A, B, C
```

Steps:

1. Total participants = 3
2. Each share = 1000
3. Update balances:

   * B owes A 1000
   * C owes A 1000
4. Store transactions:

   * (B → A, 1000)
   * (C → A, 1000)

---

### Show Balances

Displays current outstanding balances:

```
B owes 1000 to A
C owes 1000 to A
```

---

### Show Transactions

Displays all recorded transactions:

```
B owes 1000 to A
C owes 1000 to A
```

---

### Settle Up

When a user pays back:

Example:

```
B pays A 500
```

Steps:

1. Reduce balance:

   * B → A decreases by 500
2. Update reverse balance
3. Add transaction record

---

## Key Design Decisions

* **Expense stores raw input**, not final balances
* **Splitwise calculates balances**, not Expense
* **Balances and Transactions are separated**

  * balances = current state
  * transactions = history
* Used `unordered_map` for efficient balance updates

---

## Design Patterns Used / Observed

### 1. Facade / Orchestrator

`Splitwise` acts as a central controller that:

* receives input
* processes logic
* updates balances
* manages transactions

---

## Design Patterns That Can Be Added Later

### 1. Strategy Pattern (IMPORTANT)

Currently only **equal split** is supported.

Future split types:

* Equal Split
* Exact Split
* Percentage Split

Can be implemented using:

```cpp
class SplitStrategy {
    virtual void split(...) = 0;
};
```

This makes the system:

* extensible
* modular
* interview-ready

---

### 2. Observer Pattern

Can be used for:

* notifications ("You owe X")
* reminders
* settlement alerts

---

### 3. Command Pattern

Each expense or settlement can be modeled as a command for:

* logging
* undo functionality
* audit tracking

---

## Edge Cases Considered

* no participants in expense
* user not found
* settlement greater than balance
* duplicate users
* self-expense handling

---

## Possible Improvements

* support multiple split types
* add expense descriptions
* add timestamps
* optimize settlements (minimize transactions)
* support group expenses
* add UI layer

---

## Learning Outcome

This project demonstrates:

* modeling real-world financial interactions
* separating input vs computed data
* maintaining consistent state
* using appropriate data structures
* designing scalable systems

---

## Tech Stack

* Language: C++
* Concepts: OOP, STL, LLD

---

## Key Takeaway

A good Splitwise design separates:

* **Expense input**
* **Balance computation**
* **Transaction history**

and centralizes logic inside a controller for scalability and maintainability.
