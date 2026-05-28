#include<bits/stdc++.h>
using namespace std;

enum class Status{
    PENDING,
    SUCCESS,
    FAILED
};

class User{
private:
    int id;
    string name;
    string number;

public:
    User(int id,string name,string number)
        : id(id),name(name),number(number){}

    int getUserId() const{
        return id;
    }

    string getUserName() const {
        return name;
    }
};

class Account{
private:
    int accountNumber;
    int balance;
    User user;

public:
    Account(int accNo,int balance,User user)
        : accountNumber(accNo),
          balance(balance),
          user(user) {}

    int getAccountNumber() const{
        return accountNumber;
    }

    int getBalance() const{
        return balance;
    }

    void updateBalance(int newBalance){
        balance = newBalance;
    }

    User getUser() const{
        return user;
    }
};

class Transaction{
private:
    Status status;

public:

    void deposit(Account& account,int amount){
        status = Status::PENDING;
        int newBalance = account.getBalance() + amount;
        account.updateBalance(newBalance);
        status = Status::SUCCESS;
        cout<<"Deposit Successful"<<endl;
    }

    void withdraw(Account& account,int amount){
        status = Status::PENDING;
        if(account.getBalance() < amount){
            status = Status::FAILED;
            cout<<"Not enough balance in your account"<<endl;
            return;
        }
        int newBalance = account.getBalance() - amount;
        account.updateBalance(newBalance);
        status = Status::SUCCESS;
        cout<<"Withdrawal Successful"<<endl;
    }

    void Transfer(Account& payer,Account& receiver,int amount){
        status = Status::PENDING;
        withdraw(payer,amount);
        if(status == Status::FAILED){
            cout<<"Transfer Failed"<<endl;
            return;
        }
        deposit(receiver,amount);
        status = Status::SUCCESS;
        cout<<"Transaction done successfully"<<endl;
    }
};

class Bank{
private:
    unordered_map<int,Account> accounts;

public:

    void addAccount(Account account){
        accounts.insert(
            {
                account.getAccountNumber(),
                account
            }
        );
    }

    void displayAccounts(){
        for(auto it : accounts){
            cout<<"Account No : "<<it.second.getAccountNumber()<<endl;
            cout<<"User : "<<it.second.getUser().getUserName()<<endl;
            cout<<"Balance : "<<it.second.getBalance()<<endl;
            cout<<endl;
        }
    }
};

int main(){

    User user1(
        1,
        "Rohan",
        "9876543210"
    );

    User user2(
        2,
        "Rahul",
        "9999999999"
    );

    Account acc1(
        1001,
        10000,
        user1
    );

    Account acc2(
        1002,
        5000,
        user2
    );

    Bank bank;

    bank.addAccount(acc1);
    bank.addAccount(acc2);

    cout<<"----- ALL ACCOUNTS -----"<<endl;

    bank.displayAccounts();

    Transaction transaction;

    cout<<"----- DEPOSIT -----"<<endl;

    transaction.deposit(acc1,2000);

    cout<<"Balance : "
        <<acc1.getBalance()
        <<endl<<endl;

    cout<<"----- WITHDRAW -----"<<endl;

    transaction.withdraw(acc1,3000);

    cout<<"Balance : "
        <<acc1.getBalance()
        <<endl<<endl;

    cout<<"----- TRANSFER -----"<<endl;

    transaction.Transfer(
        acc1,
        acc2,
        5000
    );

    cout<<"Rohan Balance : "
        <<acc1.getBalance()
        <<endl;

    cout<<"Rahul Balance : "
        <<acc2.getBalance()
        <<endl;

    return 0;
}