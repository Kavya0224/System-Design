#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

enum class Color { WHITE, BLACK };

class Position {
public:
    int row, col;
    Position(int row, int col) : row(row), col(col) {}
    bool isValid() const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
};

class Move {
public:
    Position from, to;
    Move(Position from, Position to) : from(from), to(to) {}
};

class Board;

class Piece {
protected:
    Color color;
public:
    Piece(Color color) : color(color) {}
    virtual ~Piece() {}
    Color getColor() const { return color; }
    virtual char getSymbol() const = 0;
    virtual bool isValidMove(const Board& board, const Move& move) const = 0;
};

class Cell {
private:
    Position position;
    Piece* piece;
public:
    Cell(int row, int col) : position(row, col), piece(nullptr) {}
    Piece* getPiece() const { return piece; }
    void setPiece(Piece* p) { piece = p; }
    bool isEmpty() const { return piece == nullptr; }
};

class Board {
private:
    vector<vector<Cell>> grid;
public:
    Board() {
        for (int i = 0; i < 8; i++) {
            vector<Cell> row;
            for (int j = 0; j < 8; j++) row.emplace_back(i, j);
            grid.push_back(row);
        }
    }

    bool isValidPosition(const Position& pos) const {
        return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
    }

    Piece* getPiece(const Position& pos) const {
        if (!isValidPosition(pos)) return nullptr;
        return grid[pos.row][pos.col].getPiece();
    }

    void setPiece(const Position& pos, Piece* piece) {
        if (isValidPosition(pos)) grid[pos.row][pos.col].setPiece(piece);
    }

    bool isEmpty(const Position& pos) const {
        return getPiece(pos) == nullptr;
    }

    bool hasOpponentPiece(const Position& pos, Color color) const {
        Piece* piece = getPiece(pos);
        return piece != nullptr && piece->getColor() != color;
    }

    bool hasSameColorPiece(const Position& pos, Color color) const {
        Piece* piece = getPiece(pos);
        return piece != nullptr && piece->getColor() == color;
    }

    bool isPathClear(const Position& from, const Position& to) const {
        int rowStep = (to.row - from.row == 0) ? 0 : (to.row - from.row > 0 ? 1 : -1);
        int colStep = (to.col - from.col == 0) ? 0 : (to.col - from.col > 0 ? 1 : -1);

        int r = from.row + rowStep;
        int c = from.col + colStep;

        while (r != to.row || c != to.col) {
            if (!isEmpty(Position(r, c))) return false;
            r += rowStep;
            c += colStep;
        }
        return true;
    }

    void movePiece(const Move& move) {
        Piece* piece = getPiece(move.from);
        setPiece(move.to, piece);
        setPiece(move.from, nullptr);
    }

    void printBoard() const {
        cout << "\n";
        for (int i = 0; i < 8; i++) {
            cout << 8 - i << " ";
            for (int j = 0; j < 8; j++) {
                Piece* piece = grid[i][j].getPiece();
                cout << (piece ? piece->getSymbol() : '.') << " ";
            }
            cout << "\n";
        }
        cout << "  a b c d e f g h\n\n";
    }

    void setupInitialPositions();
};

class Rook : public Piece {
public:
    Rook(Color color) : Piece(color) {}
    char getSymbol() const override { return color == Color::WHITE ? 'R' : 'r'; }
    bool isValidMove(const Board& board, const Move& move) const override {
        if (move.from.row != move.to.row && move.from.col != move.to.col) return false;
        if (board.hasSameColorPiece(move.to, color)) return false;
        return board.isPathClear(move.from, move.to);
    }
};

class Bishop : public Piece {
public:
    Bishop(Color color) : Piece(color) {}
    char getSymbol() const override { return color == Color::WHITE ? 'B' : 'b'; }
    bool isValidMove(const Board& board, const Move& move) const override {
        int dr = abs(move.to.row - move.from.row);
        int dc = abs(move.to.col - move.from.col);
        if (dr != dc) return false;
        if (board.hasSameColorPiece(move.to, color)) return false;
        return board.isPathClear(move.from, move.to);
    }
};

class Knight : public Piece {
public:
    Knight(Color color) : Piece(color) {}
    char getSymbol() const override { return color == Color::WHITE ? 'N' : 'n'; }
    bool isValidMove(const Board& board, const Move& move) const override {
        int dr = abs(move.to.row - move.from.row);
        int dc = abs(move.to.col - move.from.col);
        if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) return false;
        if (board.hasSameColorPiece(move.to, color)) return false;
        return true;
    }
};

class Queen : public Piece {
public:
    Queen(Color color) : Piece(color) {}
    char getSymbol() const override { return color == Color::WHITE ? 'Q' : 'q'; }
    bool isValidMove(const Board& board, const Move& move) const override {
        int dr = abs(move.to.row - move.from.row);
        int dc = abs(move.to.col - move.from.col);
        bool straight = (move.from.row == move.to.row || move.from.col == move.to.col);
        bool diagonal = (dr == dc);
        if (!straight && !diagonal) return false;
        if (board.hasSameColorPiece(move.to, color)) return false;
        return board.isPathClear(move.from, move.to);
    }
};

class King : public Piece {
public:
    King(Color color) : Piece(color) {}
    char getSymbol() const override { return color == Color::WHITE ? 'K' : 'k'; }
    bool isValidMove(const Board& board, const Move& move) const override {
        int dr = abs(move.to.row - move.from.row);
        int dc = abs(move.to.col - move.from.col);
        if (dr > 1 || dc > 1) return false;
        if (board.hasSameColorPiece(move.to, color)) return false;
        return true;
    }
};

class Pawn : public Piece {
public:
    Pawn(Color color) : Piece(color) {}
    char getSymbol() const override { return color == Color::WHITE ? 'P' : 'p'; }
    bool isValidMove(const Board& board, const Move& move) const override {
        int direction = (color == Color::WHITE) ? -1 : 1;
        int startRow = (color == Color::WHITE) ? 6 : 1;

        int dr = move.to.row - move.from.row;
        int dc = abs(move.to.col - move.from.col);

        if (board.hasSameColorPiece(move.to, color)) return false;

        if (dc == 0 && dr == direction && board.isEmpty(move.to)) return true;

        if (dc == 0 && move.from.row == startRow && dr == 2 * direction) {
            Position middle(move.from.row + direction, move.from.col);
            if (board.isEmpty(middle) && board.isEmpty(move.to)) return true;
        }

        if (dc == 1 && dr == direction && board.hasOpponentPiece(move.to, color)) return true;

        return false;
    }
};

void Board::setupInitialPositions() {
    setPiece(Position(0,0), new Rook(Color::BLACK));
    setPiece(Position(0,1), new Knight(Color::BLACK));
    setPiece(Position(0,2), new Bishop(Color::BLACK));
    setPiece(Position(0,3), new Queen(Color::BLACK));
    setPiece(Position(0,4), new King(Color::BLACK));
    setPiece(Position(0,5), new Bishop(Color::BLACK));
    setPiece(Position(0,6), new Knight(Color::BLACK));
    setPiece(Position(0,7), new Rook(Color::BLACK));
    for (int j = 0; j < 8; j++) setPiece(Position(1,j), new Pawn(Color::BLACK));

    setPiece(Position(7,0), new Rook(Color::WHITE));
    setPiece(Position(7,1), new Knight(Color::WHITE));
    setPiece(Position(7,2), new Bishop(Color::WHITE));
    setPiece(Position(7,3), new Queen(Color::WHITE));
    setPiece(Position(7,4), new King(Color::WHITE));
    setPiece(Position(7,5), new Bishop(Color::WHITE));
    setPiece(Position(7,6), new Knight(Color::WHITE));
    setPiece(Position(7,7), new Rook(Color::WHITE));
    for (int j = 0; j < 8; j++) setPiece(Position(6,j), new Pawn(Color::WHITE));
}

class Player {
private:
    string name;
    Color color;
public:
    Player(string name, Color color) : name(name), color(color) {}
    string getName() const { return name; }
    Color getColor() const { return color; }
};

class Game {
private:
    Board board;
    Player whitePlayer;
    Player blackPlayer;
    Color currentTurn;

public:
    Game(string whiteName, string blackName)
        : whitePlayer(whiteName, Color::WHITE),
          blackPlayer(blackName, Color::BLACK),
          currentTurn(Color::WHITE) {
        board.setupInitialPositions();
    }

    Player& getCurrentPlayer() {
        return currentTurn == Color::WHITE ? whitePlayer : blackPlayer;
    }

    void switchTurn() {
        currentTurn = currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
    }

    bool makeMove(const Move& move) {
        Piece* piece = board.getPiece(move.from);

        if (piece == nullptr) {
            cout << "No piece at source\n";
            return false;
        }

        if (piece->getColor() != currentTurn) {
            cout << "Wrong turn\n";
            return false;
        }

        if (!piece->isValidMove(board, move)) {
            cout << "Invalid move\n";
            return false;
        }

        board.movePiece(move);
        switchTurn();
        return true;
    }

    void start() {
        while (true) {
            board.printBoard();
            Player& currentPlayer = getCurrentPlayer();
            cout << currentPlayer.getName() << "'s turn\n";

            int fr, fc, tr, tc;
            cin >> fr >> fc >> tr >> tc;

            Move move(Position(fr, fc), Position(tr, tc));
            makeMove(move);
        }
    }
};

int main() {
    Game game("Alice", "Bob");
    game.start();
    return 0;
}