#include<bits/stdc++.h>
using namespace std;

enum class Channel {
    EMAIL,
    SMS,
    PUSH
};

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

class Notification {
private:
    string message;
    Channel channel;

public:
    Notification(const string& message, Channel channel)
        : message(message), channel(channel) {}

    string getMessage() const {
        return message;
    }

    Channel getChannel() const {
        return channel;
    }
};