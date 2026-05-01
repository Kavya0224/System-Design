#include<bits/stdc++.h>
using namespace std;

class URL{
    string LongURL;
    string shortURL;
};

class StorageStrategy {
public:
    virtual void save(const string& shortCode, const string& longURL) = 0;
    virtual string getLong(const string& shortCode) = 0;
    virtual bool existsLong(const string& longURL) = 0;
    virtual string getShort(const string& longURL) = 0;

    virtual ~StorageStrategy() {}
};

class URLRepository : public StorageStrategy{
private:
    unordered_map<string, string> shortToLong;
    unordered_map<string, string> longToShort;

public:
    bool existsLong(const string& longURL) override {
        return longToShort.count(longURL);
    }

    string getShort(const string& longURL) override {
        return longToShort[longURL];
    }

    void save(const string& shortCode, const string& longURL) override {
        shortToLong[shortCode] = longURL;
        longToShort[longURL] = shortCode;
    }

    string getLong(const string& shortCode) override {
        if (shortToLong.count(shortCode))
            return shortToLong[shortCode];
        return "Not Found";
    }
};

class Encoder {
private:
    string base62 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

public:
    string encode(long long num) {
        string res = "";
        while (num > 0) {
            res += base62[num % 62];
            num /= 62;
        }
        reverse(res.begin(), res.end());
        return res;
    }
};

class IDGenerator {
private:
    long long counter = 1;

public:
    long long getNextID() {
        return counter++;
    }
};

class URLShortenerService {
private:
    StorageStrategy* storage;
    Encoder encoder;
    IDGenerator generator;

public:
    URLShortenerService(StorageStrategy* storage)
        : storage(storage) {}

    string shortenURL(const string& longURL) {
        if (storage->existsLong(longURL)) {
            return storage->getShort(longURL);
        }

        long long id = generator.getNextID();
        string shortCode = encoder.encode(id);

        storage->save(shortCode, longURL);
        return shortCode;
    }

    string getLongURL(const string& shortCode) {
        return storage->getLong(shortCode);
    }
};

int main() {
    // Create storage strategy
    StorageStrategy* storage = new URLRepository();

    // Create service with injected strategy
    URLShortenerService service(storage);

    // Shorten URLs
    string short1 = service.shortenURL("https://google.com");
    cout << "Short URL 1: " << short1 << endl;

    string short2 = service.shortenURL("https://openai.com");
    cout << "Short URL 2: " << short2 << endl;

    // Duplicate URL check
    string short3 = service.shortenURL("https://google.com");
    cout << "Duplicate URL gives same short: " << short3 << endl;

    // Retrieve original URLs
    cout << "Original URL (short1): " << service.getLongURL(short1) << endl;
    cout << "Original URL (short2): " << service.getLongURL(short2) << endl;

    // Invalid case
    cout << "Invalid lookup: " << service.getLongURL("abc123") << endl;

    // Cleanup
    delete storage;

    return 0;
}