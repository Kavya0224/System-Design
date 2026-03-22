Vending Machine - Low Level Design
____________________________________________________________________________________________________
📌 Problem Statement
----------------------------------------------------------------------------------------------------

Design a Vending Machine system that supports:

Item selection
Money insertion
Item dispensing
Change return
Out-of-stock handling
Transaction cancellation
____________________________________________________________________________________________________
🧠 Design Overview
----------------------------------------------------------------------------------------------------
This system models a real-world vending machine using object-oriented design.
The goal was to keep the design simple, clean, and extensible while separating responsibilities across classes.
____________________________________________________________________________________________________
🧩 Core Classes
----------------------------------------------------------------------------------------------------
1. VendingMachine
Controls the overall transaction flow
Handles item selection
Accepts money
Dispenses item
Returns change or refund
2. Item
Stores item details like:
item name
price
3. Slot
Represents a vending machine slot
Contains:
slot ID
item
quantity
Checks availability
Reduces quantity after dispensing
4. Inventory
Stores all slots
Provides slot lookup
Checks slot availability
5. Payment
Tracks total inserted money
Checks whether enough money is inserted
Calculates remaining change
Resets transaction money after completion/cancellation
____________________________________________________________________________________________________
🔄 Flow
----------------------------------------------------------------------------------------------------
Normal Flow
User selects an item using slot ID
Vending machine checks whether slot exists and item is available
User inserts money
Machine checks if inserted money is enough
Item is dispensed
Quantity is reduced
Remaining change is returned
Payment/session is reset
Cancel Flow
User selects item
User inserts some money
User cancels transaction
Inserted money is refunded
Machine resets the current session
____________________________________________________________________________________________________
📐 Design Decisions
----------------------------------------------------------------------------------------------------
Inventory is a separate class and is stored inside VendingMachine
Payment is separated from vending logic to keep money handling isolated
Slot contains Item using composition
Slot* is used instead of Slot while selecting item, so the original slot in inventory is modified instead of a copy
VendingMachine handles transaction flow, while Inventory and Payment manage their own responsibilities
____________________________________________________________________________________________________
⚠️ Edge Cases Considered
----------------------------------------------------------------------------------------------------
Invalid slot selection
Selected item out of stock
Inserted money less than item price
Cancelling transaction after partial payment
Quantity becoming zero after dispensing
____________________________________________________________________________________________________
🚀 Possible Improvements
----------------------------------------------------------------------------------------------------
Add vending machine states using State Pattern
Support multiple payment modes
Add admin refill functionality
Add transaction history
Add slot codes like A1, B2 instead of integer slot IDs
Add change denomination management
____________________________________________________________________________________________________
💡 Learning Outcome
----------------------------------------------------------------------------------------------------
This project helped me practice:

low level design fundamentals
object-oriented modeling
separation of concerns
inventory handling
transaction flow design
pointer vs object/reference usage in C++
🛠 Example Components
Item → item details
Slot → item + quantity + slot ID
Inventory → collection of slots
Payment → inserted money + change logic
VendingMachine → user flow controller
____________________________________________________________________________________________________
📚 Key Takeaway
----------------------------------------------------------------------------------------------------
A good vending machine design is not about making it complex.
It is about clearly separating:

item storage
stock handling
money handling
transaction flow

That makes the system easier to understand, extend, and maintain.