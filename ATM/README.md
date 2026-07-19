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
