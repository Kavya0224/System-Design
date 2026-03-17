#include <bits/stdc++.h>
using namespace std;

enum class VehicleType {
    TWO_WHEELER,
    FOUR_WHEELER,
    LARGE_VEHICLE
};

enum class SpotType {
    TWO_WHEELER,
    FOUR_WHEELER,
    LARGE
};

class Vehicle {
private:
    string vehicleNumber;
    VehicleType type;

public:
    Vehicle(string number, VehicleType type) : vehicleNumber(number), type(type) {}

    string getVehicleNumber() const {
        return vehicleNumber;
    }

    VehicleType getType() const {
        return type;
    }
};

class ParkingSpot {
private:
    int spotId;
    SpotType type;
    bool occupied;

public:
    ParkingSpot(int id, SpotType type) : spotId(id), type(type), occupied(false) {}

    bool isAvailable() const {
        return !occupied;
    }

    SpotType getType() const {
        return type;
    }

    int getSpotId() const {
        return spotId;
    }

    void occupy() {
        occupied = true;
    }

    void freeSpot() {
        occupied = false;
    }
};

class Ticket {
private:
    int ticketId;
    string vehicleNumber;
    int floorNumber;
    int spotId;
    time_t entryTime;
    time_t exitTime;
    double fare;

public:
    Ticket(int ticketId, string vehicleNumber, int floorNumber, int spotId, time_t entryTime)
        : ticketId(ticketId), vehicleNumber(vehicleNumber), floorNumber(floorNumber),
          spotId(spotId), entryTime(entryTime), exitTime(0), fare(0) {}

    int getTicketId() const {
        return ticketId;
    }

    string getVehicleNumber() const {
        return vehicleNumber;
    }

    int getFloorNumber() const {
        return floorNumber;
    }

    int getSpotId() const {
        return spotId;
    }

    time_t getEntryTime() const {
        return entryTime;
    }

    void closeTicket(time_t exit, double totalFare) {
        exitTime = exit;
        fare = totalFare;
    }

    double getFare() const {
        return fare;
    }
};

class ParkingFloor {
private:
    int floorNumber;
    vector<ParkingSpot> spots;

    bool matches(const Vehicle& vehicle, const ParkingSpot& spot) const {
        if (vehicle.getType() == VehicleType::TWO_WHEELER &&
            spot.getType() == SpotType::TWO_WHEELER) return true;

        if (vehicle.getType() == VehicleType::FOUR_WHEELER &&
            spot.getType() == SpotType::FOUR_WHEELER) return true;

        if (vehicle.getType() == VehicleType::LARGE_VEHICLE &&
            spot.getType() == SpotType::LARGE) return true;

        return false;
    }

public:
    ParkingFloor(int floorNumber) : floorNumber(floorNumber) {}

    int getFloorNumber() const {
        return floorNumber;
    }

    void addSpot(const ParkingSpot& spot) {
        spots.push_back(spot);
    }

    int assignSpot(const Vehicle& vehicle) {
        for (auto& spot : spots) {
            if (spot.isAvailable() && matches(vehicle, spot)) {
                spot.occupy();
                return spot.getSpotId();
            }
        }
        return -1;
    }

    bool releaseSpot(int spotId) {
        for (auto& spot : spots) {
            if (spot.getSpotId() == spotId) {
                spot.freeSpot();
                return true;
            }
        }
        return false;
    }
};

class FeeCalculator {
public:
    double calculateFare(time_t entryTime, time_t exitTime) {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        if (hours < 1) hours = 1;
        return ceil(hours) * 20;
    }
};

class ParkingLot {
private:
    vector<ParkingFloor> floors;
    unordered_map<int, Ticket> activeTickets;
    FeeCalculator feeCalculator;
    int nextTicketId = 1;

public:
    void addFloor(const ParkingFloor& floor) {
        floors.push_back(floor);
    }

    int parkVehicle(const Vehicle& vehicle) {
        for (auto& floor : floors) {
            int spotId = floor.assignSpot(vehicle);
            if (spotId != -1) {
                int ticketId = nextTicketId++;
                Ticket ticket(ticketId, vehicle.getVehicleNumber(),
                              floor.getFloorNumber(), spotId, time(nullptr));
                activeTickets.emplace(ticketId, ticket);
                return ticketId;
            }
        }
        return -1;
    }

    double unparkVehicle(int ticketId) {
        auto it = activeTickets.find(ticketId);
        if (it == activeTickets.end()) {
            return -1;
        }

        Ticket& ticket = it->second;
        time_t exitTime = time(nullptr);
        double fare = feeCalculator.calculateFare(ticket.getEntryTime(), exitTime);

        for (auto& floor : floors) {
            if (floor.getFloorNumber() == ticket.getFloorNumber()) {
                floor.releaseSpot(ticket.getSpotId());
                break;
            }
        }

        ticket.closeTicket(exitTime, fare);
        return fare;
    }
};

int main() {
    ParkingLot parkingLot;

    ParkingFloor floor1(1);
    floor1.addSpot(ParkingSpot(1, SpotType::TWO_WHEELER));
    floor1.addSpot(ParkingSpot(2, SpotType::TWO_WHEELER));
    floor1.addSpot(ParkingSpot(3, SpotType::FOUR_WHEELER));
    floor1.addSpot(ParkingSpot(4, SpotType::LARGE));

    ParkingFloor floor2(2);
    floor2.addSpot(ParkingSpot(1, SpotType::TWO_WHEELER));
    floor2.addSpot(ParkingSpot(2, SpotType::FOUR_WHEELER));
    floor2.addSpot(ParkingSpot(3, SpotType::LARGE));

    parkingLot.addFloor(floor1);
    parkingLot.addFloor(floor2);

    Vehicle bike("BIKE123", VehicleType::TWO_WHEELER);
    Vehicle car("CAR456", VehicleType::FOUR_WHEELER);
    Vehicle truck("TRUCK789", VehicleType::LARGE_VEHICLE);

    int bikeTicket = parkingLot.parkVehicle(bike);
    int carTicket = parkingLot.parkVehicle(car);
    int truckTicket = parkingLot.parkVehicle(truck);

    if (bikeTicket != -1) cout << "Bike parked. Ticket ID: " << bikeTicket << "\n";
    else cout << "No space for bike\n";

    if (carTicket != -1) cout << "Car parked. Ticket ID: " << carTicket << "\n";
    else cout << "No space for car\n";

    if (truckTicket != -1) cout << "Truck parked. Ticket ID: " << truckTicket << "\n";
    else cout << "No space for truck\n";

    double bikeFare = parkingLot.unparkVehicle(bikeTicket);
    if (bikeFare != -1) cout << "Bike exited. Fare: " << bikeFare << "\n";
    else cout << "Invalid bike ticket\n";

    double carFare = parkingLot.unparkVehicle(carTicket);
    if (carFare != -1) cout << "Car exited. Fare: " << carFare << "\n";
    else cout << "Invalid car ticket\n";

    return 0;
}