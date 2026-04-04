#include <bits/stdc++.h>
using namespace std;

class RateLimitingStrategy {
public:
    virtual bool allowRequest(const string& userId, int currentTime) = 0;
    virtual ~RateLimitingStrategy() {}
};

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

class FixedWindowStrategy : public RateLimitingStrategy {
private:
    int maxRequests;
    int windowSize;
    unordered_map<string, UserRequestLog> logs;

public:
    FixedWindowStrategy(int maxRequests, int windowSize)
        : maxRequests(maxRequests), windowSize(windowSize) {}

    bool allowRequest(const string& userId, int currentTime) override {
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

class RateLimiter {
private:
    RateLimitingStrategy* strategy;

public:
    RateLimiter(RateLimitingStrategy* strategy)
        : strategy(strategy) {}

    void setStrategy(RateLimitingStrategy* newStrategy) {
        strategy = newStrategy;
    }

    bool allowRequest(const string& userId, int currentTime) {
        return strategy->allowRequest(userId, currentTime);
    }
};

int main() {
    RateLimitingStrategy* strategy = new FixedWindowStrategy(3, 10);
    RateLimiter limiter(strategy);

    vector<int> times = {1, 2, 5, 7, 12, 13};

    for (int t : times) {
        if (limiter.allowRequest("user1", t)) {
            cout << "Request at time " << t << " allowed\n";
        } else {
            cout << "Request at time " << t << " blocked\n";
        }
    }

    delete strategy;
    return 0;
}