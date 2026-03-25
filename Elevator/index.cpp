//--------------------------Serves req in one direction first then another----------------
#include <bits/stdc++.h>
using namespace std;

enum class Direction {
    UP,
    DOWN
};

enum class State {
    IDLE,
    MOVING_UP,
    MOVING_DOWN
};

class Request {
private:
    int sourceFloor;
    int destinationFloor;
    Direction direction;

public:
    Request(int sourceFloor, int destinationFloor)
        : sourceFloor(sourceFloor), destinationFloor(destinationFloor) {
        direction = (destinationFloor > sourceFloor) ? Direction::UP : Direction::DOWN;
    }

    int getSourceFloor() const {
        return sourceFloor;
    }

    int getDestinationFloor() const {
        return destinationFloor;
    }

    Direction getDirection() const {
        return direction;
    }
};

class Elevator {
private:
    int id;
    int currentFloor;
    State state;
    set<int> upRequests;
    set<int> downRequests;

public:
    Elevator(int id) : id(id), currentFloor(0), state(State::IDLE) {}

    void addRequest(const Request& req) {
        int destination = req.getDestinationFloor();

        if (destination > currentFloor) {
            upRequests.insert(destination);
        } else if (destination < currentFloor) {
            downRequests.insert(destination);
        }

        if (state == State::IDLE) {
            if (!upRequests.empty()) state = State::MOVING_UP;
            else if (!downRequests.empty()) state = State::MOVING_DOWN;
        }
    }

    void move() {
        if (state == State::IDLE) {
            if (!upRequests.empty()) state = State::MOVING_UP;
            else if (!downRequests.empty()) state = State::MOVING_DOWN;
            else return;
        }

        if (state == State::MOVING_UP) {
            if (!upRequests.empty()) {
                currentFloor++;
                cout << "Elevator " << id << " moved up to floor " << currentFloor << endl;

                if (upRequests.count(currentFloor)) {
                    cout << "Elevator " << id << " stopped at floor " << currentFloor << endl;
                    upRequests.erase(currentFloor);
                }
            }

            if (upRequests.empty()) {
                if (!downRequests.empty()) state = State::MOVING_DOWN;
                else state = State::IDLE;
            }
        }
        else if (state == State::MOVING_DOWN) {
            if (!downRequests.empty()) {
                currentFloor--;
                cout << "Elevator " << id << " moved down to floor " << currentFloor << endl;

                if (downRequests.count(currentFloor)) {
                    cout << "Elevator " << id << " stopped at floor " << currentFloor << endl;
                    downRequests.erase(currentFloor);
                }
            }

            if (downRequests.empty()) {
                if (!upRequests.empty()) state = State::MOVING_UP;
                else state = State::IDLE;
            }
        }
    }

    int getCurrentFloor() const {
        return currentFloor;
    }

    State getState() const {
        return state;
    }

    int getId() const {
        return id;
    }

    bool isIdle() const {
        return state == State::IDLE;
    }

    bool hasPendingRequests() const {
    return !upRequests.empty() || !downRequests.empty();
}
};

class ElevatorManager {
private:
    vector<Elevator> elevators;

public:
    ElevatorManager(int n) {
        for (int i = 0; i < n; i++) {
            elevators.push_back(Elevator(i + 1));
        }
    }

    void addRequest(const Request& req) {
        Elevator* chosenElevator = nullptr;

        // first try to find an idle elevator
        for (auto& elevator : elevators) {
            if (elevator.isIdle()) {
                chosenElevator = &elevator;
                break;
            }
        }

        // if no idle elevator found, assign to first elevator
        if (chosenElevator == nullptr) {
            chosenElevator = &elevators[0];
        }

        chosenElevator->addRequest(req);
        cout << "Request assigned to Elevator " << chosenElevator->getId() << endl;
    }

    void step() {
        for (auto& elevator : elevators) {
            elevator.move();
        }
    }

    void status() const {
        for (const auto& elevator : elevators) {
            cout << "Elevator " << elevator.getId()
                 << " is at floor " << elevator.getCurrentFloor() << endl;
        }
    }
    bool hasPendingRequests() const {
        for (const auto& elevator : elevators) {
            if (elevator.hasPendingRequests()) return true;
        }
        return false;
    }
};

int main() {
    ElevatorManager manager(2);

    manager.addRequest(Request(3, 6));
    manager.addRequest(Request(3, 5));
    manager.addRequest(Request(8, 2));

    while (manager.hasPendingRequests()) {
        manager.step();
    }
    return 0;
}