#include<bits/stdc++.h>
using namespace std;

class UserRequestLog {
private:
    int windowStartTime;
    int requestCount;

public:
    UserRequestLog(int windowStartTime)
        : windowStartTime(windowStartTime), requestCount(1) {}

    int getStartTime() const {
        return windowStartTime;
    }

    int getRequestCount() const {
        return requestCount;
    }

    void increment() {
        requestCount++;
    }

    void reset(int newWindowStart) {
        windowStartTime = newWindowStart;
        requestCount = 1;
    }
};

class RateLimiter {
private:
    int maxRequests;
    int windowSize;
    unordered_map<string, UserRequestLog> logs;

public:
    RateLimiter(int maxRequests, int windowSize)
        : maxRequests(maxRequests), windowSize(windowSize) {}

    bool allowRequest(const string& userId, int currentTime) {
        int windowStart = windowSize * (currentTime / windowSize);

        auto it = logs.find(userId);

        if (it == logs.end()) {
            logs.emplace(userId, UserRequestLog(windowStart));
            return true;
        }

        UserRequestLog& log = it->second;

        if (log.getStartTime() != windowStart) {
            log.reset(windowStart);
            return true;
        }

        if (log.getRequestCount() >= maxRequests) {
            return false;
        }

        log.increment();
        return true;
    }
};

int main() {
    RateLimiter limiter(3, 10);

    vector<int> times = {1, 2, 5, 7, 12, 13};

    for (int t : times) {
        if (limiter.allowRequest("user1", t)) {
            cout << "Request at time " << t << " allowed\n";
        } else {
            cout << "Request at time " << t << " blocked\n";
        }
    }

    return 0;
}