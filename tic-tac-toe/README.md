# Tic Tac Toe - Low Level Design

## 📌 Problem Statement

Design a Tic Tac Toe game that supports:

* Two players
* Turn-based moves
* Win detection
* Draw condition

---

## 🧠 Design Overview

The system models a simple grid-based game with focus on clean class design and game state management.

---

## 🧩 Core Classes

### 1. Game

* Controls game flow
* Manages turns
* Determines winner

### 2. Board

* Stores grid
* Validates moves
* Checks win/draw

### 3. Player

* Stores player ID and symbol (X/O)

### 4. Move (optional)

* Represents a move (row, col)

---

## 🔄 Flow

1. Player makes move
2. Board validates move
3. Board updates grid
4. Check win condition
5. Check draw condition
6. Switch turn

---

## 📐 Design Decisions

* Board handles validation and win logic
* Game controls turns
* Player only stores identity and symbol
* Avoided putting all logic in one class

---

## ⚠️ Edge Cases

* Invalid move (cell already filled)
* Out-of-bound input
* Game already finished

---

## 🚀 Possible Improvements

* NxN board support
* Undo functionality
* AI player
* Multiplayer over network

---

## 💡 Learning Outcome

* Grid-based system design
* State handling
* Validation logic separation

