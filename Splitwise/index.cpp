#include<bits/stdc++.h>
using namespace std;

class User{
private:
    int UserId;
    string UserName;
public:
    User(int id,string name) : UserId(id),UserName(name) {};

    string getName() const {
        return UserName;
    }

    int getUserId() const {
        return UserId;
    }
};

class Expense {
private:
    int amount;
    User paidBy;
    vector<User> participants;

public:
    Expense(int amount, const User& user) : amount(amount), paidBy(user) {
        participants.push_back(user);
    }

    void addParticipant(const User& user) {
        participants.push_back(user);
    }

    int getAmount() const {
        return amount;
    }

    User getPaidBy() const {
        return paidBy;
    }

    vector<User> getParticipants() const {
        return participants;
    }
};

class Transaction {
private:
    int fromUserId;
    int toUserId;
    int amount;

public:
    Transaction(int from, int to, int amount)
        : fromUserId(from), toUserId(to), amount(amount) {}

    int getFromUserId() const {
        return fromUserId;
    }

    int getToUserId() const {
        return toUserId;
    }

    int getAmount() const {
        return amount;
    }

    void printTransaction() const {
        cout << fromUserId << " owes " << amount << " to " << toUserId << endl;
    }
};

class Splitwise {
private:
    unordered_map<int, User> users;
    unordered_map<int, unordered_map<int, int>> balances;
    vector<Transaction> transactions;

public:
    void addUser(const User& user) {
        users.emplace(user.getUserId(), user);
    }

    void addExpense(const Expense& expense) {
        int paidBy = expense.getPaidBy().getUserId();
        vector<User> participants = expense.getParticipants();
        int totalAmount = expense.getAmount();

        int n = participants.size();
        if (n == 0) return;

        int share = totalAmount / n;

        for (const auto& user : participants) {
            int userId = user.getUserId();

            if (userId == paidBy) continue;

            balances[paidBy][userId] += share;
            balances[userId][paidBy] -= share;

            transactions.emplace_back(userId, paidBy, share);
        }
    }

    void settleUp(const User& fromUser, const User& toUser, int amount) {
        int debtor = fromUser.getUserId();
        int creditor = toUser.getUserId();
            if (balances[creditor][debtor] < amount) {
                cout << "Settlement amount exceeds pending balance" << endl;
                return;
            }

            balances[creditor][debtor] -= amount;
            balances[debtor][creditor] += amount;

            transactions.emplace_back(debtor, creditor, amount);
        }

    void showBalances() const {
        for (const auto& p1 : balances) {
            int creditor = p1.first;
            for (const auto& p2 : p1.second) {
                int debtor = p2.first;
                int amount = p2.second;

                if (amount > 0) {
                    cout << debtor << " owes " << amount
                         << " to " << creditor << endl;
                }
            }
        }
    }
    void showTransactions() const {
        if (transactions.empty()) {
            cout << "No transactions found" << endl;
            return;
        }

        for (const auto& t : transactions) {
            t.printTransaction();
        }
    }
};

int main(){
    User user1(1,"abc");
    User user2(2,"xyz");
    User user3(3,"efg");
    Expense expense(3000,user1);
    expense.addParticipant(user2);
    expense.addParticipant(user3);
    Splitwise splitwise;
    splitwise.addUser(user1);
    splitwise.addUser(user2);
    splitwise.addUser(user3);
    splitwise.addExpense(expense);
    splitwise.showBalances();
    splitwise.settleUp(user2,user1,500);
    splitwise.showBalances();
    splitwise.showTransactions();
}