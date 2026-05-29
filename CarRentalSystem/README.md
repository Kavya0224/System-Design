# Car Rental System - Low Level Design (LLD)

## Overview

This project is a simplified implementation of a Car Rental System using C++ and Object-Oriented Design principles.

The system allows users to:

* Search available cars
* Book cars
* Make payments
* Start rides
* Complete rides
* Cancel bookings

The main goal of this project is to understand how real-world rental platforms internally manage:

* inventory
* bookings
* payments
* ride lifecycle
* availability management

---

# Functional Requirements

* User should be able to search available cars
* User should be able to book a car
* User should be able to make payment
* User should be able to start ride
* User should be able to complete ride
* User should be able to cancel booking
* System should prevent double booking

---

# Non Functional Requirements

## Availability

The system should always allow users to search available cars reliably.

---

## Consistency

The same car should not be booked by multiple users simultaneously.

---

## Concurrency

Multiple users may try booking the same car at the same time.
The system should handle concurrent requests safely.

---

# Core Entities

## User

Represents a customer using the rental platform.

### Fields

* userId
* userName
* phoneNumber
* address

---

## Car

Represents a rental vehicle.

### Fields

* carId
* carType
* carName
* availability
* pricePerDay

### Responsibilities

* Maintain availability status
* Provide pricing details
* Handle booking/release operations

---

## Payment

Handles payment processing.

### Responsibilities

* Process payment
* Maintain payment status

### Payment States

```text
PENDING
SUCCESS
FAILED
```

---

## Booking

Represents a car reservation.

### Responsibilities

* Track ride lifecycle
* Start ride
* Complete ride
* Cancel booking

### Booking States

```text
BOOKED
ONGOING
COMPLETED
CANCELLED
```

---

## CarRentalSystem

Acts as the central management system.

### Responsibilities

* Store all cars
* Handle bookings
* Manage inventory
* Coordinate payment and booking flow

---

# System Flow

## Car Booking Flow

```text
Search Car
    ↓
Select Car
    ↓
Check Availability
    ↓
Reserve Car
    ↓
Process Payment
    ↓
Payment Success?
   /        \
 YES         NO
  ↓           ↓
Booking     Release Car
Confirmed
```

---

# Ride Lifecycle

```text
BOOKED
   ↓
ONGOING
   ↓
COMPLETED
```

or

```text
BOOKED
   ↓
CANCELLED
```

---

# Design Decisions

## Why Separate Payment Entity?

Payment is isolated because:

* payment logic may grow independently
* supports multiple payment methods
* improves modularity

---

## Why Availability Inside Car?

Availability is a property of the car itself.

```cpp
bool available;
```

This improves encapsulation.

---

## Why CarRentalSystem Entity?

Without a central system:

* cars become disconnected
* bookings become difficult to manage
* inventory tracking becomes scattered

`CarRentalSystem` acts as the orchestrator.

---

# Data Structures Used

## unordered_map<int, Car>

Stores:

```text
carId -> Car
```

for fast lookup.

---

## vector<Booking>

Stores all booking history.

---

# Time Complexity

| Operation      | Complexity |
| -------------- | ---------- |
| Add Car        | O(1)       |
| Search Car     | O(n)       |
| Book Car       | O(1)       |
| Cancel Booking | O(1)       |
| Car Lookup     | O(1)       |

---

# Edge Cases Handled

## Double Booking Prevention

Before booking:

```cpp
if(!car.isAvailable())
```

prevents multiple users from booking the same car.

---

## Payment Failure

If payment fails:

```cpp
car.releaseCar();
```

releases reserved car.

This prevents permanent locking of inventory.

---

# Limitations

Current implementation does not include:

* Database persistence
* Authentication
* Real payment gateway
* Booking time windows
* Thread safety
* Dynamic pricing
* GPS tracking

---

# Future Enhancements

## Dynamic Pricing

Pricing can depend on:

* demand
* duration
* location
* car type

Can be implemented using:

* Strategy Pattern

---

## Time Slot Booking

Support:

* hourly rentals
* overlapping booking checks
* reservation windows

---

## Thread Safety

Use mutex/locks to prevent race conditions during simultaneous booking requests.

---

## Payment Gateway Integration

Integrate:

* Stripe
* Razorpay
* PayPal

---

## Database Integration

Replace in-memory storage with:

* MySQL
* PostgreSQL
* MongoDB

---

# Design Patterns That Can Be Added

* Strategy Pattern (Pricing)
* Factory Pattern (Car Creation)
* Singleton Pattern (Rental System)
* Observer Pattern (Notifications)

---

# Learning Outcomes

This project helped in understanding:

* Object-Oriented Design
* Booking Lifecycle Management
* Payment Handling
* Inventory Management
* Entity Relationships
* Availability Management
* Concurrency Considerations
* Low Level System Design

---
