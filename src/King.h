#pragma once
#include "Piece.h"

class King :
    public Piece
{
public:
    King(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // calls type and color to debug
    void sayMyName() override;

    // calculates the possible moves
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck) override;

    // controls whether this piece is checked or not
    void setCheck(Piece* field[8][8], int x, int y) override;

    // get check
    bool getCheck() override;

private:

    // true, if the king is on any of the other teams movelist's
    bool m_check;
};

