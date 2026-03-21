# Snake and Ladder - Low Level Design

## 📌 Problem Statement

Design a Snake and Ladder game with:

* Multiple players
* Dice-based movement
* Snakes and ladders
* Winning condition at last cell

---

## 🧠 Design Overview

The system simulates a turn-based board game with clear separation between game logic and board rules.

---

## 🧩 Core Classes

### 1. Game

* Controls game flow
* Manages player turns
* Determines winner

### 2. Player

* Stores player ID and position

### 3. Board

* Stores board size
* Contains snakes and ladders
* Applies position transitions

### 4. Dice

* Generates random values (1–6)

---

## 🔄 Flow

1. Current player rolls dice
2. Player moves forward
3. If landing on:

   * Ladder → move up
   * Snake → move down
4. Position updated
5. Check win condition
6. Switch turn

---

## 📐 Design Decisions

* Board handles snake/ladder logic (`getFinalPosition`)
* Game controls turn management
* Player only stores position
* Used `unordered_map` for fast lookup

---

## ⚠️ Edge Cases

* Move exceeds board size → ignore move
* Snake and ladder at same cell (invalid input)
* Multiple players support

---

## 🚀 Possible Improvements

* Add GUI
* Add configurable board size
* Add multiple dice
* Add game history

---

## 💡 Learning Outcome

* Turn-based system design
* Game flow modeling
* Separation of logic and data
