#include<bits/stdc++.h>
using namespace std;

enum class State{
    REQUESTED,
    ACCEPTED,
    CANCELLED,
    ONGOING,
    COMPLETED,
};

class Rider {
private:
    int userId;
    string userName;
    string userNumber;

public:
    Rider(int id, const string& name, const string& no)
        : userId(id), userName(name), userNumber(no) {}

    int getUserId() const {
        return userId;
    }

    string getUserName() const {
        return userName;
    }

    string getUserNumber() const {
        return userNumber;
    }
};

class Driver {
private:
    int driverId;
    string driverName;
    string driverNumber;
    bool available;

public:
    Driver(int id, const string& name, const string& no)
        : driverId(id), driverName(name), driverNumber(no), available(true) {}

    int getDriverId() const {
        return driverId;
    }

    string getDriverName() const {
        return driverName;
    }

    string getDriverNumber() const {
        return driverNumber;
    }

    bool isAvailable() const {
        return available;
    }

    void assignRide() {
        available = false;
    }

    void completeRide() {
        available = true;
    }
};

/*
class Location{
private:
    string address;
    Location(string address) : address(address) {}    //---FIX LOCATION---
public:
    string getAddress() const {
        return address;
    }
};
*/

class Location {
private:
    int x;
    int y;

public:
    Location(int x, int y) : x(x), y(y) {}

    int distance(const Location& other) const {
        return abs(x - other.x) + abs(y - other.y);
    }
};

class DriverAssignStrategy {
public:
    virtual Driver* findDriver(vector<Driver>& drivers,const Location& src) = 0;

    virtual ~DriverAssignStrategy() {}
};

class FirstFreeDriver : public DriverAssignStrategy {
public:
    Driver* findDriver(vector<Driver>& drivers,
                       const Location&) override {

        for (auto& d : drivers) {
            if (d.isAvailable()) {
                return &d;
            }
        }
        return nullptr;
    }
};

class Ride {
private:
    int rideId;
    Rider rider;
    Driver* driver;
    Location startLocation;
    Location endLocation;
    State state;
    int fare;

public:
    Ride(int id, const Rider& rider,
         const Location& start, const Location& end)
        : rideId(id), rider(rider),
          driver(nullptr),
          startLocation(start),
          endLocation(end),
          state(State::REQUESTED),
          fare(0) {}

    void assignDriver(Driver* d) {
        if (!d) return;

        driver = d;
        driver->assignRide();
        state = State::ACCEPTED;
    }

    void startRide() {
        state = State::ONGOING;
    }

    void completeRide() {
        state = State::COMPLETED;
        if (driver) driver->completeRide();
    }

    int calculateFare() {
        int dist = startLocation.distance(endLocation);
        fare = dist * 10;
        return fare;
    }

    Location getStartLocation() const {
        return startLocation;
    }

    Location getendLocation() const {
        return endLocation;
    }
    
};

class RideManager {
private:
    vector<Driver> drivers;
    vector<Ride> rides;
    DriverAssignStrategy* strategy;

public:
    RideManager(DriverAssignStrategy* strategy)
        : strategy(strategy) {}

    void addDriver(const Driver& d) {
        drivers.push_back(d);
    }

    void requestRide(Ride ride) {
        Driver* d = strategy->findDriver(drivers, ride.getStartLocation());

        if (!d) {
            cout << "No driver available\n";
            return;
        }

        ride.assignDriver(d);
        cout << "Driver assigned\n";

        ride.startRide();
        cout << "Ride started\n";

        ride.completeRide();
        cout << "Ride completed. Fare: " << ride.calculateFare() << endl;

        rides.push_back(ride);
    }

    void setStrategy(DriverAssignStrategy* newStrategy) {
        strategy = newStrategy;
    }
};

int main() {
    // Create Riders
    Rider rider1(1, "Kavya", "9999999999");

    // Create Drivers
    Driver d1(101, "Amit", "8888888888");
    Driver d2(102, "Rohit", "7777777777");

    // Create Locations
    Location start(0, 0);
    Location end(5, 5);

    // Create Strategy
    DriverAssignStrategy* strategy = new FirstFreeDriver();

    // Create RideManager
    RideManager manager(strategy);

    // Add drivers
    manager.addDriver(d1);
    manager.addDriver(d2);

    // Create Ride
    Ride ride1(1, rider1, start, end);

    // Request Ride
    manager.requestRide(ride1);

    // Cleanup
    delete strategy;

    return 0;
}