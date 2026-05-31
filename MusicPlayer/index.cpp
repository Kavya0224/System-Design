#include<bits/stdc++.h>
using namespace std;

class User{
private:
    int userId;
    string userName;

public:
    User(int id,string name)
        : userId(id),
          userName(name) {}

    int getUserId() const{
        return userId;
    }

    string getUserName() const{
        return userName;
    }
};

class Song{
private:
    int songId;
    string songName;
    string duration;

public:
    Song(){}
    Song(int id, string name, string duration)
        : songId(id),
          songName(name),
          duration(duration) {}

    string getSongName() const{
        return songName;
    }

    string getSongDuration() const{
        return duration;
    }

    int getSongId() const{
        return songId;
    }
};

class Playlist{
private:
    unordered_map<int,Song> playlist;
public:
    void addSong(Song song){
        int id = song.getSongId();
        playlist.insert({id,song});
        cout<<"Song successfully added to playlist"<<endl;
    }

    void deleteSong(Song song){
        int id = song.getSongId();
        playlist.erase(id);
        cout<<"Song successfully deleted from playlist"<<endl;
    }

    void displaySongs(){
        cout<<"Playlist Songs"<<endl;
        for(auto& it:playlist){
            Song song = it.second;
            cout<<song.getSongName()
                <<" "
                <<song.getSongDuration()
                <<endl;
        }
    }
};

class MusicPlayer{
private:
    vector<Song> songs;
    int currentIndex;
    bool isPlaying;
    bool repeatMode;

public:
    MusicPlayer()
        : currentIndex(-1),
          isPlaying(false),
          repeatMode(false) {}

    void addSong(Song song){
        songs.push_back(song);
    }

    void playSong(int index){
        if(index < 0 || index >= songs.size()){
            cout<<"Invalid Song"<<endl;
            return;
        }
        currentIndex = index;
        isPlaying = true;
        cout<<"Playing : "
            <<songs[currentIndex].getSongName()
            <<endl;
    }

    void pauseSong(){
        if(!isPlaying){
            cout<<"No song is playing"<<endl;
            return;
        }
        isPlaying = false;
        cout<<"Song Paused"<<endl;
    }

    void nextSong(){
        if(songs.empty()){
            cout<<"No songs available"<<endl;
            return;
        }

        if(repeatMode){
            cout<<"Repeating : "
                <<songs[currentIndex].getSongName()
                <<endl;
            return;
        }

        if(currentIndex == songs.size()-1){
            cout<<"Last song reached"<<endl;
            return;
        }
        currentIndex++;
        isPlaying = true;
        cout<<"Playing : "
            <<songs[currentIndex].getSongName()
            <<endl;
    }

    void previousSong(){
        if(songs.empty()){
            cout<<"No songs available"<<endl;
            return;
        }

        if(currentIndex <= 0){
            cout<<"Already at first song"<<endl;
            return;
        }
        currentIndex--;
        isPlaying = true;
        cout<<"Playing : "
            <<songs[currentIndex].getSongName()
            <<endl;
    }

    void searchSong(string songName){
        bool found = false;
        for(auto song : songs){
            if(song.getSongName() == songName){
                cout<<"Song Found"<<endl;
                cout<<"Name : "
                    <<song.getSongName()
                    <<endl;

                cout<<"Duration : "
                    <<song.getSongDuration()
                    <<endl;
                found = true;
                break;
            }
        }

        if(!found){
            cout<<"Song Not Found"<<endl;
        }
    }

    void shufflePlay(){
        if(songs.empty()){
            cout<<"No songs available"<<endl;
            return;
        }

        int index = rand() % songs.size();
        currentIndex = index;
        isPlaying = true;
        cout<<"Shuffle Playing : "
            <<songs[currentIndex].getSongName()
            <<endl;
    }

    void enableRepeat(){
        repeatMode = true;
        cout<<"Repeat Mode ON"<<endl;
    }

    void disableRepeat(){
        repeatMode = false;
        cout<<"Repeat Mode OFF"<<endl;
    }

    void displayAllSongs(){
        cout<<"All Songs"<<endl;
        for(int i=0;i<songs.size();i++){
            cout<<i
                <<" "
                <<songs[i].getSongName()
                <<" "
                <<songs[i].getSongDuration()
                <<endl;
        }
    }
};

int main(){

    User user(
        1,
        "XYZ"
    );

    Song s1(
        101,
        "Believer",
        "3:20"
    );

    Song s2(
        102,
        "Shape Of You",
        "4:10"
    );

    Song s3(
        103,
        "Perfect",
        "4:30"
    );

    Playlist playlist;

    playlist.addSong(s1);
    playlist.addSong(s2);
    playlist.addSong(s3);

    cout<<endl;

    playlist.displaySongs();

    cout<<endl;

    MusicPlayer player;

    player.addSong(s1);
    player.addSong(s2);
    player.addSong(s3);

    player.displayAllSongs();

    cout<<endl;

    player.playSong(0);

    player.nextSong();

    player.nextSong();

    player.previousSong();

    player.pauseSong();

    player.searchSong("Believer");

    player.shufflePlay();

    player.enableRepeat();

    player.nextSong();

    player.disableRepeat();

    player.nextSong();

    return 0;
}