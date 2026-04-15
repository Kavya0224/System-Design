# Ride Sharing System (Uber/Ola) - Low Level Design

## Overview

This project is a simplified **Ride Sharing System** implemented in **C++**, inspired by platforms like Uber/Ola.

It demonstrates how a rider requests a ride, how a driver is assigned, and how the ride progresses through different states.

---

## Functional Requirements

* Rider can request a ride
* System assigns an available driver
* Ride progresses through different states
* Fare is calculated based on distance
* Driver becomes available again after ride completion

---

## Core Components

### 1. Rider

Represents a user requesting a ride.

**Fields:**

* userId
* userName
* userNumber

---

### 2. Driver

Represents a driver in the system.

**Fields:**

* driverId
* driverName
* driverNumber
* availability status

---

### 3. Location

Represents coordinates of a place.

**Fields:**

* x, y

**Responsibility:**

* calculate distance between two locations

---

### 4. Ride

Represents a single ride instance.

**Fields:**

* rideId
* rider
* driver
* startLocation
* endLocation
* state
* fare

---

### 5. RideManager (Core Controller)

Acts as the orchestrator of the system.

**Responsibilities:**

* handle ride requests
* assign drivers
* control ride lifecycle
* store completed rides

---

### 6. DriverAssignStrategy (Strategy Pattern)

Defines how drivers are selected.

---

### 7. FirstFreeDriver Strategy

Assigns the **first available driver**.

---

## Ride Lifecycle

```text
REQUESTED → ACCEPTED → ONGOING → COMPLETED
```

Optional:

```text
CANCELLED
```

---

## Flow

1. Rider requests a ride
2. RideManager uses strategy to find driver
3. Driver is assigned
4. Ride starts
5. Ride completes
6. Fare is calculated

---

## Algorithm Used

### Driver Assignment (Strategy)

* First available driver is selected
* Strategy can be changed dynamically

---

## Design Patterns Used

### 1. Strategy Pattern ⭐

* Used for driver assignment
* Allows switching between:

  * First available driver
  * Nearest driver
  * Highest rated driver

---

### 2. Facade Pattern

* `RideManager` acts as central controller
* Simplifies interaction with system

---

### 3. State Pattern (Basic)

* Ride state handled using enum

---

## Design Patterns That Can Be Added

### 1. Advanced State Pattern

* Replace enum with state classes

### 2. Strategy Pattern for Pricing

* surge pricing
* distance-based pricing
* time-based pricing

### 3. Factory Pattern

* create rides dynamically

---

## Key Design Decisions

* Used pointers for driver assignment to avoid copying
* Used Strategy Pattern for flexibility
* Kept Ride responsible for its lifecycle
* Centralized logic in RideManager

---

## Limitations

* No real-time location tracking
* No concurrency handling
* Simple driver assignment logic
* No cancellation flow implemented
* No payment system

---

## Future Improvements

* Add nearest driver strategy
* Implement surge pricing
* Add ride cancellation
* Add driver rating system
* Add real-time tracking
* Use smart pointers instead of raw pointers

---

## Tech Stack

* Language: C++
* Concepts: OOP, STL, LLD, Design Patterns

---

## Learning Outcome

This project demonstrates:

* real-world system design
* use of Strategy Pattern
* lifecycle management using states
* separation of concerns
* scalable architecture design

---

## Key Takeaway

Ride sharing systems involve:

* intelligent matching
* state management
* extensible design

and are a great example of applying design patterns in real-world systems.

---