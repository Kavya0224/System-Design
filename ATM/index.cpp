#include<bits/stdc++.h>
using namespace std;

class Account {
private:
    string userName;
    string accountNumber;
    int pin;
    double balance;

public:
    Account(string name, string accNo, int pin, double balance)
        : userName(name), accountNumber(accNo), pin(pin), balance(balance) {}

    string getUserName() const {
        return userName;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    bool validatePin(int enteredPin) const {
        return pin == enteredPin;
    }

    double getBalance() const {
        return balance;
    }

    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    void deposit(double amount) {
        balance += amount;
    }
};

class Card { 
private:
    string cardNumber;
    string accountNumber;

public:
    Card(string cardNumber, string accountNumber)
        : cardNumber(cardNumber), accountNumber(accountNumber) {}

    string getCardNumber() const {
        return cardNumber;
    }

    string getAccountNumber() const {
        return accountNumber;
    }
};

class Bank {
private:
    unordered_map<string, Account> accounts;

public:
    void addAccount(const Account& acc) {
        accounts.emplace(acc.getAccountNumber(), acc);
    }

    Account* getAccount(const string& accNo) {
        auto it = accounts.find(accNo);
        if (it != accounts.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    bool validatePin(const string& accNo, int pin) {
        Account* acc = getAccount(accNo);
        if (!acc) return false;
        return acc->validatePin(pin);
    }

    bool withdraw(const string& accNo, double amount) {
        Account* acc = getAccount(accNo);
        if (!acc) return false;
        return acc->withdraw(amount);
    }

    void deposit(const string& accNo, double amount) {
        Account* acc = getAccount(accNo);
        if (acc) acc->deposit(amount);
    }
};

class ATM {
private:
    Bank& bank;
    Card* currentCard;
    bool isAuthenticated;

public:
    ATM(Bank& bank) : bank(bank), currentCard(nullptr), isAuthenticated(false) {}

    void insertCard(Card& card) {
        currentCard = &card;
        isAuthenticated = false;
        cout << "Card inserted\n";
    }

    bool authenticate(int pin) {
        if (currentCard == nullptr) return false;

        string accountNo = currentCard->getAccountNumber();
        isAuthenticated = bank.validatePin(accountNo, pin);
        return isAuthenticated;
    }

    void checkBalance() {
        if (currentCard == nullptr || !isAuthenticated) {
            cout << "Authentication required\n";
            return;
        }

        string accountNo = currentCard->getAccountNumber();
        Account* acc = bank.getAccount(accountNo);
        if (acc) {
            cout << "Balance: " << acc->getBalance() << "\n";
        }
    }

    void withdraw(double amount) {
        if (currentCard == nullptr || !isAuthenticated) {
            cout << "Authentication required\n";
            return;
        }

        string accountNo = currentCard->getAccountNumber();
        if (bank.withdraw(accountNo, amount)) {
            cout << "Withdrawal successful\n";
        } else {
            cout << "Insufficient balance\n";
        }
    }

    void deposit(double amount) {
        if (currentCard == nullptr || !isAuthenticated) {
            cout << "Authentication required\n";
            return;
        }

        string accountNo = currentCard->getAccountNumber();
        bank.deposit(accountNo, amount);
        cout << "Deposit successful\n";
    }

    void ejectCard() {
        currentCard = nullptr;
        isAuthenticated = false;
        cout << "Card ejected\n";
    }
};

int main() {
    Account acc1("abc", "123412341234", 1234, 9999);
    Account acc2("xyz", "987698769876", 9876, 9999);

    Bank bank;
    bank.addAccount(acc1);
    bank.addAccount(acc2);

    Card card1("12340000", "123412341234");
    Card card2("00009876", "987698769876");

    ATM atm(bank);

    atm.insertCard(card1);

    if (atm.authenticate(1234)) {
        atm.checkBalance();
        atm.withdraw(1000);
        atm.checkBalance();
    } else {
        cout << "Invalid PIN\n";
    }

    atm.ejectCard();

    return 0;
}