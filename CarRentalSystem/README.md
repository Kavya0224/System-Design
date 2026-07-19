# 🚗 Car Rental System — LLD Interview Guide

## 📋 Problem Statement
Design a car rental system supporting vehicle inventory across locations, reservation/booking, pricing, and return with damage/late-fee checks.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you structure the domain?
**A:** `RentalLocation` owns a fleet of `Vehicle`s (with `VehicleType`: Sedan, SUV, etc.). A `Reservation` books a specific vehicle category (not necessarily one exact car) for a date range at a pickup/drop-off location, and the actual `Vehicle` instance gets assigned at pickup time.

**Q2. Interviewer:** Why book by category rather than a specific car upfront?
**A:** Because between reservation and pickup, exact inventory can shift (a car goes into maintenance, etc.) — booking by category with assignment at pickup gives flexibility, similar to how real rental companies operate ("a car in this class or similar").

**Q3. Interviewer:** How do you check vehicle availability for a date range?
**A:** Each `Vehicle` maintains a sorted list/interval structure of booked date ranges. Checking availability is an interval-overlap query — for a fleet, I'd check category-level available count for the requested range rather than scanning every vehicle, using an interval tree or simply per-day counters for reasonable date ranges.

**Q4. Interviewer:** How do you compute rental price?
**A:** A `PricingStrategy` based on vehicle category's daily rate × number of days, plus optional add-ons (insurance, GPS, child seat) each contributing to the total via a composable list — again Strategy/Decorator-style so pricing rules evolve independently of the reservation flow.

**Q5. Interviewer:** What happens at pickup?
**A:** `Reservation.confirmPickup()` assigns a specific available `Vehicle` from the category, transitions reservation to `ACTIVE`, and records odometer/fuel level for later comparison at return.

**Q6. Interviewer:** How do you handle return — late fees and damage?
**A:** `Reservation.completeReturn()` compares actual return time against agreed drop-off time (late fee via a `LateFeeStrategy` — e.g., per-hour charge beyond a grace period) and compares odometer/fuel/condition notes against pickup baseline to flag damage, which routes to a `DamageAssessmentService` before finalizing the bill.

**Q7. Interviewer:** How would you handle cross-location drop-off (picked up in City A, dropped in City B)?
**A:** Add a `oneWayFee` when `pickupLocation != dropoffLocation`, and on return, the vehicle's location is simply updated to the drop-off location's fleet rather than needing to physically "move" data — the vehicle is now available for booking from City B going forward.

---

## ✅ Functional Requirements
- Search/reserve vehicles by category, date range, location
- Pickup with specific vehicle assignment
- Return with late fee and damage assessment
- Add-ons (insurance, GPS) pricing

## ⚙️ Non-Functional Requirements
- Accurate availability under overlapping date-range bookings
- Extensible pricing/fee strategies
- Cross-location (one-way rental) support

## 🏗️ Core Classes
- `RentalLocation`, `Vehicle`, `VehicleType`
- `Reservation` (context) with `ReservationState`
- `PricingStrategy`, `LateFeeStrategy`, `AddOn`
- `DamageAssessmentService`

## 🎨 Design Patterns Used
- **Strategy** — pricing, late fee
- **State** — reservation lifecycle (Booked → Active → Completed)
- **Decorator-style composition** — add-ons stacking onto base price

## ⚠️ Edge Cases Handled
- Category available but exact car assignment conflict at pickup
- Late return beyond grace period
- One-way (cross-location) rentals

## 🚀 Extensibility
- Loyalty/membership tiers, dynamic demand-based pricing, fleet maintenance scheduling.
