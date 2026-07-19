# Low Level Design (LLD) — Complete Interview-Ready Guide

Covers all 19 LLD projects in [Kavya0224/System-Design](https://github.com/Kavya0224/System-Design), each structured the same way:

- **Problem Statement**
- **🎤 Interview Flow** — realistic interviewer Q&A, including a dedicated set of **design-pattern justification questions** (why this pattern, what else was considered, why alternatives were rejected)
- **Functional / Non-Functional Requirements**
- **Core Classes**
- **Design Patterns Used**
- **Edge Cases Handled**
- **Extensibility**

---

## 📑 Navigation

- [🏧 ATM System](#atm)
- [🏦 Bank Account System](#bank-account-system)
- [🎬 BookMyShow (Movie Ticket Booking)](#bookmyshow)
- [🚗 Car Rental System](#car-rental-system)
- [♟️ Chess](#chess)
- [🛗 Elevator System](#elevator)
- [🍔 Food Delivery System](#food-delivery-system)
- [📚 Library Management System](#library-management-system)
- [🎵 Music Player](#music-player)
- [🔔 Notification System](#notification-system)
- [🅿️ Parking Lot System](#parking-lot)
- [🚦 Rate Limiter](#rate-limiter)
- [🚕 Ride Sharing System](#ride-sharing-system)
- [🛒 Shopping Cart (E-commerce)](#shopping-cart)
- [🐍 Snake and Ladder](#snake-and-ladder)
- [💸 Splitwise](#splitwise)
- [🔗 URL Shortener](#url-shortener)
- [🥤 Vending Machine](#vending-machine)
- [⭕ Tic Tac Toe](#tic-tac-toe)

---


<a id="atm"></a>
# 🏧 ATM System — LLD Interview Guide

## 📋 Problem Statement
Design an ATM that supports card authentication, cash withdrawal/deposit, balance inquiry, and PIN validation, using proper state transitions.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How would you approach designing an ATM?
**A:** I'd first nail down the operations: insert card, enter PIN, check balance, withdraw cash, deposit cash, and eject card. Then I'd note that an ATM naturally moves through a sequence of states, so this is a strong candidate for the **State pattern**.

**Q2. Interviewer:** What states did you identify?
**A:** `IdleState` → `HasCardState` (card inserted, awaiting PIN) → `AuthenticatedState` → `TransactionState` (withdraw/deposit/inquiry) → back to `IdleState` on eject. Each state only exposes the operations valid in that state — e.g., you can't withdraw cash before authentication.

**Q3. Interviewer:** Why State pattern over a big switch/if-else on an enum?
**A:** The switch approach scatters state-dependent logic everywhere and grows unmanageable as we add states (e.g., `CardBlockedState`). With the State pattern, each state is its own class implementing a common `ATMState` interface, so adding a new state doesn't touch existing ones — it's cleaner and follows open-closed.

**Q4. Interviewer:** How do you handle cash dispensing when the ATM has limited denominations?
**A:** I'd model a `CashDispenser` holding counts per denomination (2000, 500, 100...), and use a greedy algorithm to break the withdrawal amount into the fewest notes, validating first that the ATM actually has enough total cash and enough of the right denominations to make exact change.

**Q5. Interviewer:** What about incorrect PIN attempts?
**A:** The `HasCardState` tracks failed attempts on the `Card`/`Account`. After 3 failures, the card gets flagged/blocked and the machine transitions to a state that ejects the card and refuses further attempts for that card — this would involve the bank's backend in a real system.

**Q6. Interviewer:** How would you ensure withdrawal doesn't double-deduct if the network fails mid-transaction?
**A:** Good catch — I'd model this as a two-phase operation: reserve/hold the amount on the account, attempt to dispense cash, and only commit the debit after successful dispensing is confirmed. If dispensing fails, the hold is released. This is essentially a mini transaction/rollback pattern.

**Q7. Interviewer:** How does the ATM interact with the bank?
**A:** Through a `BankService` interface abstracting the actual bank API — this decouples the ATM's internal state machine from bank-specific network calls, so we can mock it for testing or extend to multiple banks.

**Q8. Interviewer:** Why State pattern specifically — could you have used Strategy instead?
**A:** Strategy is for interchangeable *algorithms* the caller picks explicitly (e.g., which pricing formula to use). State is for behavior that changes *automatically* based on internal lifecycle, and the object itself controls the transition (`HasCardState` decides to move to `AuthenticatedState` after a correct PIN). Since the ATM's allowed operations change as a side effect of its own history, not by external selection, State fits the semantics better than Strategy.

**Q9. Interviewer:** Did you consider Command pattern anywhere here?
**A:** Yes — for the transaction operations (`WithdrawCommand`, `DepositCommand`, `BalanceInquiryCommand`) if I wanted undo/redo or an audit log of executed actions as objects rather than direct method calls. I didn't make it a hard requirement for the base design since the ATM doesn't need undo, but I'd introduce it if the interviewer asked for a transaction log/replay feature.

**Q10. Interviewer:** Which SOLID principles is this design honoring, and where might it still be violated?
**A:** Open-Closed is honored well — new states or transaction types extend the interfaces without editing existing classes. Single Responsibility holds for `CashDispenser` (only cash logic) and `BankService` (only bank communication). A weak spot: if `ATM` itself starts doing too much orchestration (holding state, cash, and bank calls all inline), it can turn into a God object — I'd keep it as a thin context that delegates to `ATMState`, `CashDispenser`, and `BankService` rather than absorbing their logic.
---

## ✅ Functional Requirements
- Card auth (PIN validation, retry limit)
- Withdraw, deposit, balance inquiry
- Cash dispensing with denomination logic
- Card eject / session timeout

## ⚙️ Non-Functional Requirements
- Consistency during network/hardware failure
- Extensible for new transaction types
- Session isolation between users

## 🏗️ Core Classes
- `ATM` (context) holding current `ATMState`
- `ATMState` (interface) → `IdleState`, `HasCardState`, `AuthenticatedState`, `TransactionState`
- `Card`, `Account`, `CashDispenser`
- `BankService` (interface)

## 🎨 Design Patterns Used
- **State** — ATM lifecycle
- **Strategy** — transaction types (withdraw/deposit/inquiry)
- **Singleton** — single `CashDispenser` per machine

## ⚠️ Edge Cases Handled
- Wrong PIN retry limit
- Insufficient balance / insufficient cash in dispenser
- No exact denomination combination available

## 🚀 Extensibility
- Multi-bank support, contactless/UPI withdrawal, fraud detection hooks.


[⬆ Back to navigation](#-navigation)

---


<a id="bank-account-system"></a>
# 🏦 Bank Account System — LLD Interview Guide

## 📋 Problem Statement
Design a banking system supporting multiple account types, deposits/withdrawals/transfers, and transaction history — with correctness under concurrency.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you model different account types?
**A:** An abstract `Account` class with `balance` and common operations (`deposit`, `withdraw`), with subclasses `SavingsAccount` (interest, minimum balance, withdrawal limits) and `CurrentAccount` (overdraft limit, no interest) overriding withdrawal validation rules specific to their type.

**Q2. Interviewer:** What's the trickiest part of this system to get right?
**A:** Concurrency correctness — specifically, transferring money between two accounts atomically, and avoiding race conditions where concurrent withdrawals could overdraw an account.

**Q3. Interviewer:** How do you make a transfer atomic?
**A:** `TransferService.transfer(fromAccount, toAccount, amount)` needs both the debit and credit to succeed together or not at all. I'd wrap this in a transaction boundary: acquire locks on both accounts in a **consistent global order** (e.g., by account ID) to prevent deadlock, validate sufficient balance, debit source, credit destination, then release locks — if any step fails, the whole operation rolls back.

**Q4. Interviewer:** Why does lock ordering matter?
**A:** If Thread A transfers X→Y (locks X then Y) while Thread B simultaneously transfers Y→X (locks Y then X), they can deadlock — each holding one lock and waiting for the other. Always locking in a fixed order (e.g., lower account ID first) eliminates that cycle.

**Q5. Interviewer:** How do you prevent overdrawing under concurrent withdrawals on the *same* account?
**A:** Synchronize the withdraw operation itself (or use an atomic compare-and-swap on balance): check-then-deduct must be a single atomic step, not two separate operations, or two concurrent withdrawals could both pass the balance check before either deducts.

**Q6. Interviewer:** How do you track transaction history?
**A:** Every successful operation appends an immutable `Transaction` record (type, amount, timestamp, resulting balance, related account for transfers) to that account's ledger — append-only, so history is a reliable audit trail rather than something that can be mutated after the fact.

**Q7. Interviewer:** How would you calculate interest for savings accounts?
**A:** An `InterestStrategy` (Strategy pattern) applied periodically (e.g., monthly) — computing interest on average/minimum monthly balance depending on bank policy — kept separate from the account class so interest rules can change without touching core account logic.

**Q8. Interviewer:** Why Strategy for interest calculation instead of just an if-else on account type?
**A:** If-else on account type duplicates itself everywhere interest matters and grows brittle as new account types are added (student accounts, NRI accounts, etc.). Strategy isolates the *formula* so `Account` doesn't need to know how interest is computed — it just holds a reference to an `InterestStrategy` and delegates, which keeps the class open for new interest schemes without modification.

**Q9. Interviewer:** Could Observer pattern fit anywhere in this system?
**A:** Yes — for notifying a user (SMS/email) whenever a transaction posts. `Account` could expose itself as a subject that `TransactionListener`s subscribe to, decoupling notification concerns from the core debit/credit logic. I didn't build it into the base design since notifications weren't a stated requirement, but it's a natural extension point.

**Q10. Interviewer:** Why not use the Singleton pattern for the whole bank/ledger?
**A:** I deliberately avoided a global Singleton for `Bank` — it would make unit testing harder (shared global mutable state across tests) and doesn't reflect reality where a real system runs across multiple service instances. Instead I'd inject a `BankRepository`/`AccountRepository` per test or per service instance — easier to mock and doesn't create hidden coupling.
---

## ✅ Functional Requirements
- Multiple account types with type-specific rules
- Deposit, withdraw, transfer between accounts
- Transaction history (audit trail)
- Interest calculation for savings accounts

## ⚙️ Non-Functional Requirements
- Atomic transfers (all-or-nothing)
- No overdrawing under concurrency
- Deadlock-free locking

## 🏗️ Core Classes
- `Account` (abstract) → `SavingsAccount`, `CurrentAccount`
- `TransferService` — atomic cross-account transfer
- `Transaction` — immutable ledger entry
- `InterestStrategy` (interface)

## 🎨 Design Patterns Used
- **Strategy** — interest calculation, withdrawal rules per account type
- **Template Method** — shared account operations with type-specific overrides

## ⚠️ Edge Cases Handled
- Concurrent transfer deadlock (via ordered locking)
- Concurrent withdrawal race (atomic check-then-deduct)
- Overdraft limit enforcement (current accounts only)

## 🚀 Extensibility
- Joint accounts, scheduled/recurring transfers, multi-currency support.


[⬆ Back to navigation](#-navigation)

---


<a id="bookmyshow"></a>
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

**Q8. Interviewer:** Why State pattern for seats instead of just a boolean `isBooked` flag?
**A:** A boolean can't express the "temporarily locked, pending payment" state cleanly — you'd end up bolting on extra flags (`isLocked`, `lockExpiry`, `lockedBy`) and if-else checks scattered wherever seat status matters. Modeling it explicitly as `Available → Locked → Booked` states makes illegal transitions (e.g., booking a seat that's still available, or re-locking an already-locked seat) structurally harder to accidentally allow.

**Q9. Interviewer:** Did you consider using Observer here, and where?
**A:** Yes, for notifying the user's UI when a lock is about to expire, and notifying the booking service when payment completes. It keeps the seat-locking logic from needing to know about UI countdown timers or payment callbacks directly.

**Q10. Interviewer:** Why Singleton for the BookingService but not for individual Show/Seat objects?
**A:** There's exactly one coordinator that all requests funnel through, so Singleton fits for orchestration and avoiding duplicate inventory managers. Individual `Show`/`Seat` objects are plain domain entities — making *those* singletons would be wrong since there are legitimately many of them; Singleton should apply to a coordinating service, not to every domain object.
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


[⬆ Back to navigation](#-navigation)

---


<a id="car-rental-system"></a>
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

**Q8. Interviewer:** Why Strategy for pricing/late-fee instead of hardcoding formulas in `Reservation`?
**A:** Rental pricing varies a lot — by location, season, membership tier — and hardcoding it in `Reservation` means every pricing change requires touching core booking logic and risks regressions elsewhere in that class. Strategy isolates "how much to charge" from "how a reservation behaves," so pricing experiments (e.g., A/B testing a new fee model) don't risk booking correctness.

**Q9. Interviewer:** Did you consider Decorator instead of a simple add-ons list?
**A:** I mentioned it as "Decorator-style composition" deliberately — a true Decorator would wrap the base rental price object with `InsuranceDecorator`, `GPSDecorator`, etc., each adding cost and possibly changing behavior (e.g., insurance affecting damage-fee calculation). For a straightforward sum of optional charges, a simple list of `AddOn` objects is simpler and equally correct; I'd upgrade to full Decorator only if add-ons needed to alter *other* behavior, not just add cost.

**Q10. Interviewer:** Why State for reservation lifecycle rather than a plain enum with checks?
**A:** A plain enum plus scattered if-checks (`if status == BOOKED and now > pickupDate...`) spreads lifecycle rules across the codebase. State pattern keeps each stage's valid actions inside its own class, so, e.g., `completeReturn()` called on a `Booked`-but-not-yet-`Active` reservation is a compile-time-obvious misuse rather than a runtime bug waiting to happen.
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


[⬆ Back to navigation](#-navigation)

---


<a id="chess"></a>
# ♟️ Chess — LLD Interview Guide

## 📋 Problem Statement
Design a two-player chess game engine covering the board, pieces, legal move validation, turn management, check/checkmate detection.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you model the pieces and their movement rules?
**A:** An abstract `Piece` class with `color` and an abstract `canMove(Board, from, to)` method, implemented differently by each subclass — `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`. This is a clean use of **polymorphism**: the board doesn't need to know piece-specific rules, it just asks the piece whether a move is legal.

**Q2. Interviewer:** How do you represent the board itself?
**A:** An 8x8 grid, e.g., `Cell[][]` where each `Cell` optionally holds a `Piece` reference. `Board` exposes `getPieceAt(position)` and `movePiece(from, to)`, keeping piece-placement state centralized.

**Q3. Interviewer:** Some pieces (Rook, Bishop, Queen) move in a line and can be blocked — how do you handle that generically?
**A:** I'd give `Piece` a helper that generates the path of intermediate cells between `from` and `to`, and the `Board` checks none of them are occupied (except the destination, which just needs to not hold a same-color piece). Knight is the exception — it jumps, so it skips the path-blocking check entirely, which naturally falls out of not calling that helper in `Knight.canMove()`.

**Q4. Interviewer:** How do you detect check and checkmate?
**A:** After any candidate move, I simulate it on a temporary board state and check if the moving player's own `King` would be under attack by any opposing piece (`isKingInCheck`) — if so, the move is illegal (you can't leave your own king in check). Checkmate is: current player is in check, AND no legal move exists across all their pieces that removes the check — I'd iterate all pieces' legal moves and simulate each.

**Q5. Interviewer:** That sounds expensive — how do you make check detection efficient?
**A:** For an LLD-scope implementation, brute-force simulation per candidate move is acceptable and clear; I'd mention that a production chess engine optimizes this with bitboards and incremental attack-map updates, but that's beyond LLD scope — I'd flag the trade-off explicitly rather than silently under-engineering it.

**Q6. Interviewer:** How do you manage turns and move history?
**A:** A `Game` class holding `currentTurn` (alternating White/Black), validating that only the current player's pieces can move, and appending each executed move to a `MoveHistory` list — useful for undo, replay, and detecting special rules like castling eligibility or en passant.

**Q7. Interviewer:** How would you handle special moves like castling, en passant, and pawn promotion?
**A:** Each is a piece/board-state-dependent exception: castling checks neither King nor the specific Rook has moved before, with no pieces between and king not passing through check; en passant checks the last move was a two-square pawn advance adjacent to the capturing pawn; promotion triggers when a pawn reaches the last rank, prompting the player to choose a replacement piece — I'd implement these as extra checks layered on top of the base move validation rather than baked into `Piece.canMove()`, to keep that method focused.

**Q8. Interviewer:** Why polymorphism/Strategy for piece movement instead of one big `isValidMove()` function with a switch on piece type?
**A:** A single switch-based function grows into an unmanageable wall of piece-specific logic and violates open-closed — adding a new piece type (e.g., a custom variant piece) means editing that shared function and risking breaking existing piece rules. Polymorphism lets each piece own its rules, so `Board`/`Game` code never needs to change when piece behavior changes.

**Q9. Interviewer:** Did you consider Command pattern here, and why/why not fully commit to it?
**A:** Yes — `Move` objects as commands are useful for undo/redo and move history/replay, which chess naturally needs (e.g., "undo last move," PGN export). I do use `Move` objects for history, so this is partially Command-pattern-flavored; I stopped short of building a full undo stack unless the interviewer specifically asks for it, to avoid over-engineering the base answer.

**Q10. Interviewer:** Why not Visitor pattern for operations like "find all legal moves" or "evaluate board" across piece types?
**A:** Visitor is a reasonable alternative when you need many *unrelated* operations to traverse the same class hierarchy without polluting each piece class. For this scope, `canMove()` is really the only piece-specific behavior needed, so plain polymorphism is simpler. I'd bring in Visitor if the design grew multiple analysis passes (move generation, board evaluation, threat detection) all needing per-piece-type logic, since that's exactly the "many operations, one hierarchy" case Visitor solves.
---

## ✅ Functional Requirements
- Legal move validation per piece type
- Turn management, check/checkmate detection
- Special moves: castling, en passant, promotion
- Move history

## ⚙️ Non-Functional Requirements
- Clear separation between piece rules and board/game state
- Extensible for variants (e.g., Chess960)

## 🏗️ Core Classes
- `Board`, `Cell`
- `Piece` (abstract) → `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`
- `Game` — turn management, move history
- `Move`, `MoveValidator`

## 🎨 Design Patterns Used
- **Polymorphism / Strategy** — per-piece move rules
- **Command** — representing moves for history/undo
- **Singleton** — single `Game` instance per match

## ⚠️ Edge Cases Handled
- Moves that leave own king in check (illegal)
- Castling/en passant preconditions
- Pawn promotion choice

## 🚀 Extensibility
- Multiplayer over network, AI opponent (minimax), timed games (chess clock).


[⬆ Back to navigation](#-navigation)

---


<a id="elevator"></a>
# 🛗 Elevator System — LLD Interview Guide

## 📋 Problem Statement
Design a multi-elevator system for a building that efficiently services floor requests using an intelligent scheduling algorithm.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you begin designing an elevator system?
**A:** I'd clarify: how many elevators, how many floors, do we need direction-aware external requests (up/down buttons on each floor) plus internal requests (floor buttons inside the cabin)? I'll assume multiple elevators, external hall calls with direction, and internal car calls.

**Q2. Interviewer:** What's the core scheduling challenge?
**A:** Deciding *which* elevator responds to a hall call, and *in what order* a single elevator services its pending requests. I'd use the classic **SCAN/LOOK algorithm** — an elevator keeps moving in one direction, serving all requests along the way, before reversing — instead of naive FIFO, which causes unnecessary direction changes.

**Q3. Interviewer:** How do you decide which elevator to dispatch for a new hall call?
**A:** I'd score each elevator by proximity and direction compatibility: an idle elevator gets highest priority if nearby; a moving elevator already heading toward the requested floor in the same direction is next; and I'd avoid dispatching one moving away. This could be a pluggable `SchedulingStrategy` so we can swap in more advanced algorithms later.

**Q4. Interviewer:** What states does an elevator go through?
**A:** `IDLE`, `MOVING_UP`, `MOVING_DOWN`, `DOOR_OPEN`, `MAINTENANCE`. I'd model this with the **State pattern** again — each state governs which transitions are valid, e.g., you can't accept a new internal request while in `MAINTENANCE`.

**Q5. Interviewer:** How do you manage the elevator's request queue?
**A:** Two priority queues (or sorted sets) per elevator — one for up-requests, one for down-requests — so the elevator always services the nearest floor in its current direction first, then flips.

**Q6. Interviewer:** How would the `ElevatorController` tie this together?
**A:** It's a Singleton-ish coordinator holding all `Elevator` instances. On a hall call, it runs the dispatch strategy to pick the best elevator and enqueues the request there. It also subscribes to each elevator's state changes to keep dispatch decisions up to date.

**Q7. Interviewer:** How do you handle capacity/overload?
**A:** Each `Elevator` tracks `currentLoad` vs `maxCapacity`; the door won't close (or a buzzer/alert fires) if weight exceeds capacity — modeled as a simple guard check before state transition to `MOVING`.

**Q8. Interviewer:** Why State for the elevator instead of a simple `direction` enum with if-checks?
**A:** Direction alone doesn't capture everything — `MAINTENANCE` and `DOOR_OPEN` aren't directions, they're modes that restrict what operations are valid (you shouldn't accept new car calls while doors are open or the elevator's under maintenance). State pattern lets each mode define its own allowed transitions, instead of an if-else chain checking multiple flags together (`if direction == UP and !doorsOpen and !maintenance...`).

**Q9. Interviewer:** Why Strategy for the scheduling algorithm — what's the alternative and why reject it?
**A:** The alternative is hardcoding SCAN/LOOK directly into `ElevatorController`. That works until the interviewer (or real requirements) asks for a different policy — e.g., zone-based dispatch during a rush hour, or destination-dispatch elevators. Strategy means swapping `NearestElevatorStrategy` for `ZoneBasedStrategy` doesn't touch the controller's core dispatch loop.

**Q10. Interviewer:** Could Observer and State work together here — how?
**A:** Yes — `Elevator` is both a `State`-driven context internally, and an `Observable` externally: whenever its internal state transitions (say `MovingUp → DoorOpen`), it notifies the `ElevatorController` (Observer) so dispatch decisions always see fresh state, rather than the controller polling every elevator on a timer.
---

## ✅ Functional Requirements
- Hall calls (up/down per floor) and car calls (internal floor selection)
- Multiple elevators with independent state
- Capacity limit enforcement
- Efficient dispatch minimizing wait/travel time

## ⚙️ Non-Functional Requirements
- Scalable to N elevators / N floors
- Pluggable scheduling strategy
- Thread-safety for concurrent requests

## 🏗️ Core Classes
- `ElevatorController` — dispatch coordinator
- `Elevator` (context) with `ElevatorState`
- `ElevatorState` (interface) → `IdleState`, `MovingUpState`, `MovingDownState`, `MaintenanceState`
- `Request` (hall/car call, floor, direction)
- `SchedulingStrategy` (interface) → `NearestElevatorStrategy`, `LookAlgorithmStrategy`

## 🎨 Design Patterns Used
- **State** — elevator lifecycle
- **Strategy** — scheduling algorithm
- **Observer** — controller listening to elevator state changes

## ⚠️ Edge Cases Handled
- All elevators busy/full — request queued
- Overweight cabin
- Elevator in maintenance mode

## 🚀 Extensibility
- Priority elevators (fire/VIP), zone-based scheduling for peak hours, predictive dispatch using historical traffic patterns.


[⬆ Back to navigation](#-navigation)

---


<a id="food-delivery-system"></a>
# 🍔 Food Delivery System — LLD Interview Guide

## 📋 Problem Statement
Design a food delivery platform (like Swiggy/Zomato) covering restaurant menus, order placement, delivery agent assignment, and order tracking.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How would you break this problem down?
**A:** Into four sub-domains: restaurant/menu management, order placement & lifecycle, delivery-agent assignment, and payment. I'd design each as loosely coupled services/classes communicating through well-defined interfaces.

**Q2. Interviewer:** How do you model the menu and cart?
**A:** `Restaurant` owns a `Menu` composed of `MenuItem`s (name, price, availability). A `Cart` belongs to a `User`, holds `CartItem`s referencing a `MenuItem` + quantity, and is tied to exactly one restaurant at a time — I'd enforce that in `addItem()` to prevent mixed-restaurant carts, matching real app behavior.

**Q3. Interviewer:** Walk me through order placement.
**A:** `Cart.checkout()` creates an `Order` in `PLACED` state, snapshotting item prices at that moment (so later menu price changes don't retroactively affect past orders), then triggers `OrderService` to notify the restaurant and kick off delivery assignment.

**Q4. Interviewer:** What states does an order go through?
**A:** `PLACED → CONFIRMED → PREPARING → OUT_FOR_DELIVERY → DELIVERED` (or `CANCELLED`). State pattern again, since each stage restricts valid next actions and who (restaurant, agent, system) can trigger them.

**Q5. Interviewer:** How do you assign a delivery agent?
**A:** A `DeliveryAssignmentStrategy` (Strategy pattern) — for LLD scope, pick the nearest available agent to the restaurant. Once `PREPARING` transitions to done, the assigned agent is notified via **Observer**, and the order moves to `OUT_FOR_DELIVERY`.

**Q6. Interviewer:** How do you handle an item going out of stock mid-order?
**A:** `MenuItem.available` is checked at both add-to-cart and checkout time; if unavailable at checkout, that line item is flagged back to the user for removal/substitution before the order is finalized — I wouldn't silently drop it.

**Q7. Interviewer:** How would you calculate final order cost including delivery fee and discounts?
**A:** A `PriceCalculator` combining item subtotal, a `DeliveryFeeStrategy` (distance/time-based), taxes, and an optional `DiscountStrategy` (coupon code, percentage-off) — each pluggable so promotions can change without touching order logic.

**Q8. Interviewer:** Why State pattern for `Order` rather than a status string field?
**A:** A plain status string invites invalid transitions (nothing stops someone from setting status directly to `DELIVERED` from `PLACED`, skipping preparation entirely). State pattern encodes *who* can trigger *which* transition and from *which* prior state, catching such bugs at the design level instead of relying on scattered validation checks.

**Q9. Interviewer:** Why not use Chain of Responsibility for the discount/fee calculation pipeline instead of Strategy?
**A:** Chain of Responsibility fits when steps should potentially short-circuit or pass through sequentially with each step deciding whether to act (like the Notification System's validation pipeline). Here, delivery fee, tax, and discount are independent, always-applied components summed together — that's better expressed as a set of composable Strategies feeding into `PriceCalculator` than a chain, since none of them should "stop" the calculation.

**Q10. Interviewer:** Where would Observer fit, and did you use it elsewhere too?
**A:** Observer notifies the restaurant, delivery agent, and customer as order status changes — each interested party subscribes without `Order` needing direct references to UI/notification code. I use it similarly in Elevator (controller watching elevators) and BookMyShow (lock-expiry/payment notifications) — it's a recurring pattern anywhere one core object's state change needs to fan out to multiple independent listeners.
---

## ✅ Functional Requirements
- Browse restaurant menus, manage cart (single-restaurant constraint)
- Place order, track order lifecycle
- Assign nearest delivery agent
- Price calculation with fees/discounts

## ⚙️ Non-Functional Requirements
- Price snapshot immutability per order
- Extensible fee/discount/assignment strategies
- Real-time order status updates

## 🏗️ Core Classes
- `Restaurant`, `Menu`, `MenuItem`, `Cart`, `CartItem`
- `Order` (context) with `OrderState`
- `DeliveryAgent`, `DeliveryAssignmentStrategy`
- `PriceCalculator`, `DiscountStrategy`, `DeliveryFeeStrategy`

## 🎨 Design Patterns Used
- **State** — order lifecycle
- **Strategy** — assignment, pricing, discounts
- **Observer** — status notifications to user/restaurant/agent

## ⚠️ Edge Cases Handled
- Mixed-restaurant cart prevention
- Item unavailable at checkout
- No delivery agent available nearby

## 🚀 Extensibility
- Scheduled orders, multi-restaurant "food court" orders, live GPS tracking integration.


[⬆ Back to navigation](#-navigation)

---


<a id="library-management-system"></a>
# 📚 Library Management System — LLD Interview Guide

## 📋 Problem Statement
Design a library system supporting book catalog management, member registration, book issue/return, and fine calculation.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you approach this?
**A:** I'd separate concerns into catalog (books, copies, search), membership (members, borrowing limits), and transactions (issue, return, fines). This mirrors a real library's operational split.

**Q2. Interviewer:** A book title can have multiple physical copies — how do you model that?
**A:** I'd distinguish `Book` (metadata: title, author, ISBN) from `BookCopy` (a specific physical copy with a unique barcode/ID and its own `Status`: Available, Issued, Reserved, Lost). Search operates on `Book`; issue/return operates on a specific `BookCopy`.

**Q3. Interviewer:** How do you handle issuing a book to a member?
**A:** `LibraryService.issueBook(member, bookCopy)` checks: member's active loan count is under their limit, member has no unpaid fines above a threshold, and the copy is `Available`. If all pass, it creates a `Loan` record with due date and flips the copy to `Issued`.

**Q4. Interviewer:** How do you calculate fines for late returns?
**A:** A `FineCalculationStrategy` — e.g., flat rate per day overdue, possibly capped at a max fine — computed on return by diffing `returnDate` against `Loan.dueDate`. I'd keep this pluggable since fine policy often differs by member type (student vs. faculty).

**Q5. Interviewer:** What if a book is currently issued and another member wants it?
**A:** They can place a `Reservation`. When the current holder returns it, the system checks the reservation queue (FIFO) and, if one exists, holds the copy for that member for a limited pickup window instead of returning it to general availability.

**Q6. Interviewer:** How would you support searching the catalog efficiently?
**A:** Index `Book`s by title, author, and ISBN in separate maps/inverted indices for O(1)/O(log n) lookup rather than linear scan, and combine results for multi-field search queries.

**Q7. Interviewer:** How do you enforce different borrowing rules for different member types?
**A:** `Member` (abstract) → `Student`, `Faculty`, `Guest`, each overriding `maxBooksAllowed()` and `loanDurationDays()` — Template Method / polymorphism keeps rule differences isolated per subclass rather than if-else checks scattered through the service.

**Q8. Interviewer:** Why inheritance/Template Method for member types instead of a `MemberType` enum with a lookup table of rules?
**A:** An enum + lookup table works for now, but it means adding a new member type (say, "Alumni") requires editing a shared table/switch rather than adding a self-contained class. Subclassing `Member` keeps each type's rules (`maxBooksAllowed()`, `loanDurationDays()`) local to that type and lets me override just what differs, honoring open-closed.

**Q9. Interviewer:** Did you consider Strategy instead of inheritance for the member rules?
**A:** It's a fair alternative — a `BorrowingPolicy` strategy injected into a single concrete `Member` class avoids a class-per-type hierarchy and is more flexible if a member's policy needs to change at runtime (e.g., a student temporarily granted faculty-level limits). I leaned toward inheritance here because member type is usually fixed for the member's lifetime, but I'd switch to Strategy without much rework if runtime policy changes became a requirement.

**Q10. Interviewer:** Why Observer for reservations rather than the requesting member polling availability?
**A:** Polling wastes resources and adds latency to notify the member. Observer means the book copy publishes an "available" event exactly once, at the moment it happens, and the reservation queue is notified immediately — more efficient and simpler than a background polling job.
---

## ✅ Functional Requirements
- Catalog search, book issue/return
- Member types with different borrowing limits
- Reservation queue for unavailable books
- Fine calculation on overdue returns

## ⚙️ Non-Functional Requirements
- Fast catalog search
- Consistent copy status under concurrent issue/return
- Extensible fine/borrowing policy per member type

## 🏗️ Core Classes
- `Book`, `BookCopy` (with `CopyStatus`)
- `Member` (abstract) → `Student`, `Faculty`, `Guest`
- `Loan`, `Reservation`
- `FineCalculationStrategy`

## 🎨 Design Patterns Used
- **Strategy** — fine calculation
- **Template Method / Inheritance** — member-type-specific rules
- **Observer** — notify reserving member when a copy becomes available

## ⚠️ Edge Cases Handled
- Member exceeding borrow limit or unpaid fines
- Reservation pickup window expiry
- Lost/damaged copy handling

## 🚀 Extensibility
- E-books/digital lending, inter-library loan requests, self-checkout kiosk integration.


[⬆ Back to navigation](#-navigation)

---


<a id="music-player"></a>
# 🎵 Music Player — LLD Interview Guide

## 📋 Problem Statement
Design a music player supporting playlists, playback controls, shuffle/repeat modes, and queue management.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you model the core playback behavior?
**A:** A `MusicPlayer` (context) with a `PlayerState` (**State pattern**) — `PlayingState`, `PausedState`, `StoppedState` — so operations like `pause()` behave correctly no matter the current state (e.g., calling `pause()` while already `Stopped` is a no-op instead of undefined behavior).

**Q2. Interviewer:** How do you manage the playback queue and track order?
**A:** A `Playlist` holds an ordered list of `Song`s. For "current track" navigation, I'd maintain an index pointer plus `next()`/`previous()`. Shuffle and repeat change *how* that navigation resolves — which points to Strategy again.

**Q3. Interviewer:** Explain how you'd implement shuffle without breaking "previous" navigation.
**A:** A naive random pick each time makes "previous" meaningless. Instead, on enabling shuffle, I generate a shuffled index order (a permutation of the playlist) once, and navigation walks through *that* order — so `next()`/`previous()` remain consistent and reversible within the shuffled sequence. Turning shuffle off reverts navigation to the original order, preserving the current song's position.

**Q4. Interviewer:** How do you handle repeat modes (Off / RepeatOne / RepeatAll)?
**A:** A `RepeatStrategy` interface: `RepeatOffStrategy` (stop at playlist end), `RepeatOneStrategy` (replay current track on completion), `RepeatAllStrategy` (loop back to index 0). `MusicPlayer.onTrackFinished()` delegates to the active strategy to decide the next action — clean separation from playback state itself.

**Q5. Interviewer:** How would you support multiple playlists and switching between them mid-playback?
**A:** `MusicPlayer` holds a reference to the `currentPlaylist`; switching playlists pauses/stops current playback (transition through `PlayerState`), loads the new playlist, and resets navigation index — I'd trigger this explicitly rather than allow silent mid-song playlist swaps that leave state inconsistent.

**Q6. Interviewer:** How do you notify a UI of playback progress/track changes?
**A:** **Observer pattern** — UI components subscribe to the `MusicPlayer` as `PlaybackListener`s; the player fires `onTrackChanged`, `onProgressUpdate`, `onStateChanged` events, decoupling playback logic from any specific UI framework.

**Q7. Interviewer:** How would you add offline/download support or streaming quality selection?
**A:** Introduce a `Song` abstraction with a `SongSource` (Local file vs. `StreamSource`), and a `StreamingQualityStrategy` chosen based on network conditions — the player just calls `source.getAudioStream()` without caring whether it's local or remote, keeping playback logic source-agnostic.

**Q8. Interviewer:** Why separate State (playback) from Strategy (repeat mode) instead of one combined concept?
**A:** They vary independently — you can be `Playing` with `RepeatAll` or `Paused` with `RepeatOne`; conflating them into one enum would force handling every combination explicitly (`PlayingRepeatAll`, `PlayingRepeatOne`...), which multiplies states unnecessarily. Keeping them orthogonal — State governs *is it playing*, Strategy governs *what happens after a track ends* — keeps each simple and independently extensible.

**Q9. Interviewer:** Why not use Command pattern for playback controls (play/pause/skip)?
**A:** Command would help if I needed to queue, log, or undo control actions — e.g., a "smart speaker" voice command queue, or a macro like "play X then pause after 10 minutes." For a straightforward player, direct method calls on `MusicPlayer` are simpler and sufficient; I'd bring in Command specifically if remote/scripted control became a requirement.

**Q10. Interviewer:** Why Observer instead of just having the UI poll `getCurrentProgress()` on a timer?
**A:** Polling from the UI works but wastes cycles checking even when nothing's changed, and it couples the UI to knowing a poll interval that matches playback granularity. Observer lets the player push updates only when something actually changes (`onProgressUpdate`, `onTrackChanged`), which is both more efficient and keeps the UI reactive rather than guessing when to refresh.
---

## ✅ Functional Requirements
- Play/pause/stop/next/previous
- Playlist management, queue navigation
- Shuffle and repeat modes
- Playback progress notifications

## ⚙️ Non-Functional Requirements
- Consistent navigation under shuffle
- Extensible for streaming/offline sources
- Decoupled UI notification

## 🏗️ Core Classes
- `MusicPlayer` (context) with `PlayerState`
- `Playlist`, `Song`, `SongSource`
- `RepeatStrategy` (interface) → `RepeatOff`, `RepeatOne`, `RepeatAll`
- `PlaybackListener` (Observer interface)

## 🎨 Design Patterns Used
- **State** — playback state
- **Strategy** — repeat mode, streaming quality
- **Observer** — UI/progress notifications

## ⚠️ Edge Cases Handled
- Pause/stop calls in already-paused/stopped state (no-op, not crash)
- Shuffle-consistent previous/next navigation
- Mid-song playlist switch

## 🚀 Extensibility
- Cross-device sync, collaborative playlists, offline downloads with quality selection.


[⬆ Back to navigation](#-navigation)

---


<a id="notification-system"></a>
# 🔔 Notification System — LLD Interview Guide

## 📋 Problem Statement
Design a notification system that sends alerts to users through multiple channels (email, SMS, push) with templating and delivery tracking.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you scope this?
**A:** I'd clarify: how many channels (email/SMS/push/in-app)? Do users choose preferred channels? Do we need retry-on-failure and delivery status tracking? I'll assume multi-channel, user preferences, and retry/tracking are all in scope.

**Q2. Interviewer:** How do you support multiple channels cleanly?
**A:** A `NotificationChannel` interface with a `send(Notification)` method, implemented by `EmailChannel`, `SMSChannel`, `PushChannel`. The core `NotificationService` doesn't know channel-specific details — it just iterates the channels relevant to a request. This is the **Strategy pattern** applied to delivery mechanism.

**Q3. Interviewer:** A user might want the same notification sent to multiple channels — how do you avoid duplicating the send logic?
**A:** I'd combine Strategy with a simple **Decorator**-like composite: `NotificationService.notify(user, message)` looks up the user's enabled channels and dispatches to each independently, collecting per-channel results — so failure in one channel (e.g., SMS provider down) doesn't block others.

**Q4. Interviewer:** How do you handle notification templates (e.g., "Your OTP is {code}")?
**A:** A `NotificationTemplate` with placeholders, resolved via a simple templating step before dispatch — `TemplateEngine.render(templateId, params)`. This keeps message content out of business logic and lets templates be managed/localized independently.

**Q5. Interviewer:** How do you build the notification pipeline itself — validation, rate-limiting, sending, logging?
**A:** I'd use **Chain of Responsibility**: a `NotificationHandler` chain where each handler does one job — `ValidationHandler` → `RateLimitHandler` (don't spam the same user) → `DeliveryHandler` → `LoggingHandler`. Each can short-circuit the chain on failure, and it's easy to insert new steps like a `DedupeHandler`.

**Q6. Interviewer:** How would you track delivery status and retries?
**A:** Each dispatch creates a `NotificationRecord` with status (`PENDING → SENT/FAILED`), and failed sends go into a retry queue with exponential backoff, capped at a max retry count before being marked `FAILED_PERMANENT` — I'd model this as an **Observer**: the channel notifies a `DeliveryTracker` on completion/failure.

**Q7. Interviewer:** How do you respect user notification preferences (e.g., muted at night, opted out of SMS)?
**A:** A `NotificationPreference` per user checked before dispatch — filtering out disabled channels, and a `DoNotDisturb` window check that queues low-priority notifications for later delivery instead of dropping them, while critical ones (like security alerts) bypass DND.

**Q8. Interviewer:** Why Chain of Responsibility for the pipeline instead of just calling validate(), then rateLimit(), then send() directly in sequence?
**A:** Calling them directly works but hardcodes the exact sequence and makes it awkward to conditionally skip/insert steps (e.g., skip rate-limiting for critical security alerts). Chain of Responsibility lets each handler decide independently whether to pass along, short-circuit, or hand off — and reordering or inserting a new handler (e.g., `DedupeHandler`) means changing how the chain is assembled, not editing each handler's internals.

**Q9. Interviewer:** Why Strategy for channels instead of just an if-else on channel type inside `NotificationService`?
**A:** Same reasoning as elsewhere — if-else on type grows unwieldy as channels increase (email, SMS, push, WhatsApp, Slack...) and mixes channel-specific API details into the core service. Strategy means `EmailChannel`'s SMTP details never leak into `NotificationService`, which just knows "channels implement `send()`."

**Q10. Interviewer:** Did you consider Decorator for something like "add a signature/footer to every email" or "encrypt SMS payloads"?
**A:** Yes — that's a legitimate Decorator use case: wrapping a base channel with `SignatureDecorator` or `EncryptionDecorator` that augments the message before delegating to the wrapped channel's `send()`. I didn't build it into the base answer since it wasn't a stated requirement, but it composes cleanly with the existing Strategy-based channels without conflict.
---

## ✅ Functional Requirements
- Multi-channel dispatch (email/SMS/push)
- Templated messages
- Delivery tracking with retry
- User channel preferences + DND

## ⚙️ Non-Functional Requirements
- Failure in one channel doesn't block others
- Extensible pipeline (validation/rate-limit/logging)
- Retry with backoff, not infinite

## 🏗️ Core Classes
- `NotificationService`, `Notification`, `NotificationTemplate`
- `NotificationChannel` (interface) → `EmailChannel`, `SMSChannel`, `PushChannel`
- `NotificationHandler` chain (Chain of Responsibility)
- `DeliveryTracker` (Observer), `NotificationPreference`

## 🎨 Design Patterns Used
- **Strategy** — channel delivery
- **Chain of Responsibility** — processing pipeline
- **Observer** — delivery status tracking

## ⚠️ Edge Cases Handled
- One channel failing shouldn't fail others
- Retry storms (capped exponential backoff)
- DND window vs. critical alert override

## 🚀 Extensibility
- New channels (WhatsApp, Slack), A/B tested templates, priority queues for critical alerts.


[⬆ Back to navigation](#-navigation)

---


<a id="parking-lot"></a>
# 🅿️ Parking Lot System — LLD Interview Guide

## 📋 Problem Statement
Design a Parking Lot Management System that supports multiple floors, multiple vehicle types (bike, car, truck), ticket generation, fee calculation, and spot allocation.

---

## 🎤 Interview Flow

**Q1. Interviewer:** Let's design a parking lot. Where would you like to start?
**A:** I'd start by clarifying scope — is this a single-floor or multi-floor lot? Do we support multiple vehicle types with different spot sizes? Is payment part of the scope, and do we need real-time availability tracking?

**Q2. Interviewer:** Assume multi-floor, multiple vehicle types, and yes — payment and availability tracking are in scope.
**A:** Then my functional requirements are: park a vehicle, unpark a vehicle, find the nearest available spot for a vehicle type, generate a ticket on entry, calculate fee on exit, and track real-time spot availability per floor.

**Q3. Interviewer:** How do you model different spot sizes for different vehicles?
**A:** I'd create an abstract `ParkingSpot` base class with concrete subclasses — `SmallSpot` (bike), `MediumSpot` (car), `LargeSpot` (truck). Each `Vehicle` has a `VehicleType` enum, and spot assignment checks compatibility (a truck can't fit in a small spot, but a bike could technically go in a large one — so I'd map vehicle type to a *minimum* required spot size).

**Q4. Interviewer:** How would you efficiently find the nearest free spot on entry?
**A:** Each floor maintains its own free-spot pool, ideally as a set or min-heap keyed by spot number so we always get the lowest-numbered (nearest to entrance) free spot in O(log n). A naive linear scan across all spots works for small lots but doesn't scale — I'd mention that as an optimization point.

**Q5. Interviewer:** Walk me through what happens when a vehicle enters.
**A:** The `ParkingLot` (a Singleton, since there's only one instance managing the whole facility) receives the vehicle, calls `ParkingSpotManager.findAvailableSpot(vehicleType)`, assigns it, marks the spot occupied, and issues a `Ticket` with entry timestamp, spot ID, and vehicle ID.

**Q6. Interviewer:** How do you calculate the exit fee?
**A:** I'd use a `PricingStrategy` interface (Strategy pattern) so pricing logic — flat rate, hourly, or slab-based — can be swapped without touching the core system. On exit, the ticket's entry time is diffed against exit time, and the strategy computes the amount.

**Q7. Interviewer:** What if the lot is full?
**A:** `findAvailableSpot` returns null/throws a `NoAvailableSpotException`, and the entry gate displays "Lot Full" without issuing a ticket.

**Q8. Interviewer:** How would you extend this for reserved/VIP spots or EV charging spots?
**A:** Add new spot subtypes (`EVSpot`, `ReservedSpot`) implementing the same `ParkingSpot` interface, and extend the allocation strategy to check spot-specific eligibility before generic size compatibility. Open-closed principle keeps existing code untouched.

**Q9. Interviewer:** Why Strategy for pricing instead of a fixed formula on `ParkingLot`?
**A:** Real parking lots vary pricing by location, time of day, or vehicle type, and a fixed formula would need editing every time business rules change — risking bugs in unrelated ticketing/spot-allocation logic. Strategy isolates "how much to charge" so it can be swapped or A/B tested without touching spot management.

**Q10. Interviewer:** Why not use Observer here — where might it fit?
**A:** It fits for a real-time "spots available" display board — each `ParkingFloor` could notify a `DisplayBoard` observer whenever occupancy changes, instead of the display polling floor state on a timer. I didn't include it in the base design since the stated requirement was just tracking availability, not broadcasting it externally, but it's a natural next step.

**Q11. Interviewer:** Why Singleton for `ParkingLot` — isn't that an anti-pattern some interviewers dislike?
**A:** Fair pushback — Singleton is criticized when it's used as a lazy global-state dumping ground or makes testing hard. Here it's justified because there's a genuine real-world constraint: exactly one facility-wide coordinator should own spot allocation to avoid double-assigning the same spot from two different code paths. I'd still make it injectable/mockable (constructor-injected single instance) rather than a hard static global, so tests aren't stuck with shared state.
---

## ✅ Functional Requirements
- Park/unpark vehicles across multiple floors
- Support Bike, Car, Truck with size-matched spots
- Generate entry ticket, compute exit fee
- Real-time spot availability per floor/vehicle type

## ⚙️ Non-Functional Requirements
- O(log n) or better spot lookup
- Thread-safe spot allocation (concurrent entries)
- Extensible pricing and spot types

## 🏗️ Core Classes
- `ParkingLot` (Singleton) — orchestrates floors
- `ParkingFloor` — owns a collection of `ParkingSpot`
- `ParkingSpot` (abstract) → `SmallSpot`, `MediumSpot`, `LargeSpot`
- `Vehicle` (abstract) → `Bike`, `Car`, `Truck`
- `Ticket` — entry time, spot ref, vehicle ref
- `PricingStrategy` (interface) → `HourlyPricing`, `FlatPricing`

## 🎨 Design Patterns Used
- **Singleton** — single `ParkingLot` instance
- **Strategy** — pluggable pricing
- **Factory** — vehicle/spot creation

## ⚠️ Edge Cases Handled
- Lot full on entry
- Invalid ticket on exit
- Vehicle type / spot size mismatch

## 🚀 Extensibility
- EV charging spots, reserved/VIP spots, dynamic surge pricing, multiple entry/exit gates with load balancing.


[⬆ Back to navigation](#-navigation)

---


<a id="rate-limiter"></a>
# 🚦 Rate Limiter — LLD Interview Guide

## 📋 Problem Statement
Design a rate limiter that restricts the number of requests a client can make within a time window, supporting pluggable algorithms.

---

## 🎤 Interview Flow

**Q1. Interviewer:** What clarifying questions would you ask?
**A:** Per-client or global limiting? What granularity — per API key, per IP, per user? Is this in-process or does it need to work across multiple servers? What should happen when the limit is hit — reject or queue? I'll assume per-client, in-process, reject-on-exceed for the core design, and mention distributed extension after.

**Q2. Interviewer:** What algorithms do you know, and which would you pick?
**A:** Fixed Window Counter, Sliding Window Log, Sliding Window Counter, Token Bucket, and Leaky Bucket. I'd design the system around a `RateLimitingStrategy` interface so any of these can be swapped in — for most APIs I'd default to **Token Bucket** since it allows bursts while enforcing an average rate, which fits typical API usage patterns better than a hard fixed window.

**Q3. Interviewer:** Explain how Token Bucket works and why Fixed Window has a flaw.
**A:** Fixed Window resets the counter at a clock boundary, so a client can send the full quota right at the end of one window and again right at the start of the next — effectively 2x the limit in a short burst. Token Bucket instead has a bucket refilling at a constant rate up to a max capacity; each request consumes a token, and if none are available, the request is rejected — bursts are allowed only up to the bucket capacity, and the average rate is still bounded by the refill rate.

**Q4. Interviewer:** How do you implement the Token Bucket without a background thread constantly refilling it?
**A:** Lazy refill — store `lastRefillTimestamp` and `currentTokens` per client. On each request, compute `elapsedTime * refillRate` tokens to add (capped at bucket capacity) before checking/consuming, rather than running a timer. This avoids wasted CPU on idle clients.

**Q5. Interviewer:** How do you key rate limits per client efficiently?
**A:** A `ConcurrentHashMap<clientId, TokenBucket>` inside the `RateLimiter`, with thread-safe read-modify-write per bucket (synchronized block or atomic compare-and-swap on the token count) so concurrent requests from the same client don't race past the limit.

**Q6. Interviewer:** How would this scale to a distributed system with multiple API gateway instances?
**A:** In-memory maps don't work across nodes — I'd move bucket state to a shared store like Redis, using `INCR` + `EXPIRE` for fixed window, or a Lua script for atomic token-bucket check-and-decrement to avoid race conditions across nodes.

**Q7. Interviewer:** What response do you give the client when rate-limited?
**A:** HTTP 429 Too Many Requests, plus a `Retry-After` header computed from how long until the next token is available — good practice so clients can back off intelligently instead of hammering the API.

**Q8. Interviewer:** Why Strategy for the algorithm instead of hardcoding Token Bucket everywhere?
**A:** Different endpoints often need different limiting behavior — a login endpoint might want strict Fixed Window to prevent brute force, while a general API might prefer Token Bucket for burst tolerance. Strategy lets `RateLimiter` stay agnostic to the algorithm and lets each client/endpoint be configured independently.

**Q9. Interviewer:** Did you consider Decorator — wrapping an existing API handler with rate-limiting behavior?
**A:** Yes, that's actually how it's often deployed in practice — a `RateLimitedHandlerDecorator` wraps the real request handler, checking the limiter before delegating. I described the limiter itself as a standalone component reusable from either a decorator or a filter/middleware layer, since the pattern used to *invoke* it (decorator vs. middleware) is a separate concern from the limiting algorithm itself.

**Q10. Interviewer:** Why not Singleton for the RateLimiter, given ATM/ParkingLot used it?
**A:** Here I'd scope it differently — a Singleton *per limiting scope* (e.g., one instance per API gateway process) makes sense, but I wouldn't hardcode it as a single global static Singleton, since that fights against per-endpoint or per-tenant configuration. I'd inject a `RateLimiter` instance where needed instead of reaching for a static global, so different services/tests can each hold independent limiter state.
---

## ✅ Functional Requirements
- Per-client request limiting within a rate
- Pluggable algorithm (token bucket default)
- Clear reject response with retry guidance

## ⚙️ Non-Functional Requirements
- Thread-safe under concurrent requests
- Low overhead (no busy-wait threads)
- Extensible to distributed deployment

## 🏗️ Core Classes
- `RateLimiter` — orchestrator, holds per-client state
- `RateLimitingStrategy` (interface) → `TokenBucketStrategy`, `FixedWindowStrategy`, `SlidingWindowStrategy`
- `TokenBucket` — capacity, refill rate, current tokens, last refill time

## 🎨 Design Patterns Used
- **Strategy** — swappable limiting algorithm
- **Singleton** — shared limiter instance per service

## ⚠️ Edge Cases Handled
- Burst traffic at window boundaries (fixed window flaw avoided via token bucket)
- Concurrent requests from same client (race-safe token consumption)
- Idle clients (lazy refill, no wasted computation)

## 🚀 Extensibility
- Distributed limiting via Redis, per-endpoint differentiated limits, tiered limits by subscription plan.


[⬆ Back to navigation](#-navigation)

---


<a id="ride-sharing-system"></a>
# 🚕 Ride Sharing System — LLD Interview Guide

## 📋 Problem Statement
Design a ride-sharing platform (like Uber/Ola) handling rider requests, driver matching, ride lifecycle, and fare calculation.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you scope a ride-sharing system for an LLD round?
**A:** I'd keep it to core object interactions rather than distributed infra: rider requests a ride, system matches the nearest available driver, ride goes through states (requested → accepted → ongoing → completed), and fare is computed at the end. I'd mention that real-world geo-matching (quad-trees, geohashing) is a system-design-level concern, but I can sketch a simplified nearest-driver strategy for LLD purposes.

**Q2. Interviewer:** How do you find the nearest available driver?
**A:** A `DriverMatchingStrategy` interface — for LLD scope, a simple implementation scans available drivers within a radius and picks the closest by Euclidean/haversine distance. I'd note this as a pluggable strategy so it can later be swapped for a geospatial index without touching the rest of the system.

**Q3. Interviewer:** What states does a `Ride` go through, and how do you model transitions?
**A:** `REQUESTED → ACCEPTED → ARRIVED → IN_PROGRESS → COMPLETED` (or `CANCELLED` from most states). I'd use the **State pattern** so invalid transitions — e.g., completing a ride that was never accepted — are structurally impossible.

**Q4. Interviewer:** How does the driver get notified of a request?
**A:** **Observer pattern** — drivers subscribe as observers to a `RideDispatcher`; when a match is found, the dispatcher notifies that specific driver's observer with the ride request, who then accepts or rejects within a timeout.

**Q5. Interviewer:** What happens if the driver doesn't respond in time or rejects?
**A:** The dispatcher falls back to the next-nearest available driver from the matching strategy, with a max retry count before informing the rider no drivers are available.

**Q6. Interviewer:** How do you calculate the fare?
**A:** A `FareCalculator` (Strategy pattern) using base fare + distance-based rate + time-based rate, with optional surge multiplier based on demand/supply ratio in the area — kept pluggable so surge logic can evolve independently.

**Q7. Interviewer:** How would you handle ride cancellation and its cost implications?
**A:** Cancellation is allowed from `REQUESTED`/`ACCEPTED` states only. If cancelled after the driver has already started moving toward pickup (post-`ACCEPTED`), a small cancellation fee strategy kicks in; this is again a pluggable `CancellationPolicy`.

**Q8. Interviewer:** Why State for `Ride` instead of Command for the ride lifecycle?
**A:** Command models discrete *actions* as objects (useful for undo/queueing/logging an action), while what we actually need here is to restrict which actions are *valid* depending on where the ride currently is in its lifecycle — that's State's job. I do use something Command-like implicitly (`accept()`, `cancel()` as explicit operations), but the *gatekeeping* of which operations are even callable is State's responsibility.

**Q9. Interviewer:** Why Observer for driver notification instead of the dispatcher directly calling a method on the driver object?
**A:** A direct call tightly couples the dispatcher to a specific `Driver` interface and makes it hard to have multiple things react to a new ride request (e.g., logging, analytics, the driver's app UI) without the dispatcher knowing about all of them. Observer means new listeners can subscribe without the dispatcher's code changing at all.

**Q10. Interviewer:** Why did you reject a simpler design where fare calculation lives directly inside `Ride.complete()`?
**A:** Because fare rules (surge multipliers, promo codes, vehicle-tier rates) change far more often than the ride lifecycle itself does, and embedding them in `Ride` would mean every pricing tweak risks destabilizing lifecycle logic that has nothing to do with pricing. Separating `FareCalculator` as its own Strategy keeps those concerns from bleeding into each other.
---

## ✅ Functional Requirements
- Request ride, match nearest driver
- Ride lifecycle state management
- Fare calculation with surge pricing
- Cancellation handling

## ⚙️ Non-Functional Requirements
- Extensible matching/fare/cancellation strategies
- Consistent state transitions (no illegal jumps)
- Reasonably responsive matching

## 🏗️ Core Classes
- `Rider`, `Driver`, `Ride` (context) with `RideState`
- `RideState` (interface) → `Requested`, `Accepted`, `InProgress`, `Completed`, `Cancelled`
- `DriverMatchingStrategy`, `FareCalculator`, `CancellationPolicy` (Strategy interfaces)
- `RideDispatcher` (Observer subject)

## 🎨 Design Patterns Used
- **State** — ride lifecycle
- **Strategy** — matching, fare, cancellation policy
- **Observer** — driver notification

## ⚠️ Edge Cases Handled
- No drivers available
- Driver timeout/rejection fallback
- Mid-ride cancellation fee

## 🚀 Extensibility
- Ride pooling/shared rides, multiple vehicle tiers, scheduled rides.


[⬆ Back to navigation](#-navigation)

---


<a id="shopping-cart"></a>
# 🛒 Shopping Cart (E-commerce) — LLD Interview Guide

## 📋 Problem Statement
Design a shopping cart and checkout system supporting product catalog, cart operations, discounts/coupons, and order placement.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you begin modeling this?
**A:** Core entities: `Product` (catalog), `Cart` (per user, holds `CartItem`s), `Order` (finalized checkout), and pricing/discount logic. I'd keep pricing computation separate from cart state so promotions can evolve independently.

**Q2. Interviewer:** How do you handle adding/removing/updating quantities in the cart?
**A:** `Cart` maintains a `Map<productId, CartItem>` where `CartItem` holds quantity and a price snapshot. `addItem` increments quantity if already present, `updateQuantity` validates against available stock, and `removeItem` deletes the entry — all backed by checking `Product.stock` before confirming.

**Q3. Interviewer:** How would you design a flexible discount/coupon system?
**A:** A `DiscountStrategy` interface — `PercentageDiscount`, `FlatDiscount`, `BuyOneGetOneDiscount` — and I'd support **stacking multiple discounts** via a `CompositeDiscount` that applies each strategy in sequence (or picks the best single one, depending on business rules), so adding a new promo type doesn't touch checkout logic.

**Q4. Interviewer:** How do you validate a coupon code?
**A:** `CouponService.validate(code, cart)` checks: code exists and isn't expired, minimum cart value is met, usage limit per user isn't exceeded, and it applies to eligible categories/products if restricted — returning a clear reason on rejection rather than a generic failure.

**Q5. Interviewer:** Walk me through checkout.
**A:** `Cart.checkout()` re-validates stock for every item (in case something sold out since it was added), locks/reserves inventory, computes final price via `PriceCalculator` (subtotal → discounts → tax → shipping), creates an `Order` in `PENDING_PAYMENT` state, and hands off to a `PaymentService`. On payment success, order moves to `CONFIRMED` and inventory reservation is committed (decremented); on failure, it's released.

**Q6. Interviewer:** How do you prevent overselling when two users buy the last unit simultaneously?
**A:** Inventory reservation at checkout time uses an atomic decrement-if-available operation (e.g., `UPDATE stock SET qty = qty - 1 WHERE qty > 0`), so only one of the two concurrent checkouts succeeds; the other is told the item just sold out.

**Q7. Interviewer:** How would you support wishlists or "save for later"?
**A:** A separate `Wishlist` per user, structurally similar to `Cart` but without checkout capability — items can move between `Cart` and `Wishlist` via simple transfer methods, avoiding duplicating item-management logic by extracting a shared `ItemCollection` base behavior.

**Q8. Interviewer:** Why Composite for stacking discounts instead of just looping through a list of discounts in checkout code?
**A:** A raw loop in checkout works initially but means checkout logic owns discount-combination rules (e.g., "percentage discounts apply before flat ones," "only one coupon at a time unless stackable flag set"). Wrapping that combination logic inside `CompositeDiscount` means checkout just calls `discount.apply(cart)` once, and the composition/ordering rules live with the discounts themselves, not scattered in checkout.

**Q9. Interviewer:** Why State for `Order` but Strategy (not State) for discounts — how did you decide which pattern applies where?
**A:** The distinguishing question I ask myself: does the object move through a lifecycle where valid operations change over time (→ State), or am I choosing between interchangeable algorithms for a single computation (→ Strategy)? An order's status naturally progresses and restricts actions — that's State. A discount is just "one way among several to compute a reduction" — that's Strategy, since there's no lifecycle involved.

**Q10. Interviewer:** Did you consider Decorator for wishlist-to-cart item transfer, or Observer for stock-change alerts?
**A:** Observer, yes — a `StockWatcher` could notify users who wishlisted an out-of-stock item once it's back, without the inventory system needing to know about wishlists directly. Decorator less so here — item transfer between `Cart` and `Wishlist` is really just moving data between two similar collections, not adding new behavior to an existing object, so a shared base behavior (as I mentioned) is a better fit than Decorator.
---

## ✅ Functional Requirements
- Browse catalog, manage cart (add/remove/update qty)
- Apply discounts/coupons (stackable)
- Checkout with stock validation and payment
- Order creation and status tracking

## ⚙️ Non-Functional Requirements
- No overselling under concurrent checkout
- Price snapshot consistency
- Extensible discount/tax/shipping strategies

## 🏗️ Core Classes
- `Product`, `Cart`, `CartItem`, `Wishlist`
- `DiscountStrategy` (interface) → `PercentageDiscount`, `FlatDiscount`, `CompositeDiscount`
- `CouponService`, `PriceCalculator`
- `Order` (context) with `OrderState`, `PaymentService`

## 🎨 Design Patterns Used
- **Strategy** — discount computation
- **Composite** — stacking multiple discounts
- **State** — order lifecycle

## ⚠️ Edge Cases Handled
- Item sells out between add-to-cart and checkout
- Coupon expiry / usage-limit / min-cart-value violations
- Concurrent last-unit purchase race

## 🚀 Extensibility
- Multi-vendor marketplace support, subscription/recurring orders, dynamic tax by region.


[⬆ Back to navigation](#-navigation)

---


<a id="snake-and-ladder"></a>
# 🐍 Snake and Ladder — LLD Interview Guide

## 📋 Problem Statement
Design the classic Snake and Ladder board game supporting configurable board size, snakes/ladders placement, dice rolling, and multiple players.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How do you model snakes and ladders on the board?
**A:** I'd unify both concepts into a single abstraction — a `Jump` with a `start` and `end` cell. A `Snake` is a `Jump` where `end < start`; a `Ladder` is a `Jump` where `end > start`. The `Board` just holds `Map<cellNumber, Jump>` — after landing on a cell, it checks if a jump exists there and, if so, moves the player to `jump.end`. This avoids two near-duplicate classes and duplicate logic.

**Q2. Interviewer:** How do you validate that snake/ladder placements are legal (e.g., no cell has both, no chained jumps)?
**A:** At board setup: each cell can have at most one jump (start positions must be unique), a jump's start/end must be within board bounds and not on cell 1 or the final cell, and I'd explicitly disallow a jump's `end` landing on another jump's `start` to prevent ambiguous chained resolution — validated once at construction time via a `BoardValidator`.

**Q3. Interviewer:** How do you handle the dice and turn order?
**A:** `Dice` is its own class (rollable, configurable number of faces — supporting variants with 2 dice), and `Game` maintains a queue/list of `Player`s, cycling turns. Each turn: roll dice → compute `newPosition = current + roll` → if `newPosition > lastCell`, the move is invalid in the classic "exact landing" rule variant (stay in place) or simply capped, depending on house rules — I'd make this configurable via a `MovementRule` strategy.

**Q4. Interviewer:** How do you check for a win?
**A:** After resolving position (including any jump), if `position == lastCell`, that player wins and the game ends — I'd check this after jump resolution, since landing exactly on the last cell via a ladder should still count as winning.

**Q5. Interviewer:** What if two players land on the same cell — does that matter here (unlike some board games)?
**A:** Classic Snake and Ladder doesn't have "bumping" like Ludo, so I'd keep it simple — multiple players can coexist on the same cell with no interaction. I'd mention this as a design assumption to confirm with the interviewer, since some game variants do add player-collision rules.

**Q6. Interviewer:** How would you make this support different board sizes or custom rule sets (e.g., "roll a 6 to start")?
**A:** `Board` takes size as a constructor parameter rather than hardcoding 100, and player-turn logic is wrapped in a `TurnRule` strategy (e.g., `RequireSixToStartRule`, `ExtraTurnOnSixRule`) so house rules plug in without touching the core game loop.

**Q7. Interviewer:** How do you structure the main game loop?
**A:** `Game.play()` loops: get current player → roll dice → `Board.movePlayer(player, roll)` (which internally applies jumps) → check win condition → advance turn — kept in `Game`, while `Board` only knows about cells and jumps, and `Player` only knows its own position. Clean single-responsibility split.

**Q8. Interviewer:** Why unify Snake and Ladder into one `Jump` class instead of two separate classes?
**A:** Structurally they're identical — both map a start cell to an end cell and get resolved the same way after landing on a cell; the only difference is the *direction* of the jump (down vs. up), which is derivable from comparing start and end rather than needing separate types. Two classes would just duplicate the same lookup/resolution logic with no behavioral difference, violating DRY for no benefit.

**Q9. Interviewer:** Why Strategy for movement/turn rules instead of hardcoding "roll a 6 to start" as a special case in `Game`?
**A:** House rules vary a lot between implementations/regions, and hardcoding one variant into `Game`'s core loop means every new rule variant requires editing that shared loop and risks breaking existing behavior. Strategy isolates each rule as a swappable component so `Game` stays a stable orchestrator regardless of which house rules are active.

**Q10. Interviewer:** Did you consider Observer for updating a UI as players move?
**A:** Yes — `Game` firing a `onPlayerMoved(player, newPosition)` event that a UI/board-renderer subscribes to, rather than the UI polling player positions after every turn. Same reasoning as the Music Player and Elevator cases — anywhere a UI needs to reflect internal state changes without polling.
---

## ✅ Functional Requirements
- Configurable board size
- Snake/ladder placement with validation
- Turn-based dice rolling, multiple players
- Win detection

## ⚙️ Non-Functional Requirements
- No ambiguous/chained jump configurations
- Configurable movement/turn rule variants

## 🏗️ Core Classes
- `Board`, `Cell`, `Jump` (base for Snake/Ladder)
- `Dice`, `Player`
- `Game` — turn loop, win check
- `MovementRule` / `TurnRule` (Strategy interfaces)

## 🎨 Design Patterns Used
- **Strategy** — movement/turn rule variants
- **Unified abstraction** — Snake & Ladder modeled as one `Jump` type

## ⚠️ Edge Cases Handled
- Overshoot beyond last cell (rule-dependent handling)
- Landing exactly on last cell via a ladder (still a win)
- Invalid/chained jump configuration at setup

## 🚀 Extensibility
- Multiple dice, "roll six to start" and "extra turn on six" house rules, animated multiplayer online play.


[⬆ Back to navigation](#-navigation)

---


<a id="splitwise"></a>
# 💸 Splitwise — LLD Interview Guide

## 📋 Problem Statement
Design an expense-sharing app (like Splitwise) supporting groups, equal/exact/percentage splits, and simplified debt settlement between users.

---

## 🎤 Interview Flow

**Q1. Interviewer:** Where do you start with Splitwise?
**A:** Clarifying split types first — equal, exact amounts, or percentage-based — plus whether we need group-level expense tracking and a "simplify debts" feature that minimizes the number of settlement transactions. I'll assume all of these are in scope.

**Q2. Interviewer:** How do you model a split?
**A:** An abstract `Split` class with `user` and `amount`, and I use the **Strategy pattern** for split *computation*: `EqualSplitStrategy`, `ExactSplitStrategy`, `PercentSplitStrategy` — each takes an `Expense` and a list of participants and returns computed `Split` objects. This keeps `Expense` creation logic decoupled from how the split math is done.

**Q3. Interviewer:** How do you track who owes whom?
**A:** A `Balance` map, effectively a graph where each edge `(userA -> userB)` holds a signed amount. Every new expense updates the relevant edges: the payer's balance with each participant increases by their share.

**Q4. Interviewer:** That graph could get messy with many users owing many others. How do you simplify it?
**A:** This is the classic "simplify debts" problem — treat it as balancing a graph so the *total number of transactions* is minimized. Compute each user's net balance (total owed minus total owing), then greedily match the largest creditor with the largest debtor repeatedly (a min-heap/max-heap pair), settling the smaller of the two amounts each time. This is a known variant of the "optimal account balancing" problem.

**Q5. Interviewer:** How do you ensure split amounts add up exactly, especially with rounding?
**A:** For equal splits with a non-divisible total (e.g., ₹100 among 3 people), I distribute the remainder paise to the first few participants deterministically so the sum always equals the original amount exactly — avoiding floating-point drift.

**Q6. Interviewer:** How would you validate a percentage split?
**A:** The strategy sums all provided percentages and rejects the expense (throws `InvalidSplitException`) if they don't total 100%, before any balance updates happen — so partial/inconsistent state is never persisted.

**Q7. Interviewer:** How do you handle a user leaving a group with outstanding balances?
**A:** I wouldn't allow removal while `balance != 0` with any group member — the system would prompt for settlement first, similar to how the real app enforces this.

**Q8. Interviewer:** Why Strategy for splits instead of one `Expense.calculateSplits()` method with an if-else on split type?
**A:** Split calculation logic (equal/exact/percentage) is genuinely different math with different validation rules — cramming it into one method with branching makes the method long and each split type's tests harder to isolate. Strategy gives each split type its own small, independently testable class, and adding a new type (e.g., "shares-based" splitting, like 2 shares vs 1 share) doesn't touch existing strategies.

**Q9. Interviewer:** Why not model the balance sheet as a graph with a dedicated Graph class/library instead of a simple map?
**A:** For the scope of typical group sizes in this kind of app, a `Map<Pair<userA,userB>, amount>` is simple and sufficient — a full graph library adds complexity without clear benefit here since we don't need general graph algorithms, just net-balance computation and greedy settlement. I'd reconsider if group sizes or the need for more complex graph queries (e.g., "shortest debt path") grew significantly.

**Q10. Interviewer:** Where would Observer fit here, if anywhere?
**A:** Notifying group members when a new expense is added or a balance is settled — `Group` could act as a subject that members subscribe to, so notification/UI-refresh logic doesn't get baked into `ExpenseManager`'s core balance-update code.
---

## ✅ Functional Requirements
- Create groups, add expenses with equal/exact/percent splits
- Track pairwise balances
- Simplify debts (minimize transactions)
- Settle up between users

## ⚙️ Non-Functional Requirements
- Exact decimal correctness (no rounding drift)
- Scalable balance computation for large groups
- Extensible split types

## 🏗️ Core Classes
- `User`, `Group`, `Expense`
- `Split` (abstract) → `EqualSplit`, `ExactSplit`, `PercentSplit`
- `SplitStrategy` (interface) → corresponding strategies
- `BalanceSheet` — pairwise ledger
- `DebtSimplifier` — settlement optimizer

## 🎨 Design Patterns Used
- **Strategy** — split computation
- **Observer** — notify group members on new expense
- **Singleton** — central `ExpenseManager`

## ⚠️ Edge Cases Handled
- Percentages not summing to 100%
- Rounding remainder distribution
- Removing a user with non-zero balance

## 🚀 Extensibility
- Multi-currency support, recurring expenses, itemized/receipt-based splitting.


[⬆ Back to navigation](#-navigation)

---


<a id="url-shortener"></a>
# 🔗 URL Shortener — LLD Interview Guide

## 📋 Problem Statement
Design a URL shortening service (like bit.ly) that maps long URLs to short codes, handles redirection, and supports expiry/analytics.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How would you generate the short code?
**A:** A few options: hash the long URL (MD5/SHA and truncate), use a counter-based approach with base62 encoding, or generate a random string and check for collisions. I'd go with **counter-based + base62 encoding** — it guarantees uniqueness without collision checks and produces short, predictable-length codes.

**Q2. Interviewer:** Explain base62 encoding and why it's used here.
**A:** Base62 uses `[a-zA-Z0-9]` (62 characters) instead of base10, so a given number range maps to a much shorter string — e.g., a 7-character base62 code can represent up to 62^7 (~3.5 trillion) unique URLs, versus needing far more digits in base10. I convert an auto-incrementing global counter (or a distributed ID generator) into base62 to form the short code.

**Q3. Interviewer:** A single incrementing counter is a bottleneck at scale — how would you address that?
**A:** I'd use a distributed unique ID generator — e.g., pre-allocate ID ranges to each server (range-based sharding of the counter), or use something like Snowflake IDs, then base62-encode the result. Each server can then generate short codes independently without contention.

**Q4. Interviewer:** How do you handle custom aliases (user-specified short codes)?
**A:** A separate check-and-reserve path: `createCustomAlias(longUrl, alias)` checks the alias isn't already taken in the mapping store, and if free, reserves it atomically (e.g., `putIfAbsent`) — falling back to an error if taken, rather than silently overwriting.

**Q5. Interviewer:** How does redirection work, and how do you make it fast?
**A:** `GET /{shortCode}` looks up the mapping and issues an HTTP 302 redirect to the original URL. Since redirection is read-heavy, I'd cache hot mappings (LRU cache in front of the persistent store) — most requests hit cache, only cold entries hit the DB.

**Q6. Interviewer:** How do you support link expiry?
**A:** Each `UrlMapping` stores an optional `expiresAt`. On lookup, if the entry is expired, return a "link expired" response and mark it inactive; a background cleanup job can periodically purge expired entries from storage to reclaim space.

**Q7. Interviewer:** How would you add click analytics without slowing down the redirect path?
**A:** Don't do analytics synchronously in the redirect request — fire an async event (e.g., to a queue) with click metadata (timestamp, referrer, rough geo/IP) that's processed by a separate consumer, so the user-facing redirect stays fast regardless of analytics load.

**Q8. Interviewer:** Why Strategy for ID generation instead of hardcoding the counter-based approach?
**A:** Different deployment scales need different generation approaches — a single small service can use a simple counter, but a distributed system might need Snowflake IDs or pre-allocated ranges. Strategy means the rest of the system (base62 encoding, mapping storage) doesn't care which generation approach is active, so scaling up doesn't require redesigning the whole service.

**Q9. Interviewer:** Why Facade for `UrlShortenerService` — what would it look like without one?
**A:** Without a Facade, callers (API controllers) would need to directly orchestrate ID generation, base62 encoding, cache lookup, and repository calls themselves — duplicating that orchestration wherever shortening or resolving happens. The Facade bundles that sequence behind two simple calls (`shorten()`, `resolve()`), hiding internal wiring changes from callers entirely.

**Q10. Interviewer:** Did you consider Proxy pattern for the caching layer?
**A:** It's a reasonable framing — a `CachingUrlRepositoryProxy` implementing the same interface as the real repository, transparently checking cache before hitting the DB. I described it more simply as "cache fronting the repository," which is functionally the same idea; I'd formalize it as an explicit Proxy if the interviewer wanted to see the pattern named and structured that way.
---

## ✅ Functional Requirements
- Shorten long URL → unique short code
- Custom alias support
- Redirect short → long URL
- Optional expiry + click analytics

## ⚙️ Non-Functional Requirements
- Fast, low-latency redirects (cache-first)
- No collisions on generated codes
- Horizontally scalable ID generation

## 🏗️ Core Classes
- `UrlShortenerService` — orchestrator
- `UrlMapping` (shortCode, longUrl, createdAt, expiresAt)
- `Base62Encoder`, `IdGenerator` (Strategy — swappable: counter-based, Snowflake, hash-based)
- `Cache` (LRU) fronting persistent `UrlRepository`

## 🎨 Design Patterns Used
- **Strategy** — ID/code generation algorithm
- **Facade** — `UrlShortenerService` hides encoding + storage + cache details
- **Singleton** — shared ID generator per instance

## ⚠️ Edge Cases Handled
- Custom alias collision
- Expired link access
- Malformed/duplicate long URL submission (optionally return existing short code)

## 🚀 Extensibility
- Distributed ID generation (Snowflake), rate limiting per user/IP, QR code generation for short links.


[⬆ Back to navigation](#-navigation)

---


<a id="vending-machine"></a>
# 🥤 Vending Machine — LLD Interview Guide

## 📋 Problem Statement
Design a vending machine that accepts coins/notes, lets a user select a product, dispenses it, and returns change — modeled cleanly with states.

---

## 🎤 Interview Flow

**Q1. Interviewer:** How would you model the vending machine's behavior?
**A:** A vending machine is a textbook **State pattern** use case. It moves through: `IdleState` → `HasMoneyState` (after money inserted) → `ProductSelectedState` → `DispensingState` → back to `IdleState`.

**Q2. Interviewer:** What happens if a user inserts money but never selects a product?
**A:** I'd add a "return coins" action available from `HasMoneyState`, and optionally a timeout that auto-triggers refund after N seconds of inactivity — handled by the state itself exposing a `cancel()` method.

**Q3. Interviewer:** How do you track inventory?
**A:** An `Inventory` map of `productCode -> (Product, quantityAvailable, slotNumber)`. Before allowing selection, the machine checks quantity > 0, and after dispensing, decrements it.

**Q4. Interviewer:** How do you validate whether inserted money covers the product price?
**A:** The machine accumulates inserted denominations in a running total. On product selection, it compares total against price — if insufficient, it prompts for more money; if sufficient, it proceeds to dispense and computes change as `total - price`.

**Q5. Interviewer:** How do you compute change with limited coin/note denominations?
**A:** Same greedy denomination-breakdown approach as the ATM — sort available denominations descending, and check the machine actually has enough of each. If exact change isn't possible, the transaction is cancelled and the full amount is refunded instead of shortchanging the customer.

**Q6. Interviewer:** What if the selected slot is out of stock?
**A:** Selection is rejected up front (before entering `ProductSelectedState`) with a "Sold Out" message; money already inserted stays available for another selection or refund.

**Q7. Interviewer:** How would you extend this for card payments alongside cash?
**A:** Introduce a `PaymentStrategy` interface (Strategy pattern) with `CashPayment` and `CardPayment` implementations. The state machine doesn't care *how* payment is validated — it just asks the strategy "is payment complete?"

**Q8. Interviewer:** Why State here instead of a simpler "hasEnoughMoney" boolean check before each action?
**A:** A boolean check scattered before every method call (dispense, refund, select) duplicates the same guard logic repeatedly and is easy to forget in one place, causing bugs like dispensing before payment. State pattern centralizes "what's allowed right now" inside the current state object, so you literally cannot call an invalid operation because the current state doesn't expose it.

**Q9. Interviewer:** Why Strategy for payment method rather than an `isCard` boolean flag on the machine?
**A:** A boolean flag forces if-else branching wherever payment logic is checked, and doesn't scale past two payment types. `PaymentStrategy` means adding UPI or wallet payments later is just a new implementation of the interface — zero changes to the state machine or existing payment types.

**Q10. Interviewer:** Would Observer fit anywhere in a vending machine design?
**A:** Yes — notifying a remote inventory-monitoring system when stock runs low, without the core dispensing logic needing to know about remote monitoring APIs. It's a common real-world feature (smart vending machines report low stock automatically) that layers cleanly on top of the existing `Inventory` class via a subscribed listener.
---

## ✅ Functional Requirements
- Accept coins/notes, track balance
- Product selection tied to inventory
- Dispense product + correct change
- Refund/cancel support

## ⚙️ Non-Functional Requirements
- No shortchange (exact-change guarantee or full refund)
- Consistent inventory even under rapid selections
- Extensible payment methods

## 🏗️ Core Classes
- `VendingMachine` (context) with `VendingState`
- `VendingState` (interface) → `IdleState`, `HasMoneyState`, `ProductSelectedState`, `DispensingState`
- `Inventory`, `Product`, `Slot`
- `PaymentStrategy` (interface) → `CashPayment`, `CardPayment`

## 🎨 Design Patterns Used
- **State** — machine lifecycle
- **Strategy** — payment method
- **Singleton** — single machine instance managing shared inventory

## ⚠️ Edge Cases Handled
- Insufficient money
- Out-of-stock product
- No exact change available (auto full refund)

## 🚀 Extensibility
- Multi-machine remote inventory monitoring, loyalty/coupon codes, temperature-sensitive product handling.


[⬆ Back to navigation](#-navigation)

---


<a id="tic-tac-toe"></a>
# ⭕ Tic Tac Toe — LLD Interview Guide

## 📋 Problem Statement
Design a Tic Tac Toe game supporting an NxN board, two players, win/draw detection, and clean extensibility.

---

## 🎤 Interview Flow

**Q1. Interviewer:** This seems simple — what would make a strong LLD answer here?
**A:** Even for a simple game, the interviewer usually wants to see clean OOP separation and efficient win-detection rather than a brute-force 3x3-only solution. I'd design for an **NxN board** from the start so the code isn't hardcoded to the classic 3x3 case.

**Q2. Interviewer:** How do you check for a win efficiently, especially for large N?
**A:** A naive approach re-scans all rows/columns/diagonals after every move — O(N) per check. Instead, I maintain running counters per player: an array of row counts, column counts, and two diagonal counters. After each move at `(row, col)`, I increment the relevant counters for that player and decrement the opponent's (or just track per-player), and if any counter hits N, that player wins — this makes each move check O(1).

**Q3. Interviewer:** How do you structure the classes?
**A:** `Board` holds the grid and win-check counters. `Player` has a symbol and type (`Human`/`Computer` — using **Strategy** for move-selection so an AI player can plug in later). `Game` orchestrates turns, calls `Board.placeMark()`, checks win/draw after each move, and manages `GameState`.

**Q4. Interviewer:** How do you detect a draw?
**A:** Track a `movesPlayed` counter; if it reaches `N*N` without any win condition being met, the game is a draw. I check this only after confirming no win, so a winning final move on the last cell is correctly reported as a win, not a draw.

**Q5. Interviewer:** How would you validate a move?
**A:** `Board.placeMark(row, col, player)` checks bounds (`0 <= row,col < N`), that the cell is empty, and that it's the correct player's turn (enforced by `Game`, not `Board`, keeping single responsibility) — invalid moves throw/return a clear error rather than silently failing.

**Q6. Interviewer:** How would you extend this to support an AI opponent?
**A:** Since `Player` already delegates move selection via a `MoveStrategy` interface, I'd add a `MinimaxStrategy` implementing optimal play (or a simpler heuristic for larger boards where minimax becomes too expensive) — the `Game` loop itself doesn't change at all.

**Q7. Interviewer:** What about supporting more than 2 players or a "connect K in a row on NxN" variant (like Gomoku)?
**A:** For >2 players, the win-counter approach needs a small tweak — track counts per player per line instead of a single shared counter, resetting a line's eligibility once any two different players have marked it. For "K in a row" instead of full N, I'd switch the win check to a sliding-window count along each direction from the last-placed cell rather than the O(1) full-line counters, since a win no longer requires filling the whole line.

**Q8. Interviewer:** Why Strategy for move selection instead of a boolean `isAI` flag with if-else in `Game`?
**A:** A flag-based approach means `Game`'s turn loop needs to know about AI-specific logic (minimax calls, difficulty levels) directly, mixing orchestration with move-selection intelligence. `MoveStrategy` keeps `Game` blind to *how* a move is chosen — human input vs. minimax vs. a future heuristic AI are all just "ask the strategy for a move," so `Game` never changes when AI logic evolves.

**Q9. Interviewer:** Why not State pattern here, given how many other designs used it?
**A:** State fits when *behavior* changes across a lifecycle with different valid operations per stage. Tic Tac Toe's lifecycle is simple enough (`InProgress → Won/Draw`) that a plain enum checked once per turn is clear and sufficient — introducing full State classes for two terminal outcomes would be over-engineering relative to the complexity it's solving.

**Q10. Interviewer:** How would you unit test the O(1) win-detection logic specifically?
**A:** I'd test the counters directly rather than only end-to-end game play: place a controlled sequence of moves that should *not* trigger a win and assert no win is reported, then one that completes a row/column/diagonal and assert a win is reported exactly on that move — including a boundary case where the last empty cell on the board is also the winning move, to make sure draw-checking doesn't shadow the win check.
---

## ✅ Functional Requirements
- NxN board support
- Turn-based play, win/draw detection
- Extensible player types (human/AI)

## ⚙️ Non-Functional Requirements
- O(1) win-check per move (not full-board rescan)
- Clean separation of board state, player, and game orchestration

## 🏗️ Core Classes
- `Board` — grid + row/col/diagonal counters
- `Player` — symbol, `MoveStrategy`
- `MoveStrategy` (interface) → `HumanMoveStrategy`, `MinimaxStrategy`
- `Game` — turn orchestration, `GameState`

## 🎨 Design Patterns Used
- **Strategy** — move selection (human vs AI)
- **State** — game state (InProgress/Won/Draw)

## ⚠️ Edge Cases Handled
- Full board with no winner (draw)
- Win detected exactly on the final move
- Out-of-bounds / already-occupied cell moves

## 🚀 Extensibility
- Gomoku-style "K in a row" variant, more than 2 players, online multiplayer.


[⬆ Back to navigation](#-navigation)

---