# Elevator System - Low Level Design

## Overview

This project is a simple **Elevator System** built in **C++** to practice Low Level Design concepts such as request handling, object coordination, state modeling, and scheduling flow.

The system supports:

* receiving elevator requests
* moving elevators up and down
* handling pickup and drop
* managing multiple elevators through a central manager

---

## Core Components

### 1. Request

Represents one elevator request.

**Fields:**

* `sourceFloor`
* `destinationFloor`
* `direction`

**Responsibilities:**

* store pickup floor
* store destination floor
* derive direction from source and destination

---

### 2. Elevator

Represents a single elevator.

**Fields:**

* `elevatorId`
* `currentFloor`
* `state`
* pending requests
* active request
* pickup status

**Responsibilities:**

* accept assigned requests
* move floor by floor
* first reach source floor
* then drop passenger at destination floor
* maintain current movement state

---

### 3. ElevatorManager

Acts as the main controller of the system.

**Fields:**

* collection of elevators

**Responsibilities:**

* receive incoming requests
* choose an elevator
* assign requests to elevators
* simulate system step by step

---

## Flow

### Request Handling

1. A request is created with source and destination floor
2. `ElevatorManager` receives the request
3. Manager assigns it to an elevator
4. Elevator first moves to pickup floor
5. After pickup, elevator moves to destination floor
6. Request completes after drop

### System Execution

* The system runs in steps
* Each step moves elevators by one floor
* Execution continues until no pending requests remain

---

## Key Design Decisions

* A request is modeled with both **source** and **destination**
* Elevator must handle **pickup first, then drop**
* `ElevatorManager` is responsible for assigning requests
* `Elevator` is responsible for executing assigned requests
* Used step-based simulation to model movement clearly

---

## Why Source and Destination Both Matter

A request like `8 -> 2` should not make the elevator go directly to floor 2 from floor 1.

Correct flow:

* go to floor 8 first
* pick passenger
* then go to floor 2

This is why request stores both pickup and drop information.

---

## Design Patterns Used / Observed

### 1. Facade / Orchestrator Style

`ElevatorManager` behaves like a central coordinator.
The outside world interacts with the manager, and the manager delegates work to elevators.

### 2. State Modeling

The system uses an enum-based state representation:

* `IDLE`
* `MOVING_UP`
* `MOVING_DOWN`

This helps control elevator movement logic.

**Note:**
This is **not a full State Pattern implementation**. It is only state representation using enums.

---

## Design Patterns That Can Be Added Later

### 1. State Pattern

Currently state is handled using enums and conditional logic.
If behavior becomes more complex, it can be refactored into full state classes like:

* `IdleState`
* `MovingUpState`
* `MovingDownState`

This would make behavior cleaner and more extensible.

### 2. Strategy Pattern

Can be used for elevator assignment logic.

Current assignment rule is simple:

* choose first idle elevator

Later this can be replaced with strategies like:

* nearest elevator
* same-direction elevator
* least loaded elevator

### 3. Observer Pattern

Can be added if the system later needs:

* floor display updates
* alerts
* monitoring dashboards

### 4. Command Pattern

Requests can later be modeled as command objects if the system becomes more event-driven.

---

## Edge Cases Considered

* multiple pending requests
* pickup floor different from destination floor
* request direction handling
* no idle elevator available
* continuing simulation until all requests are served

---

## Possible Improvements

* smarter elevator assignment
* support external and internal buttons separately
* add door open/close states
* add elevator capacity
* optimize request scheduling using SCAN-like strategy
* support priority or emergency requests

---

## Learning Outcome

This project helped practice:

* request modeling
* state-based behavior
* centralized orchestration
* pickup vs drop flow handling
* medium-level LLD thinking

---

## Tech Stack

* Language: C++
* Concepts: OOP, STL, LLD

---

## Key Takeaway

A good elevator design is not just about moving between floors.
It is about correctly modeling:

* request flow
* pickup and drop
* state
* request assignment
* system coordination
