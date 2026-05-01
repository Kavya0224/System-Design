# URL Shortener System - Low Level Design

## Overview

This project implements a simplified **URL Shortener** (similar to bit.ly) using **C++**.

It converts long URLs into short, unique codes and allows retrieval of the original URL using the short code.

The design focuses on **clean architecture**, **extensibility**, and **design patterns**.

---

## Functional Requirements

* Convert a long URL into a short URL
* Retrieve original URL from short URL
* Return same short URL for duplicate long URLs
* Support pluggable storage mechanism

---

## Core Components

### 1. URLShortenerService (Core Controller)

Acts as the main service layer.

**Responsibilities:**

* Handle URL shortening
* Handle URL retrieval
* Coordinate between components

---

### 2. StorageStrategy (Strategy Pattern)

Abstracts storage logic.

```text
save(short, long)
getLong(short)
existsLong(long)
getShort(long)
```

---

### 3. InMemoryStorage

Concrete implementation using `unordered_map`.

**Responsibilities:**

* Store mappings
* Retrieve mappings

---

### 4. Encoder

Encodes numeric IDs into Base62 strings.

**Why Base62?**

* Short and compact
* URL-safe characters
* Avoids collisions

---

### 5. IDGenerator

Generates unique incremental IDs.

---

## System Flow

### Shortening a URL

```text
Long URL
   ↓
Check if already exists
   ↓
Generate unique ID
   ↓
Encode using Base62
   ↓
Store mapping
   ↓
Return short URL
```

---

### Retrieving a URL

```text
Short URL
   ↓
Lookup in storage
   ↓
Return original URL
```

---

## Data Structures Used

```cpp
unordered_map<string, string> shortToLong;
unordered_map<string, string> longToShort;
```

---

## Design Patterns Used

### 1. Strategy Pattern ⭐

* Used for storage abstraction
* Allows switching storage implementations

Examples:

* InMemoryStorage
* DatabaseStorage (future)
* CacheStorage (future)

---

### 2. Dependency Injection

* Storage strategy injected into service
* Improves flexibility and testability

---

## Design Decisions

* Used Base62 encoding for compact URLs
* Used incremental ID to ensure uniqueness
* Avoided hashing to reduce collisions
* Separated storage from business logic
* Kept system modular and extensible

---

## Limitations

* No distributed storage
* No caching layer
* No expiration support
* No analytics (click count)
* No concurrency handling

---

## Future Enhancements

* Add expiry time for URLs
* Add click analytics
* Implement cache (LRU / Redis)
* Use database instead of in-memory storage
* Add custom alias support
* Convert to distributed system

---

## Tech Stack

* Language: C++
* Concepts: OOP, STL, LLD, Design Patterns

---

## Learning Outcome

This project demonstrates:

* Designing scalable services
* Applying Strategy Pattern
* Clean separation of concerns
* Real-world system modeling

---

## Key Takeaway

A URL shortener may seem simple, but it involves:

* efficient encoding
* collision handling
* scalable storage

and is a great example of designing **extensible backend systems**.

