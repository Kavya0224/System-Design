#include<bits/stdc++.h>
using namespace std;

enum class State {
    CREATED,
    ACCEPTED,
    PREPARING,
    OUT_FOR_DELIVERY,
    DELIVERED
};

class User {
private:
    string userName;
    string userNumber;

public:
    User(string name, string number) 
        : userName(name), userNumber(number) {}

    string getUserName() const {
        return userName;
    }

    string getUserNumber() const {
        return userNumber;
    }
};

class Item{
private:
    string itemName;
    int price;
public:
    Item(string name,int price) : itemName(name),price(price) {};
    string getItemName() const{
        return itemName;
    }
    int getItemPrice() const{
        return price;
    }
};

class Restaurant {
private:
    string restaurantName;
    string location;
    vector<Item> menu; //can use map too but for simplicity I used vector

public:
    Restaurant(string name, string location) 
        : restaurantName(name), location(location) {}
    
    void addItem(const Item& item){
        menu.push_back(item);
    }

    void displayMenu() const {
        for(const auto& item : menu){
            cout << item.getItemName() << " " << item.getItemPrice() << endl;
        }
    }

    bool isPresentItem(const Item& item) const {
        //current : O(n)
        //map : O(1)
        for(const auto& it : menu){
            if(it.getItemName() == item.getItemName()) return true;
        }
        return false;
    }

    bool isOrderAccepted() const {
        return true;
    }
};

class Order {
private:
    int orderId;
    User user;
    Restaurant restaurant;
    vector<Item> items;
    State state;

public:
    Order(int id, const User& user, const Restaurant& restaurant)
        : orderId(id), user(user), restaurant(restaurant), state(State::CREATED) {}

    void selectItem(const Item& item){
        if(restaurant.isPresentItem(item)) {
            items.push_back(item);
        } else {
            cout << "Item not available" << endl;
        }
    }
    User getUser() const{
        return user;
    }
    Restaurant getRestaurant() const{
        return restaurant;
    }
    void updateState(State newState) {
        state = newState;
    }

    State getState() const {
        return state;
    }
};

class DeliveryPartner {
private:
    int id;
    bool available;

public:
    DeliveryPartner(int id) : id(id), available(true) {}

    bool isAvailable() const {
        return available;
    }

    void assign() {
        available = false;
    }

    void free() {
        available = true;
    }
};

class OrderManager {
private:
    vector<Order> orders;
    vector<DeliveryPartner> deliveryPartners;

public:
    void addDeliveryPartner(const DeliveryPartner& partner) {
        deliveryPartners.push_back(partner);
    }

    void placeOrder(Order& order) {
        // Step 1: Restaurant accepts order
        if (!order.getRestaurant().isOrderAccepted()) {
            cout << "Order rejected by restaurant\n";
            return;
        }

        order.updateState(State::ACCEPTED);
        cout << "Order Accepted\n";

        // Step 2: Preparing
        order.updateState(State::PREPARING);
        cout << "FOOD IS PREPARING...\n";

        // Step 3: Assign delivery partner
        bool assigned = false;
        for (auto& partner : deliveryPartners) {
            if (partner.isAvailable()) {
                partner.assign();
                assigned = true;

                order.updateState(State::OUT_FOR_DELIVERY);
                cout << "Order is out for delivery\n";
                break;
            }
        }

        if (!assigned) {
            cout << "No delivery partner available\n";
            return;
        }

        // Step 4: Delivered
        order.updateState(State::DELIVERED);
        cout << "Order Delivered to: "
             << order.getUser().getUserName() << endl;

        // store order
        orders.push_back(order);
    }
};

int main() {
    // Users
    User user1("ROY", "9999999999");

    // Restaurant
    Restaurant rest("PizzaHub", "Delhi");

    // Items
    Item item1("Pizza", 300);
    Item item2("Burger", 150);

    rest.addItem(item1);
    rest.addItem(item2);

    // Display menu
    cout << "Menu:\n";
    rest.displayMenu();

    // Create Order
    Order order1(1, user1, rest);
    order1.selectItem(item1);
    order1.selectItem(item2);

    // Delivery Partners
    DeliveryPartner dp1(1);
    DeliveryPartner dp2(2);

    // Order Manager
    OrderManager manager;
    manager.addDeliveryPartner(dp1);
    manager.addDeliveryPartner(dp2);

    // Place Order
    manager.placeOrder(order1);

    return 0;
}