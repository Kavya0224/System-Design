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
