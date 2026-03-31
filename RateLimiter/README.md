# Rate Limiter System - Low Level Design

## Overview

This project is a simplified **Rate Limiter System** implemented in **C++** to control the number of requests a user can make within a fixed time window.

The system supports:

* limiting requests per user
* allowing at most **N requests in T seconds**
* blocking requests beyond the limit
* handling multiple users independently

---

## Functional Requirements

* A user can send requests to the system
* The system allows at most **N requests in T seconds**
* If a user exceeds the limit, further requests are blocked for that window
* Rate limiting is applied **per user**

---

## Core Components

### 1. UserRequestLog

Stores request data for a single user within a time window.

**Fields:**

* `windowStartTime`
* `requestCount`

**Responsibilities:**

* track the start of the current window
* track number of requests in the current window
* reset or increment request count

---

### 2. RateLimiter

Acts as the main controller of the system.

**Fields:**

* `maxRequests`
* `windowSize`
* `unordered_map<string, UserRequestLog> logs`

**Responsibilities:**

* check whether a request is allowed
* maintain per-user logs
* enforce rate limiting rules

---

## Algorithm Used

### Fixed Window Algorithm

The system divides time into fixed intervals (windows).

Formula:

```cpp
windowStart = windowSize * (currentTime / windowSize);
```

### Example

If:

* `windowSize = 10`
* requests at times: `1, 2, 5, 7`

Then all belong to:

```
window [0–9]
```

If next request comes at:

```
time = 12
```

Then new window:

```
[10–19]
```

---

## Flow

### Request Handling

For each request:

1. Compute window start:

   ```cpp
   windowStart = windowSize * (currentTime / windowSize);
   ```

2. If user is new:

   * create new log
   * allow request

3. If window changed:

   * reset log
   * allow request

4. If within same window:

   * if request count < maxRequests → allow
   * else → block

---

## Example

Limit:

```
maxRequests = 3
windowSize = 10
```

Requests:

```
1 → allowed
2 → allowed
5 → allowed
7 → blocked
12 → allowed (new window)
```

---

## Key Design Decisions

* Used **fixed window bucketing** for simplicity
* Stored per-user logs using `unordered_map`
* Separated:

  * **data (UserRequestLog)**
  * **logic (RateLimiter)**
* Used integer time for simplicity (can be extended to timestamps)

---

## Design Patterns Used / Observed

### 1. Facade / Controller

`RateLimiter` acts as the main entry point that:

* receives requests
* applies logic
* manages user logs

---

## Design Patterns That Can Be Added Later

### 1. Strategy Pattern (IMPORTANT)

Different rate limiting algorithms can be implemented:

* Fixed Window (current)
* Sliding Window Log
* Sliding Window Counter
* Token Bucket
* Leaky Bucket

Can be designed as:

```cpp
class RateLimitStrategy {
    virtual bool allowRequest(...) = 0;
};
```

This makes the system:

* extensible
* modular
* production-ready

---

### 2. Decorator Pattern

Can be used to wrap APIs with rate limiting logic.

---

### 3. Middleware Pattern

In real systems (backend), rate limiter is often implemented as middleware.

---

## Edge Cases Considered

* new user request
* window boundary reset
* exceeding max requests
* multiple users independently tracked
* rapid burst requests

---

## Limitations

* Fixed Window allows burst at window boundaries
  (e.g., many requests at end of one window and start of next)

---

## Possible Improvements

* implement Sliding Window for smoother rate limiting
* support distributed rate limiting
* integrate with Redis for shared state
* use timestamps instead of integers
* add per-endpoint rate limiting
* support global rate limits

---

## Learning Outcome

This project demonstrates:

* time-based request control
* efficient data structure usage
* separation of logic and state
* system design for scalability
* trade-offs between simplicity and accuracy

---

## Tech Stack

* Language: C++
* Concepts: OOP, STL, LLD

---

## Key Takeaway

A Rate Limiter is not just about counting requests —
it is about:

* time window modeling
* fairness
* performance vs accuracy trade-offs
* extensibility using better algorithms
