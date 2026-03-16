#include<bits/stdc++.h>
using namespace std;
enum class Symbol{
    EMPTY,
    X,O
};
class Player{
    private:
       string name;
       Symbol symbol;
    public:
       Player(string name,Symbol symbol){
        this->name=name;
        this->symbol=symbol;
       }
       string getName(){
        return name;
       }
       Symbol getSymbol(){
        return symbol;
       }
};

class Move{
    private:
       int row;
       int col;
       Symbol symbol;
    public:
       Move(int row,int col,Symbol symbol){
        this->row=row;
        this->col=col;
        this->symbol=symbol;
       }
       int getRow(){
        return row;
       }
       int getCol(){
        return col;
       }
       Symbol getSymbol(){
        return symbol;
       }
};

class Board{
    private:
       int size;
       vector<vector<Symbol>> grid;
       char symbolToChar(Symbol symbol){
        if(symbol==Symbol::X) return 'X';
        if(symbol==Symbol::O) return 'O';
        return ' ';
       }
    public:
        Board(int n=3){
            this->size=n;
            grid.resize(n,vector<Symbol>(n,Symbol::EMPTY));
        }
        int getSize(){
            return size;
        }

        bool isValidMove(int row,int col){
            if(row<0 || col<0 || row>=size || col>=size) return false;
            return grid[row][col]==Symbol::EMPTY;
        }

        bool placeMove(Move& move){
            int row=move.getRow();
            int col=move.getCol();
            if(!isValidMove(row,col)) return false;
            grid[row][col]=move.getSymbol();
            return true;
        }

        bool checkWinner(Symbol symbol){
            for (int i = 0; i < size; i++) {
            bool win = true;
            for (int j = 0; j < size; j++) {
                if (grid[i][j] != symbol) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        // Check columns
        for (int j = 0; j < size; j++) {
            bool win = true;
            for (int i = 0; i < size; i++) {
                if (grid[i][j] != symbol) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        // Check main diagonal
        bool win = true;
        for (int i = 0; i < size; i++) {
            if (grid[i][i] != symbol) {
                win = false;
                break;
            }
        }
        if (win) return true;

        // Check anti-diagonal
          win = true;
          for (int i = 0; i < size; i++) {
            if (grid[i][size - i - 1] != symbol) {
                win = false;
                break;
            }
          }
          return win;
        }

        bool isFull(){
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    if(grid[i][j]==Symbol::EMPTY) return false;
                }
            }
            return true;
        }

        void printBoard(){
        cout << "\nBoard:\n";
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << " " << symbolToChar(grid[i][j]) << " ";
                if (j < size - 1) cout << "|";
            }
            cout << "\n";
            if (i < size - 1) {
                for (int j = 0; j < size; j++) {
                    cout << "---";
                    if (j < size - 1) cout << "+";
                }
                cout << "\n";
            }
        }
        cout << "\n";
    }

};

class Game{
public:
    Board board;
    Player player1;
    Player player2;
    int currentPlayerIndex;
public:
    Game(string p1Name, string p2Name, int boardSize = 3)
    : board(boardSize),
      player1(p1Name, Symbol::X),
      player2(p2Name, Symbol::O),
      currentPlayerIndex(0) {}

    Player& getCurrentPlayer(){
        return (currentPlayerIndex==0)?player1:player2;
    }

    void switchTurn(){
        currentPlayerIndex=1-currentPlayerIndex;
    }

    void start(){
        cout<<"Starting TIC-TAC-TOE"<<endl;
        cout<<player1.getName()<<" is X"<<endl;
        cout<<player2.getName()<<" is O"<<endl;

        while(true){
            board.printBoard();
            Player& currentPlayer=getCurrentPlayer();
            int row,col;
            cout<<currentPlayer.getName()<<" turn: ";
            cin>>row>>col;

            Move move(row,col,currentPlayer.getSymbol());
            if(!board.placeMove(move)){
                cout<<"Invalid move"<<endl;
                continue;
            }
            if(board.checkWinner(currentPlayer.getSymbol())){
                board.printBoard();
                cout<<currentPlayer.getName()<<"wins"<<endl;
                break;
            }
            if(board.isFull()){
                board.printBoard();
                cout<<"Draw"<<endl;
                break;
            }
            switchTurn();
        }
    }

};
int main(){
    Game game("Alice","Bob",3);
    game.start();
    return 0;
}