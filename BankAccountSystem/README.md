# Bank Account System - Low Level Design (LLD)

## Overview

This project is a simplified implementation of a Bank Account System using C++ and Object-Oriented Design principles.

The system supports:

* Account creation
* Deposit money
* Withdraw money
* Transfer money between accounts
* Balance management
* Bank account management

The main goal of this project is to understand how banking systems internally manage accounts and transactions while maintaining consistency and reliability.

---

# Functional Requirements

* User should be able to create a bank account
* User should be able to deposit money
* User should be able to withdraw money
* User should be able to transfer money to another account
* User should be able to view account balance
* Bank should manage multiple accounts

---

# Non Functional Requirements

## Consistency

The system should ensure balances remain correct during transactions.

Example:

* Two simultaneous withdrawals should not create incorrect balances
* Transfers should not lose money

---

## Reliability

Transactions should complete safely without corrupting account balances.

---

## Availability

Users should be able to access banking operations reliably.

---

# Core Entities

## User

Represents a bank customer.

### Fields

* userId
* userName
* contactNumber

---

## Account

Represents a bank account.

### Fields

* accountNumber
* balance
* User

### Responsibilities

* Maintain account balance
* Update balance after transactions
* Store account owner details

---

## Transaction

Handles banking operations.

### Responsibilities

* Deposit money
* Withdraw money
* Transfer money
* Maintain transaction status

### Transaction States

```text
PENDING
SUCCESS
FAILED
```

---

## Bank

Represents the bank system.

### Responsibilities

* Store all accounts
* Add new accounts
* Display accounts
* Manage bank-level operations

---

# Class Relationships

```text
User
  ↓
Account
  ↓
Transaction

Bank
  ↓
Stores Multiple Accounts
```

---

# Deposit Flow

```text
User initiates deposit
        ↓
Transaction status = PENDING
        ↓
Balance updated
        ↓
Transaction status = SUCCESS
```

---

# Withdrawal Flow

```text
User initiates withdrawal
        ↓
Check sufficient balance
        ↓
Enough balance?
      /       \
    YES        NO
     ↓          ↓
Update      Transaction
Balance        FAILED
     ↓
SUCCESS
```

---

# Transfer Flow

```text
Withdraw from Sender
        ↓
Success?
      /    \
    YES     NO
     ↓       ↓
Deposit     FAILED
to Receiver
     ↓
SUCCESS
```

---

# Design Decisions

## Why Separate Bank Entity?

The `Bank` entity acts as the central manager for all accounts.

Responsibilities include:

* storing accounts
* managing account lookup
* maintaining centralized operations

Without `Bank`, accounts become disconnected objects.

---

## Why Use Transaction Status?

Banking systems require transaction tracking.

Using statuses like:

```text
PENDING
SUCCESS
FAILED
```

helps in:

* tracking failures
* debugging issues
* ensuring consistency

---

## Why Pass Account By Reference?

```cpp
void deposit(Account& account,int amount)
```

Using references ensures:

* original account updates
* no unnecessary copying
* better performance

Passing by value would modify only a copy.

---

# Data Structures Used

## unordered_map<int,Account>

Stores:

```text
AccountNumber -> Account
```

for fast account lookup.

---

# Time Complexity

| Operation      | Complexity |
| -------------- | ---------- |
| Add Account    | O(1)       |
| Deposit        | O(1)       |
| Withdraw       | O(1)       |
| Transfer       | O(1)       |
| Account Lookup | O(1)       |

---

# Limitations

Current implementation is simplified and does not include:

* Database persistence
* Authentication
* Thread safety
* Real payment processing
* Transaction rollback
* Distributed transactions

---

# Future Enhancements

## Transaction History

Store all transaction logs for each account.

---

## Authentication System

Add:

* PIN
* Password
* OTP verification

---

## Thread Safety

Use mutexes to avoid race conditions during simultaneous transactions.

---

## Transaction Rollback

Prevent issues like:

```text
Money deducted from sender
but not added to receiver
```

using atomic transactions.

---

## Database Integration

Replace in-memory storage with:

* MySQL
* PostgreSQL
* MongoDB

---

## Account Types

Support:

* Savings Account
* Current Account
* Business Account

---

## Interest Calculation

Add monthly/yearly interest functionality.

---

# Design Patterns That Can Be Added

* Strategy Pattern (Interest Calculation)
* Factory Pattern (Account Creation)
* Singleton Pattern (Bank Service)
* Observer Pattern (Transaction Notifications)

---

# Learning Outcomes

This project helped in understanding:

* Object-Oriented Design
* Banking Transaction Modeling
* Entity Relationships
* Transaction Handling
* Consistency in Banking Systems
* Reference vs Value Passing
* Low Level System Design

---

