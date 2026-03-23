#include<bits/stdc++.h>
using namespace std;

class Book{
private:
    int bookId;
    string bookName;
    bool available;

public:
    Book(int id,const string& name) : bookId(id), bookName(name), available(true) {}

    int getBookId() const {
        return bookId;
    }

    string getBookName() const {
        return bookName;
    }

    bool isAvailable() const {
        return available;
    }

    void markIssued() {
        available = false;
    }

    void markReturned() {
        available = true;
    }
};

class User{
private:
    string userName;
    int userId;
public:
    User(const string& name,int id) : userName(name),userId(id) {};
    string getUserName() const{
        return userName;
    }
    int getUserId() const{
        return userId;
    }
};
class IssueRecord {
private:
    int userId;
    int bookId;
    string issueDate;
    string returnDate;

public:
    IssueRecord(int userId, int bookId, const string& issueDate)
        : userId(userId), bookId(bookId), issueDate(issueDate), returnDate("") {}

    int getBookId() const {
        return bookId;
    }

    int getUserId() const {
        return userId;
    }

    string getIssueDate() const {
        return issueDate;
    }

    string getReturnDate() const {
        return returnDate;
    }

    void markReturned(const string& date) {
        returnDate = date;
    }

    bool isActive() const {
        return returnDate.empty();
    }
};

class Library {
private:
    unordered_map<int, Book> books;
    unordered_map<int, User> users;
    vector<IssueRecord> records;

public:
    void addBook(const Book& book) {
        int id = book.getBookId();
        if (books.find(id) == books.end()) {
            books.emplace(id, book);
        } else {
            cout << "Book already present" << endl;
        }
    }

    void addUser(const User& user) {
        int id = user.getUserId();
        if (users.find(id) == users.end()) {
            users.emplace(id, user);
        } else {
            cout << "User already exists" << endl;
        }
    }

    void issueBook(int userId, int bookId, const string& date) {
    if (users.find(userId) == users.end()) {
        cout << "User does not exist" << endl;
        return;
    }

    auto it = books.find(bookId);
    if (it == books.end()) {
        cout << "Book does not exist" << endl;
        return;
    }

    if (!it->second.isAvailable()) {
        cout << "Book not available" << endl;
        return;
    }

    IssueRecord issueRecord(userId, bookId, date);
    records.push_back(issueRecord);
    it->second.markIssued();

    cout << "Book issued successfully to" << endl;
    }
    void returnBook(int userId, int bookId, const string& date) {
        if (users.find(userId) == users.end()) {
            cout << "User does not exist" << endl;
            return;
        }

        if (books.find(bookId) == books.end()) {
            cout << "Invalid bookId" << endl;
            return;
        }

        for (auto& record : records) {
            if (record.getUserId() == userId &&
                record.getBookId() == bookId &&
                record.isActive()) {

                record.markReturned(date);
                auto it = books.find(bookId);
                if (it != books.end()) {
                    it->second.markReturned();
                }
                cout << "Book returned successfully " <<endl;
                return;
            }
        }

        cout << "No active issue record found" << endl;
    }
};
int main(){
    Book book1(1,"abc");
    Book book2(2,"xyz");
    User user1("Bhavya",1);
    User user2("Manu",2);
    Library library;
    library.addBook(book1);
    library.addBook(book2);
    library.addUser(user1);
    library.addUser(user2);
    library.issueBook(1,1,"23-03-2026");
    library.issueBook(2,1,"23-03-2026");
    library.returnBook(1,1,"25-03-2026");
    return 0;
}