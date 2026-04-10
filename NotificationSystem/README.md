# 🚀 Notification System (C++)

A scalable and extensible **Notification System** built in C++ using modern design principles and design patterns.  
This project demonstrates how to design a flexible system that can send notifications via multiple channels like **Email, SMS, and Push**.

---

## 📌 Features

- Send notifications based on user preferences  
- Supports multiple channels:
  - 📧 Email  
  - 📱 SMS  
  - 🔔 Push Notifications  
- Easily extensible for new channels (e.g., WhatsApp, Slack)  
- Clean and modular design using OOP principles  

---

## 🧠 Design Overview

The system is built using:

### 🔹 Strategy Pattern

Defines a common interface for all notification channels:

- `IChannel` → abstract interface  
- `EmailChannel`, `SMSChannel`, `PushChannel` → implementations  

### 🔹 Factory Pattern

Responsible for creating channel objects dynamically:

- `ChannelFactory`

---

## 🔹 Core Components

| Component | Responsibility |
|---------- |----------------|
| `User`               | Stores user info and preferred channels |
| `Notification`       | Holds message + channel type |
| `IChannel`           | Interface for sending messages |
| `ChannelFactory`     | Creates channel objects |
| `NotificationSender` | Sends notifications to users |

---


---

## 💡 Key Concepts Used

- Object-Oriented Design (Encapsulation, Abstraction)  
- Design Patterns:
  - Strategy Pattern  
  - Factory Pattern  
- Smart Pointers (`unique_ptr`)  
- Clean and maintainable code practices  

---

## 🔧 How It Works

1. Create users and assign preferred channels  
2. Create a notification with a specific channel  
3. `NotificationSender`:
   - Filters users based on preference  
   - Uses `ChannelFactory` to create channel  
   - Sends message via selected strategy  

---

## ▶️ Example Usage

```cpp
User u1(1, "Alice");
u1.addChannel(Channel::EMAIL);

Notification n("Welcome!", Channel::EMAIL);

NotificationSender sender;
sender.addUser(u1);
sender.send(n);

⚡ Future Improvements
-Async notification processing (Kafka / RabbitMQ)
-Retry mechanism for failed messages
-Rate limiting per user
-Logging & monitoring
-Channel prioritization

🏗️ Project Structure
notification-system/
│── main.cpp
│── User.h
│── Notification.h
│── Channel.h
│── ChannelFactory.h
│── NotificationSender.h


🤝 Contribution
Feel free to fork and improve the project! Suggestions and improvements are welcome.
---

If you want to make it **even stronger for GitHub/recruiters**, I can:
- Add **badges (build, license, stars)** ⭐  
- Add **GIF/demo output** 🎥  
- Add **UML diagrams** 📊 (big impact)