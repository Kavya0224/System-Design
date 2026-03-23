# Library Management System - Low Level Design

## Overview

This project is a simple **Library Management System** built in **C++** to practice Low Level Design concepts such as object modeling, separation of concerns, and transaction tracking.

The system supports:

* adding books
* adding users
* issuing books
* returning books
* maintaining issue history

---

## Core Components

### 1. Book

Represents a book in the library.

**Fields:**

* `bookId`
* `bookName`
* `available`

**Responsibilities:**

* store book details
* track current availability
* mark issued / returned

---

### 2. User

Represents a library user.

**Fields:**

* `userId`
* `userName`

**Responsibilities:**

* store user information

---

### 3. IssueRecord

Represents one borrowing transaction.

**Fields:**

* `userId`
* `bookId`
* `issueDate`
* `returnDate`

**Responsibilities:**

* track when a book was issued
* track when it was returned
* identify active issue records

---

### 4. Library

Acts as the main controller of the system.

**Fields:**

* `unordered_map<int, Book> books`
* `unordered_map<int, User> users`
* `vector<IssueRecord> records`

**Responsibilities:**

* add books and users
* issue books
* return books
* coordinate updates between current book state and issue history

---

## Flow

### Issue Book

1. Check if user exists
2. Check if book exists
3. Check if the book is available
4. Create a new `IssueRecord`
5. Mark the book as issued

### Return Book

1. Check if user exists
2. Check if book exists
3. Find the active issue record for that user and book
4. Update return date in record
5. Mark the book as returned

---

## Key Design Decisions

* **Book availability** is stored inside `Book` because it represents the current state.
* **Borrowing history** is stored in `IssueRecord` because it represents transactions over time.
* `Library` acts as the main coordinator and keeps the design simple.
* Used `unordered_map` for fast lookup of books and users by ID.
* Used `vector<IssueRecord>` to keep issue/return history.

---

## Design Patterns Used / Observed

### 1. Facade Pattern

`Library` behaves like a **Facade** because it provides a simple interface for the outside world:

* `addBook()`
* `addUser()`
* `issueBook()`
* `returnBook()`

Internally, it coordinates multiple classes like `Book`, `User`, and `IssueRecord`.

### 2. Repository-like Storage

Although not a formal repository implementation, `Library` stores and manages collections of books, users, and records in a centralized way. This resembles a **repository-style approach**.

---

## Design Patterns That Can Be Added Later

### 1. Strategy Pattern

Can be used for:

* fine calculation
* different issue policies
* different borrowing limits for different users

Example:

* student can borrow 3 books
* faculty can borrow 10 books

### 2. Observer Pattern

Can be used for notifications such as:

* due date reminders
* overdue alerts
* book availability notifications

### 3. State Pattern

Right now a book only has a boolean availability flag.
Later, book state can become richer:

* Available
* Issued
* Reserved
* Lost

In that case, **State Pattern** can model book lifecycle more cleanly.

### 4. Factory Pattern

Can be used if the system later supports:

* different user types
* different record types
* different book categories with special rules

---

## Edge Cases Covered

* user does not exist
* book does not exist
* book already issued
* returning a book with no active issue record

---

## Possible Improvements

* add search by book name
* add quantity for multiple copies of same book
* add due date and fine calculation
* support different user roles
* add notification system
* add reservation feature

---

## Learning Outcome

This project helped practice:

* object-oriented design
* separation of current state vs history
* transaction modeling
* collection management using STL
* identifying where design patterns naturally fit

---

## Example Output

* Book issued successfully
* Book not available
* Book returned successfully

---

## Tech Stack

* Language: C++
* Concepts: OOP, STL, LLD
