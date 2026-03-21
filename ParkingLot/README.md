# Parking Lot System - Low Level Design

## 📌 Problem Statement

Design a Parking Lot system that supports:

* Vehicle entry and exit
* Parking spot allocation
* Ticket generation
* Fee calculation
* Multi-floor parking

---

## 🧠 Design Overview

The system models a real-world parking structure using object-oriented design with clear separation of responsibilities.

---

## 🧩 Core Classes

### 1. ParkingLot

* Manages overall system
* Handles entry and exit
* Maintains active tickets

### 2. ParkingFloor

* Contains multiple parking spots
* Assigns available spots
* Releases spots on exit

### 3. ParkingSpot

* Represents a single spot
* Stores type and occupancy status

### 4. Vehicle

* Stores vehicle number and type

### 5. Ticket

* Stores:

  * ticket ID
  * vehicle details
  * entry/exit time
  * assigned spot

### 6. FeeCalculator

* Calculates parking fee based on duration

---

## 🔄 Flow

### Entry

1. Vehicle enters parking lot
2. ParkingLot asks floors for available spot
3. Spot is assigned
4. Ticket is generated

### Exit

1. User provides ticket ID
2. System calculates fare
3. Spot is freed
4. Ticket is closed

---

## 📐 Design Decisions

* ParkingSpot represents a single slot (not multiple)
* ParkingFloor handles spot allocation (not ParkingLot)
* Fee calculation is separated from Ticket (extensible design)
* Ticket acts as a single source of truth for parking session

---

## ⚠️ Edge Cases

* No available spots
* Invalid ticket ID
* Vehicle type mismatch with spot
* Releasing already free spot

---

## 🚀 Possible Improvements

* Add display boards
* Add reservation system
* Add EV charging spots
* Add dynamic pricing

---

## 💡 Learning Outcome

* Entity modeling
* Object relationships
* Separation of concerns
* Real-world system abstraction
