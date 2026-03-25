//---------------------first come first serve------------------------------
#include <bits/stdc++.h>
using namespace std;

enum class Direction {
    UP,
    DOWN
};

enum class ElevatorState {
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
    int elevatorId;
    int currentFloor;
    ElevatorState state;

    queue<Request> pendingRequests;
    bool hasActiveRequest;
    Request activeRequest;
    bool pickedUpPassenger;

    void updateState(int targetFloor) {
        if (currentFloor < targetFloor) {
            state = ElevatorState::MOVING_UP;
        } else if (currentFloor > targetFloor) {
            state = ElevatorState::MOVING_DOWN;
        } else {
            state = ElevatorState::IDLE;
        }
    }

public:
    Elevator(int id)
        : elevatorId(id),
          currentFloor(0),
          state(ElevatorState::IDLE),
          hasActiveRequest(false),
          activeRequest(0, 1),
          pickedUpPassenger(false) {}

    int getId() const {
        return elevatorId;
    }

    int getCurrentFloor() const {
        return currentFloor;
    }

    ElevatorState getState() const {
        return state;
    }

    bool isIdle() const {
        return !hasActiveRequest && pendingRequests.empty();
    }

    bool hasPendingRequests() const {
        return hasActiveRequest || !pendingRequests.empty();
    }

    void addRequest(const Request& req) {
        pendingRequests.push(req);
        cout << "Request added to Elevator " << elevatorId
             << " : " << req.getSourceFloor()
             << " -> " << req.getDestinationFloor() << "\n";
    }

    void step() {
        if (!hasActiveRequest) {
            if (pendingRequests.empty()) {
                state = ElevatorState::IDLE;
                return;
            }

            activeRequest = pendingRequests.front();
            pendingRequests.pop();
            hasActiveRequest = true;
            pickedUpPassenger = false;
        }

        int targetFloor = pickedUpPassenger
                              ? activeRequest.getDestinationFloor()
                              : activeRequest.getSourceFloor();

        if (currentFloor == targetFloor) {
            if (!pickedUpPassenger) {
                cout << "Elevator " << elevatorId
                     << " picked passenger at floor " << currentFloor << "\n";
                pickedUpPassenger = true;
            } else {
                cout << "Elevator " << elevatorId
                     << " dropped passenger at floor " << currentFloor << "\n";
                hasActiveRequest = false;
                pickedUpPassenger = false;
                state = ElevatorState::IDLE;
            }
            return;
        }

        updateState(targetFloor);

        if (state == ElevatorState::MOVING_UP) {
            currentFloor++;
            cout << "Elevator " << elevatorId
                 << " moved up to floor " << currentFloor << "\n";
        } else if (state == ElevatorState::MOVING_DOWN) {
            currentFloor--;
            cout << "Elevator " << elevatorId
                 << " moved down to floor " << currentFloor << "\n";
        }
    }
};

class ElevatorManager {
private:
    vector<Elevator> elevators;

public:
    ElevatorManager(int numberOfElevators) {
        for (int i = 0; i < numberOfElevators; i++) {
            elevators.emplace_back(i + 1);
        }
    }

    void addRequest(const Request& req) {
        Elevator* chosenElevator = nullptr;

        for (auto& elevator : elevators) {
            if (elevator.isIdle()) {
                chosenElevator = &elevator;
                break;
            }
        }

        if (chosenElevator == nullptr) {
            chosenElevator = &elevators[0];
        }

        chosenElevator->addRequest(req);
        cout << "Assigned request " << req.getSourceFloor()
             << " -> " << req.getDestinationFloor()
             << " to Elevator " << chosenElevator->getId() << "\n";
    }

    void step() {
        for (auto& elevator : elevators) {
            elevator.step();
        }
    }

    bool hasPendingRequests() const {
        for (const auto& elevator : elevators) {
            if (elevator.hasPendingRequests()) {
                return true;
            }
        }
        return false;
    }

    void printStatus() const {
        cout << "Current Status:\n";
        for (const auto& elevator : elevators) {
            cout << "Elevator " << elevator.getId()
                 << " at floor " << elevator.getCurrentFloor() << "\n";
        }
        cout << "--------------------------\n";
    }
};

int main() {
    ElevatorManager manager(2);

    manager.addRequest(Request(8, 2));
    manager.addRequest(Request(3, 6));
    manager.addRequest(Request(5, 1));

    while (manager.hasPendingRequests()) {
        manager.printStatus();
        manager.step();
    }

    manager.printStatus();

    return 0;
}