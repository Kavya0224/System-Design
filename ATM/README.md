# ATM System - Low Level Design

## 📌 Problem Statement

Design an ATM system that supports:

* Card insertion
* PIN authentication
* Balance inquiry
* Cash withdrawal
* Deposit
* Card ejection

---

## 🧠 Design Overview

The system is designed using object-oriented principles with clear separation of responsibilities.

---

## 🧩 Core Classes

### 1. ATM

* Handles user interaction
* Manages session (card + authentication)
* Delegates operations to Bank

### 2. Bank

* Stores accounts
* Handles:

  * authentication
  * withdraw
  * deposit

### 3. Account

* Stores:

  * account number
  * balance
  * PIN
* Provides:

  * withdraw()
  * deposit()
  * validatePin()

### 4. Card

* Stores:

  * card number
  * linked account number

---

## 🔄 Flow

1. User inserts card
2. ATM reads account number from card
3. User enters PIN
4. ATM validates via Bank
5. User selects operation:

   * check balance
   * withdraw
   * deposit
6. ATM delegates operation to Bank
7. ATM ejects card

---

## 📐 Design Decisions

* ATM does NOT store account data → Bank handles it
* Account handles balance logic → encapsulation
* Card only identifies account → loose coupling
* Used `unordered_map` for fast account lookup

---

## ⚠️ Edge Cases Considered

* Invalid PIN
* Insufficient balance
* No account found
* Unauthorized access

---

## 🚀 Possible Improvements

* Add transaction history
* Add multiple ATM states (State Pattern)
* Add cash availability in ATM
* Add daily withdrawal limits

---

## 🛠 How to Run

```bash
g++ index.cpp -o atm
./atm
```

---

## 💡 Learning Outcome

* Real-world system modeling
* Session handling
* Separation of concerns
* Clean API design
