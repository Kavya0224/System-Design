#include<bits/stdc++.h>
using namespace std;

class URLShortner{
private:
    unordered_map<string,string> longToshort;
    unordered_map<string,string> shortTolong;
    
    long long counter = 1;
    string base62 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    string encode(int code){
        string res="";
        while(code){
            res+=base62[code%62];
            code/=62;
        }
        return res;
    }
public:
    string shortnerService(string orgURL){
        if (longToshort.count(orgURL)) {
            return longToshort[orgURL];
        }

        string shortURL=encode(counter++);
        longToshort[orgURL]=shortURL;
        shortTolong[shortURL]=orgURL;

        return shortURL;
    }

    string getLongURL(string  URL){
        if (shortTolong.count(URL)) {
            return shortTolong[URL];
        }
        return "Not Found";
    }

};

int main(){
    URLShortner service;

    string short1 = service.shortnerService("https://google.com");
    cout << "Short URL: " << short1 << endl;

    string original = service.getLongURL(short1);
    cout << "Original URL: " << original << endl;

    string short2 = service.shortnerService("https://google.com");
    cout << "Same URL gives same short: " << short2 << endl;

    return 0;


}