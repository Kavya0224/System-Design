#include<bits/stdc++.h>
using namespace std;

enum class SeatState {
    AVAILABLE,
    LOCKED,
    BOOKED
};

enum class BookingState{
    CREATED,
    PAYMENT_PENDING,
    CONFIRMED,
    CANCELLED,
    FAILED
};

class User{
private:
    int userId;
    string UserName;
public:
    User(int id,string name) : userId(id),UserName(name) {}

    int getUserId() const {
        return userId;
    }
};

class Seat {
private:
    int seatId;
    string seatType;
    SeatState state;

public:
    Seat(int id, const string& type)
        : seatId(id),
          seatType(type),
          state(SeatState::AVAILABLE) {}

    bool isAvailable() const {
        return state == SeatState::AVAILABLE;
    }

    void lockSeat() {
        if (state == SeatState::AVAILABLE) {
            state = SeatState::LOCKED;
        }
    }
    // You should eventually add:
    //    -> lockedByUser
    //    -> lockTimestamp
    //     Why?
    //     Because: seat locked forever if payment abandoned
    //     Real systems add: lock expiry (5 mins)

    void bookSeat() {
        if (state == SeatState::LOCKED) {
            state = SeatState::BOOKED;
        }
    }

    void releaseSeat() {
        if (state == SeatState::LOCKED) {
            state = SeatState::AVAILABLE;
        }
    }

    SeatState getState() const {
        return state;
    }

    int getSeatId() const {
        return seatId;
    }
};

class Show {
private:
    int showId;
    string movieName;
    int screenId;

    vector<Seat> seats;

    string timing;

public:
    Show(int id,
         const string& movie,
         int screenId,
         const vector<Seat>& seats,
         const string& timing)

        : showId(id),
          movieName(movie),
          screenId(screenId),
          seats(seats),
          timing(timing) {}

    void displaySeats() {
        for (auto& seat : seats) {
            cout << "Seat ID: "
                 << seat.getSeatId()
                 << endl;
        }
    }

    vector<Seat>& getSeats() {
        return seats;
    }

    string getMovieName() const {
        return movieName;
    }

    string getTiming() const {
        return timing;
    }
};

class Theatre {
private:
    int theatreId;
    string theatreName;
    string location;

    vector<Show> shows;

public:
    Theatre(int id,
            const string& name,
            const string& location)

        : theatreId(id),
          theatreName(name),
          location(location) {}

    void addShow(const Show& show) {
        shows.push_back(show);
    }

    vector<Show>& getShows() {
        return shows;
    }

    string getTheatreName() const {
        return theatreName;
    }

    string getLocation() const {
        return location;
    }
};

class Booking {
private:
    int bookingId;
    User user;
    Theatre theatre;
    Show show;
    vector<Seat*> selectedSeats;
    BookingState state;

public:
    Booking(int id,
            const User& user,
            const Theatre& theatre,
            const Show& show)
        : bookingId(id),
          user(user),
          theatre(theatre),
          show(show),
          state(BookingState::CREATED) {}

    bool addSeat(Seat* seat, int userId) {
        if (!seat->isAvailable()) {
            cout << "Seat already booked/locked\n";
            return false;
        }
        seat->lockSeat();
        selectedSeats.push_back(seat);
        cout << "Seat added successfully\n";
        return true;
    }

    bool isPaymentSuccessful() {
        return true; // mock payment
    }

    bool processPayment() {
        state = BookingState::PAYMENT_PENDING;
        if (isPaymentSuccessful()) {
            for (auto seat : selectedSeats) {
                seat->bookSeat();
            }
            state = BookingState::CONFIRMED;
            cout << "Booking Confirmed\n";
            return true;
        }

        for (auto seat : selectedSeats) {
            seat->releaseSeat();
        }
        state = BookingState::FAILED;
        cout << "Payment Failed\n";
        return false;
    }

    BookingState getState() const {
        return state;
    }
};

class BookingManager {
private:
    int bookingCounter = 1;

public:

    Booking createBooking(User& user,
                      Theatre& theatre,
                      Show& show,
                      vector<Seat*>& seats) {

    Booking booking(
        bookingCounter++,
        user,
        theatre,
        show
    );

    vector<Seat*> lockedSeats;

    for (auto seat : seats) {

        if (!seat->isAvailable()) {

            cout << "Seat "
                 << seat->getSeatId()
                 << " not available\n";

            // rollback previously locked seats
            for (auto s : lockedSeats) {
                s->releaseSeat();
            }

            return booking;
        }

        booking.addSeat(seat, user.getUserId());

        lockedSeats.push_back(seat);
    }

    booking.processPayment();

    return booking;
    }
};

int main() {
    // Create User
    User user1(1, "XYZ");

    // Create Seats
    Seat s1(1, "NORMAL");
    Seat s2(2, "NORMAL");
    Seat s3(3, "VIP");

    // Store seats for show
    vector<Seat> seats = {s1, s2, s3};

    // Create Show
    Show show1(
        101,
        "Interstellar",
        1,
        seats,
        "6:00 PM"
    );

    // Create Theatre
    Theatre theatre1(
        1,
        "PVR Select CityWalk",
        "Delhi"
    );

    theatre1.addShow(show1);

    // Get seats from show
    vector<Seat*> selectedSeats = {
        &show1.getSeats()[0],
        &show1.getSeats()[1]
    };

    // Create Booking Manager
    BookingManager manager;

    // Create Booking
    Booking booking =
        manager.createBooking(
            user1,
            theatre1,
            show1,
            selectedSeats
        );

    // Display final booking state
    if (booking.getState() ==
        BookingState::CONFIRMED) {

        cout << "Booking Successful!" << endl;
    }
    else {
        cout << "Booking Failed!" << endl;
    }

    return 0;
}