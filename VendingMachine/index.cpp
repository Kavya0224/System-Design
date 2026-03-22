#include<bits/stdc++.h>
using namespace std;

class Item {
private:
    string itemName;
    int price;

public:
    Item(string name, int price) : itemName(name), price(price) {}

    string getItemName() const {
        return itemName;
    }

    int getPrice() const {
        return price;
    }
};

class Slot {
private:
    int slotId;
    Item item;
    int quantity;

public:
    Slot(int id, Item item, int quantity)
        : slotId(id), item(item), quantity(quantity) {}

    int getSlotId() const {
        return slotId;
    }

    bool isAvailable() const {
        return quantity > 0;
    }

    bool reduceQuantity() {
        if (quantity <= 0) return false;
        quantity--;
        return true;
    }

    Item getItem() const {
        return item;
    }

    int getQuantity() const {
        return quantity;
    }
};

class Inventory {
private:
    unordered_map<int, Slot> slots;

public:
    void addSlot(const Slot& slot) {
        int slotId = slot.getSlotId();
        if (slots.find(slotId) == slots.end()) {
            slots.emplace(slotId, slot);
        }
    }

    bool checkAvailability(int slotId) {
        auto it = slots.find(slotId);
        if (it == slots.end()) return false;

        return it->second.isAvailable();
    }

    Slot* getSlot(int slotId) {
        auto it = slots.find(slotId);
        if (it == slots.end()) return nullptr;

        return &(it->second);
    }
};

class Payment {
private:
    int totalMoney;

public:
    Payment() : totalMoney(0) {}

    void insertMoney(int amount) {
        totalMoney += amount;
    }

    int getTotalMoney() const {
        return totalMoney;
    }

    bool hasEnough(int price) const {
        return totalMoney >= price;
    }

    int getChange(int price) const {
        return totalMoney - price;
    }

    void reset() {
        totalMoney = 0;
    }
};

enum class MachineState {
    IDLE,
    ITEM_SELECTED,
    PAYMENT_PENDING,
    DISPENSING
};

class VendingMachine {
private:
    Inventory inventory;
    Payment payment;
    Slot* selectedSlot;
    MachineState state;

public:
    VendingMachine(const Inventory& inventory)
        : inventory(inventory), selectedSlot(nullptr), state(MachineState::IDLE) {}

    bool selectItem(int slotId) {
        Slot* slot = inventory.getSlot(slotId);

        if (slot == nullptr) {
            cout << "Invalid slot\n";
            return false;
        }

        if (!slot->isAvailable()) {
            cout << "Item out of stock\n";
            return false;
        }

        selectedSlot = slot;
        state = MachineState::ITEM_SELECTED;

        cout << "Selected item: " << selectedSlot->getItem().getItemName() << "\n";
        cout << "Price: " << selectedSlot->getItem().getPrice() << "\n";
        return true;
    }

    void insertMoney(int amount) {
        if (selectedSlot == nullptr) {
            cout << "Please select item first\n";
            return;
        }

        payment.insertMoney(amount);
        state = MachineState::PAYMENT_PENDING;

        cout << "Inserted money: " << amount << "\n";
        cout << "Total money: " << payment.getTotalMoney() << "\n";
    }

    void dispenseItem() {
        if (selectedSlot == nullptr) {
            cout << "No item selected\n";
            return;
        }

        int price = selectedSlot->getItem().getPrice();

        if (!payment.hasEnough(price)) {
            cout << "Not enough money\n";
            return;
        }

        state = MachineState::DISPENSING;
        selectedSlot->reduceQuantity();

        cout << "Dispensed item: " << selectedSlot->getItem().getItemName() << "\n";

        int change = payment.getChange(price);
        cout << "Returned change: " << change << "\n";

        payment.reset();
        selectedSlot = nullptr;
        state = MachineState::IDLE;
    }

    void cancelTransaction() {
        cout << "Transaction cancelled\n";
        cout << "Refunded money: " << payment.getTotalMoney() << "\n";
        payment.reset();
        selectedSlot = nullptr;
        state = MachineState::IDLE;
    }
};
int main() {
    Item chips("Chips", 20);
    Item coke("Coke", 30);

    Slot slot1(1, chips, 5);
    Slot slot2(2, coke, 3);

    Inventory inventory;
    inventory.addSlot(slot1);
    inventory.addSlot(slot2);

    VendingMachine vm(inventory);

    vm.selectItem(1);
    vm.insertMoney(10);
    vm.insertMoney(10);
    vm.dispenseItem();

    return 0;
}