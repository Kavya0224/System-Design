#include<bits/stdc++.h>
using namespace std;

enum class State{
    BOOKED,
    ONGOING,
    COMPLETED,
    CANCELLED
};

enum class PaymentStatus{
    PENDING,
    SUCCESS,
    FAILED
};

class User{
private:
    int id;
    string name;
    string number;
    string address;

public:
    User(int id,
         string name,
         string no,
         string address)

        : id(id),
          name(name),
          number(no),
          address(address) {}

    int getUserId() const{
        return id;
    }

    string getUserName() const{
        return name;
    }

    string getUserNumber() const{
        return number;
    }
};

class Car{
private:
    int id;
    string type;
    string carName;
    bool available;
    int pricePerDay;

public:

    Car(){}

    Car(string type,
        int id,
        string name,
        int price)

        : type(type),
          id(id),
          carName(name),
          available(true),
          pricePerDay(price) {}

    string getCartype() const{
        return type;
    }

    string getCarName() const{
        return carName;
    }

    int getCarid() const{
        return id;
    }

    int getPrice() const{
        return pricePerDay;
    }

    bool isAvailable() const{
        return available;
    }

    void bookCar(){
        available = false;
    }

    void releaseCar(){
        available = true;
    }
};

class Payment{
private:
    int paymentId;
    int amount;
    PaymentStatus status;

public:

    Payment(int id,int amount)

        : paymentId(id),
          amount(amount),
          status(PaymentStatus::PENDING) {}

    bool processPayment(){
        status = PaymentStatus::SUCCESS;
        cout<<"Payment Successful"<<endl;
        return true;
    }

    PaymentStatus getStatus() const{
        return status;
    }
};

class Booking{
private:
    int bookingId;
    User user;
    Car car;
    State state;

public:

    Booking(int id,
            User user,
            Car car)

        : bookingId(id),
          user(user),
          car(car),
          state(State::BOOKED) {}

    void startRide(){
        if(state != State::BOOKED){
            cout<<"Ride cannot start"<<endl;
            return;
        }
        state = State::ONGOING;
        cout<<"Ride Started"<<endl;
    }

    void completeRide(){
        if(state != State::ONGOING){
            cout<<"Ride not ongoing"<<endl;
            return;
        }
        state = State::COMPLETED;
        cout<<"Ride Completed"<<endl;
    }

    void cancelBooking(){
        if(state == State::COMPLETED){
            cout<<"Cannot cancel completed ride"<<endl;
            return;
        }
        state = State::CANCELLED;
        cout<<"Booking Cancelled"<<endl;
    }

    Car getCar() const{
        return car;
    }
};

class CarRentalSystem{
private:
    unordered_map<int,Car> cars;
    vector<Booking> bookings;
    int bookingCounter = 1;
    int paymentCounter = 1;

public:

    void addCar(Car& car){
        cars.insert(
            {
                car.getCarid(),
                car
            }
        );
    }

    void showCar(){
        cout<<"Available Cars"<<endl;
        for(auto& it : cars){
            if(it.second.isAvailable()){
                cout<<"Car Id : "
                    <<it.second.getCarid()
                    <<endl;

                cout<<"Car Name : "
                    <<it.second.getCarName()
                    <<endl;

                cout<<"Car Type : "
                    <<it.second.getCartype()
                    <<endl;

                cout<<"Price Per Day : "
                    <<it.second.getPrice()
                    <<endl;

                cout<<endl;
            }
        }
    }

    void BookCar(int carId,User& user){
        if(cars.find(carId)==cars.end()){
            cout<<"Car not found"<<endl;
            return;
        }
        Car& car = cars[carId];
        if(!car.isAvailable()){
            cout<<"Car already booked"<<endl;
            return;
        }
        car.bookCar();
        Payment payment(
            paymentCounter++,
            car.getPrice()
        );

        bool success = payment.processPayment();

        if(!success){
            car.releaseCar();
            cout<<"Payment Failed"<<endl;
            return;
        }

        Booking booking(
            bookingCounter++,
            user,
            car
        );

        bookings.push_back(booking);
        cout<<"Booking Successful"<<endl;
        cout<<"User : "
            <<user.getUserName()
            <<endl;

        cout<<"Car : "
            <<car.getCarName()
            <<endl;
    }

    void displayBookings(){
        cout<<"Total Bookings : "
            <<bookings.size()
            <<endl;
    }
};

int main(){

    User user1(
        1,
        "Kavya",
        "9876543210",
        "Delhi"
    );

    Car car1(
        "SUV",
        101,
        "Thar",
        5000
    );

    Car car2(
        "Sedan",
        102,
        "Honda City",
        3000
    );

    CarRentalSystem system;

    system.addCar(car1);

    system.addCar(car2);

    system.showCar();

    system.BookCar(
        101,
        user1
    );

    cout<<endl;

    system.showCar();

    cout<<endl;

    system.displayBookings();

    return 0;
}