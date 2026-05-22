#include <bits/stdc++.h>
using namespace std;

class Product {
private:
    int productId;
    string productName;
    int productPrice;

public:
    Product(int id, string name, int price)
        : productId(id), productName(name), productPrice(price) {}

    int getProductId() const {
        return productId;
    }

    string getProductName() const {
        return productName;
    }

    int getProductPrice() const {
        return productPrice;
    }
};

class User {
private:
    int userId;
    string userName;

public:
    User(int id, string name) : userId(id), userName(name) {}

    int getUserId() const {
        return userId;
    }

    string getUserName() const {
        return userName;
    }
};

class Inventory {
private:
    unordered_map<int, int> stock;

public:
    void addProduct(int productId, int quantity) {
        stock[productId] += quantity;
    }

    bool isAvailable(int productId, int quantity) {
        return stock[productId] >= quantity;
    }

    void reduceStock(int productId, int quantity) {
        stock[productId] -= quantity;
    }

    int getStock(int productId) {
        return stock[productId];
    }
};

class CartItem {
private:
    Product product;
    int quantity;

public:
    CartItem() : product(0,"",0), quantity(0) {}

    CartItem(Product product,int quantity)
        : product(product),quantity(quantity) {}

    Product getProduct() const {
        return product;
    }

    int getQuantity() const {
        return quantity;
    }

    void updateQuantity(int qty) {
        quantity = qty;
    }

    int getTotalPrice() const {
        return quantity * product.getProductPrice();
    }
};

class Cart {
private:
    User user;
    Inventory& inventory;
    unordered_map<int, CartItem> items;

public:
    Cart(User user, Inventory& inventory) : user(user), inventory(inventory) {}

    void addItem(Product product, int quantity) {
        if (!inventory.isAvailable(product.getProductId(),quantity)) {
            cout << "Stock not available\n";
            return;
        }
        int productId = product.getProductId();
        if (items.find(productId) != items.end()) {
            int newQty = items[productId].getQuantity() + quantity;
            items[productId].updateQuantity(newQty);
        }
        else {
            items.emplace(productId,CartItem(product, quantity)
            );
        }
        cout << "Item added successfully\n";
    }

    void removeItem(int productId) {
        if (items.find(productId) == items.end()) {
            cout << "Item not present\n";
            return;
        }
        items.erase(productId);
        cout << "Item removed\n";
    }

    void updateQuantity(int productId,int quantity) {
        if (items.find(productId) == items.end()) {
            cout << "Item not present\n";
            return;
        }
        if (!inventory.isAvailable(productId,quantity)) {
            cout << "Stock not available\n";
            return;
        }
        items[productId].updateQuantity(quantity);
        cout << "Quantity updated\n";
    }

    int getCartTotal() {
        int total = 0;
        for (auto& item : items) {
            total += item.second.getTotalPrice();
        }
        return total;
    }

    void displayCart() {
        cout << "\nCart Items\n";
        for (auto& item : items) {
            CartItem cartItem = item.second;
            cout<< cartItem.getProduct().getProductName()
                << " | Qty: "
                << cartItem.getQuantity()
                << " | Price: "
                << cartItem.getTotalPrice()
                << endl;
        }

        cout<< "Total = "
            << getCartTotal()
            << endl;
    }

    void checkout() {
        for (auto& item : items) {
            int productId = item.second.getProduct().getProductId();
            int quantity = item.second.getQuantity();
            if (!inventory.isAvailable(productId,quantity)) {
                cout<< "Checkout failed\n";
                return;
            }
        }

        for (auto& item : items) {
            int productId = item.second.getProduct().getProductId();
            int quantity = item.second.getQuantity();
            inventory.reduceStock(productId,quantity);
        }
        cout<< "Checkout Successful\n";
        items.clear();
    }
};

int main() {

    User user(1, "XYZ");

    Product laptop(
        101,
        "Laptop",
        50000);

    Product mouse(
        102,
        "Mouse",
        1000);

    Inventory inventory;

    inventory.addProduct(
        laptop.getProductId(),
        10);

    inventory.addProduct(
        mouse.getProductId(),
        20);

    Cart cart(user, inventory);

    cart.addItem(laptop, 1);
    cart.addItem(mouse, 2);

    cart.displayCart();

    cart.updateQuantity(
        mouse.getProductId(),
        5);

    cart.displayCart();

    cart.checkout();

    cout
        << "Laptop Stock Left: "
        << inventory.getStock(
               laptop.getProductId())
        << endl;

    return 0;
}