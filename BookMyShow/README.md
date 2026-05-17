# BookMyShow - Low Level Design (LLD)

## Overview

This project is a simplified implementation of a BookMyShow-like movie ticket booking system using C++ and Object-Oriented Design principles.

The system supports:
- Movie shows in theatres
- Seat selection
- Seat locking
- Ticket booking
- Payment processing
- Booking state management
- Concurrent booking handling (basic)

The main focus of this project is designing a scalable and extensible system using proper abstractions and design patterns.

---

# Functional Requirements

- User can browse movies and shows
- User can select seats
- System should prevent double booking
- User can make payment
- Booking should be confirmed only after successful payment
- Seat should be released if payment fails

---

# Non-Functional Requirements

## 1. Concurrency Handling

The system should handle multiple users trying to book the same seat simultaneously.

Implemented using:

AVAILABLE → LOCKED → BOOKED

seat lifecycle.

---

## 2. Consistency

The system ensures:
- successful payment ⇒ seat booked
- failed payment ⇒ seat released

No inconsistent states are allowed.

---

## 3. Scalability

The design separates:
- booking logic
- theatre management
- seat management
- payment flow

allowing future scalability.

---

# Core Entities

## 1. User

Represents customer booking tickets.

### Fields
- userId
- userName

---

## 2. Seat

Represents a seat inside a show.

### Fields
- seatId
- seatType
- seatState

### States

AVAILABLE  
LOCKED  
BOOKED

### Responsibilities
- lock seat
- release seat
- book seat

---

## 3. Show

Represents a movie running at a specific timing.

### Fields
- showId
- movieName
- screenId
- seats
- timing

### Responsibilities
- manage seats
- display seat availability

---

## 4. Theatre

Represents a movie theatre.

### Fields
- theatreId
- theatreName
- location
- shows

### Responsibilities
- manage shows
- display available movies

---

## 5. Booking

Represents a ticket booking transaction.

### Fields
- bookingId
- user
- theatre
- show
- selectedSeats
- bookingState

### Booking States

CREATED  
PAYMENT_PENDING  
CONFIRMED  
FAILED  
CANCELLED

### Responsibilities
- manage selected seats
- process payment
- confirm booking
- release seats on payment failure

---

## 6. BookingManager

Acts as the orchestrator/facade of the system.

### Responsibilities
- validate seat availability
- lock seats
- create booking
- process payment
- handle rollback

---

# System Flow

## Booking Flow

```text
User selects seats
        ↓
Check seat availability
        ↓
Lock seats temporarily
        ↓
Create booking
        ↓
Process payment
        ↓
If success:
    BOOK seats
    CONFIRM booking

If failure:
    RELEASE seats
    FAIL booking
```

---

# Design Patterns Used

## 1. State Pattern (Conceptually)

Used for:
- SeatState
- BookingState

This models the lifecycle of:
- seats
- bookings

---

## 2. Facade Pattern

BookingManager simplifies the booking workflow by hiding internal complexity.

---

## 3. Single Responsibility Principle (SOLID)

Each class has a dedicated responsibility:
- Seat → seat lifecycle
- Booking → booking state
- Theatre → show management

---

# Important Design Decisions

## Why SeatState instead of bool booked?

Using:

```cpp
bool booked;
```

cannot differentiate between:
- LOCKED
- BOOKED

Using explicit states improves:
- readability
- correctness
- scalability

---

## Why Theatre entity?

Without Theatre:
- system becomes single-theatre only
- multiple theatre support becomes difficult

---

## Why lock seats before payment?

To prevent double booking during concurrent booking requests.

---

# Data Structures Used

```cpp
vector<Seat>
vector<Show>
vector<Seat*>
```

---

# Limitations

Current implementation does NOT include:
- real concurrency handling (mutex/thread safety)
- database persistence
- lock timeout expiration
- multiple payment strategies
- search by city/movie
- dynamic pricing

---

# Future Enhancements

## 1. Lock Expiry

Automatically release seats after timeout.

Example:
- 5-minute seat lock

---

## 2. Payment Strategy Pattern

Support:
- UPI
- Credit Card
- Wallet

---

## 3. Movie Entity

Separate movie details:
- language
- genre
- duration
- rating

---

## 4. Screen Entity

Support:
- multiple halls per theatre
- different seat layouts

---

## 5. Database Support

Replace in-memory storage with:
- MySQL
- PostgreSQL
- Redis

---

# Tech Stack

- Language: C++
- Concepts:
  - OOP
  - STL
  - LLD
  - Design Patterns

---

# Learning Outcomes

This project helped in understanding:
- real-world object modeling
- state transitions
- concurrency-related design thinking
- orchestration layers
- scalable LLD architecture

---

# Key Takeaway

A movie booking system is not just about selecting seats.

It involves:
- state management
- concurrency handling
- transactional consistency
- orchestration
- scalable abstractions

making it one of the best intermediate-level LLD problems.

---
