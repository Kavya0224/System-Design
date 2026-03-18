#include<bits/stdc++.h>
using namespace std;
class Player {
private:
    int playerId;
    int position;

public:
    Player(int id) : playerId(id), position(1) {}

    int getPosition() const {
        return position;
    }

    void setPosition(int pos) {
        position = pos;
    }

    int getId() const {
        return playerId;
    }
};

class Board {
private:
    int size;
    unordered_map<int, int> snake;
    unordered_map<int, int> ladder;

public:
    Board(int sz) : size(sz) {}

    void setLadder(int start, int end) {
        ladder[start] = end;
    }

    void setSnake(int start, int end) {
        snake[start] = end;
    }

    int getFinalPosition(int pos) {
        // if ladder exists
        if (ladder.count(pos)) {
            return ladder[pos];
        }
        // if snake exists
        if (snake.count(pos)) {
            return snake[pos];
        }
        return pos;
    }

    int getSize() const {
        return size;
    }
};

class Dice{
public:
    int rollDice(){
        return rand()%6+1;
    }
};

class Game {
private:
    vector<Player> players;
    Board board;
    Dice dice;
    int currentPlayerIndex;

public:
    Game(const vector<Player>& players, const Board& board)
        : players(players), board(board), currentPlayerIndex(0) {}

    void startGame() {
        while (true) {
            Player& player = players[currentPlayerIndex];

            int roll = dice.rollDice();
            cout << "Player " << player.getId() << " rolled: " << roll << "\n";

            int currentPos = player.getPosition();
            int newPos = currentPos + roll;

            if (newPos <= board.getSize()) {
                newPos = board.getFinalPosition(newPos);
                player.setPosition(newPos);
            }

            cout << "Player " << player.getId() << " is now at: "
                 << player.getPosition() << "\n";

            if (player.getPosition() == board.getSize()) {
                cout << "Player " << player.getId() << " wins!\n";
                break;
            }

            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        }
    }
};
int main() {
    srand(time(0));

    Board board(100);

    board.setLadder(4, 14);
    board.setLadder(9, 31);
    board.setLadder(20, 38);

    board.setSnake(17, 7);
    board.setSnake(54, 34);
    board.setSnake(62, 19);

    Player player1(1);
    Player player2(2);

    vector<Player> players = {player1, player2};

    Game game(players, board);
    game.startGame();

    return 0;
}