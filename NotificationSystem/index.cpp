#include <bits/stdc++.h>
using namespace std;

// ================= ENUM =================
enum class Channel {
    EMAIL,
    SMS,
    PUSH
};

// ================= USER =================
class User {
private:
    int userId;
    string userName;
    set<Channel> channels;

public:
    User(int id, const string& name) : userId(id), userName(name) {}

    void addChannel(Channel channel) {
        channels.insert(channel);
    }

    string getUserName() const {
        return userName;
    }

    int getUserId() const {
        return userId;
    }

    bool isPreferredChannel(Channel channel) const {
        return channels.count(channel) > 0;
    }
};

// ================= NOTIFICATION =================
class Notification {
private:
    string message;
    Channel channel;

public:
    Notification(const string& msg, Channel ch)
        : message(msg), channel(ch) {}

    const string& getMessage() const {
        return message;
    }

    Channel getChannel() const {
        return channel;
    }
};

// ================= STRATEGY =================
class IChannel {
public:
    virtual void send(const string& msg) = 0;
    virtual ~IChannel() = default;
};

class EmailChannel : public IChannel {
public:
    void send(const string& msg) override {
        cout << "[EMAIL] " << msg << endl;
    }
};

class SMSChannel : public IChannel {
public:
    void send(const string& msg) override {
        cout << "[SMS] " << msg << endl;
    }
};

class PushChannel : public IChannel {
public:
    void send(const string& msg) override {
        cout << "[PUSH] " << msg << endl;
    }
};

// ================= FACTORY =================
class ChannelFactory {
public:
    // unique_ptr is a smart pointer from the C++ STL that:
    //    -> owns a dynamically allocated object
    //    ->ensures automatic deletion
    //    ->cannot be copied (only moved)
    // intsead of this (IChannel* ch = new EmailChannel();) used unique_ptr
    static unique_ptr<IChannel> createChannel(Channel channel) {
        switch (channel) {
            case Channel::EMAIL:
                return make_unique<EmailChannel>();
            case Channel::SMS:
                return make_unique<SMSChannel>();
            case Channel::PUSH:
                return make_unique<PushChannel>();
            default:
                return nullptr;
        }
    }
};

// ================= SENDER =================
class NotificationSender {
private:
    vector<User> users;

public:
    void addUser(const User& user) {
        users.push_back(user);
    }

    void send(const Notification& notification) {
        Channel type = notification.getChannel();
        auto channelObj = ChannelFactory::createChannel(type);

        if (!channelObj) return;

        for (const auto& user : users) {
            if (user.isPreferredChannel(type)) {
                string personalizedMsg ="To " + user.getUserName() + ": " + notification.getMessage();

                channelObj->send(personalizedMsg);
            }
        }
    }
};

// ================= MAIN =================
int main() {
    // Create users
    User u1(1, "Alice");
    u1.addChannel(Channel::EMAIL);
    u1.addChannel(Channel::SMS);

    User u2(2, "Bob");
    u2.addChannel(Channel::PUSH);

    User u3(3, "Charlie");
    u3.addChannel(Channel::EMAIL);

    // Create sender
    NotificationSender sender;
    sender.addUser(u1);
    sender.addUser(u2);
    sender.addUser(u3);

    // Send EMAIL notification
    Notification n1("Welcome to the platform!", Channel::EMAIL);
    sender.send(n1);

    cout << "------------------" << endl;

    // Send PUSH notification
    Notification n2("You have a new alert!", Channel::PUSH);
    sender.send(n2);

    return 0;
}