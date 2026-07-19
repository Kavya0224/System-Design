# 🎬 BookMyShow (Movie Ticket Booking) — LLD Interview Guide

## 📋 Problem Statement
Design a movie ticket booking system covering cinemas, shows, seat selection, and booking with concurrency-safe seat locking.

---

## 🎤 Interview Flow

**Q1. Interviewer:** This is a popular one — how do you structure your approach?
**A:** I'd model the domain hierarchy first — `City → Cinema → Screen → Show → Seat` — then focus the design discussion on the hardest part: preventing two users from booking the same seat at the same time.

**Q2. Interviewer:** Good, let's go there directly. How do you prevent double booking?
**A:** When a user selects seats, I don't book immediately — I put them in a temporary `LOCKED` state tied to that user's session with a short TTL (say 5–10 minutes). Only on payment confirmation do locked seats transition to `BOOKED`. If the TTL expires without payment, a background job (or lazy check on next access) releases them back to `AVAILABLE`.

**Q3. Interviewer:** How do you make the lock itself thread-safe in a single-instance system?
**A:** Each `Seat` (per show) has its own state plus a lock object; booking a set of seats acquires locks in a consistent order (e.g., sorted by seat ID) to avoid deadlocks, checks all are `AVAILABLE`, and atomically flips them to `LOCKED` — otherwise it rolls back and informs the user which seats were taken.

**Q4. Interviewer:** What if this needs to scale across multiple servers?
**A:** I'd mention that in a distributed setup, an in-memory lock isn't enough — you'd use a distributed lock (Redis with TTL, e.g., `SETNX`) or push seat-hold state into a shared store. For an LLD interview I'd note this as the natural evolution, while implementing the simpler in-process version.

**Q5. Interviewer:** How do you model seat types and pricing?
**A:** `Seat` has a `SeatType` enum (Regular, Premium, Recliner), and each `Show` has a `PricingStrategy` mapping seat type to price — so the same movie can have different pricing per show/screen (e.g., weekend surcharge).

**Q6. Interviewer:** Walk me through the booking flow end to end.
**A:** User searches shows by movie/city/date → selects a `Show` → selects seats (locked) → `BookingService` validates lock ownership and computes total via `PricingStrategy` → payment → on success, seats move to `BOOKED` and a `Booking` record with a confirmation ID is created; on failure, seats are released.

**Q7. Interviewer:** How would you handle a show getting cancelled after bookings exist?
**A:** `Show.cancel()` iterates its bookings, triggers a refund workflow via a `RefundStrategy`, and notifies booked users — I'd keep refund logic as its own service so it isn't tangled into the booking class.

---

## ✅ Functional Requirements
- Browse movies/shows by city/cinema/date
- Seat selection with temporary hold
- Booking + payment confirmation
- Cancellation/refund handling

## ⚙️ Non-Functional Requirements
- No double-booking under concurrency
- Lock TTL / auto-release
- Extensible pricing per seat type/show

## 🏗️ Core Classes
- `City`, `Cinema`, `Screen`, `Show`, `Movie`
- `Seat` (with `SeatState`: Available/Locked/Booked)
- `Booking`, `BookingService`
- `PricingStrategy`, `RefundStrategy`

## 🎨 Design Patterns Used
- **State** — seat lifecycle
- **Strategy** — pricing, refund policy
- **Singleton** — central `BookingService`/inventory manager

## ⚠️ Edge Cases Handled
- Concurrent seat selection race
- Lock expiry without payment
- Show cancellation with active bookings

## 🚀 Extensibility
- Distributed locking (Redis), dynamic pricing, food/combo add-ons at checkout.
