#include "King.h"
#include "Pawn.h"
#include <iostream>
#include <stdlib.h>

King::King(Team team, std::pair<int, int> pos, SDL_Handler* handler)
	:Piece(team, pos, handler, KING), m_check(false)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/Chess_kdt60.png";
	}
	else
	{
		filename = "../res/Chess_klt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	render();
}

void King::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK KING" << std::endl;
	}
	else
	{
		std::cout << "WHTIE KING" << std::endl;
	}
}

void King::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple <int, int, Piece::MoveType>> moves;
	bool enemyPlace = false;

	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (m_pos.first + dx >= 0 && m_pos.first + dx <= 7 && m_pos.second + dy >= 0 && m_pos.second + dy <= 7)
			{
				if (field[m_pos.first + dx][m_pos.second + dy] != nullptr)
				{
					if (field[m_pos.first + dx][m_pos.second + dy]->getTeam() != m_team)
					{
						moves = pushMove(moves,
							std::tuple<int, int, Piece::MoveType>(m_pos.first + dx, m_pos.second + dy, Piece::NORMAL),
							getOwnKing(field),
							field,
							checkCheck);
					}
				}
				else
				{
					moves = pushMove(moves,
									 std::tuple<int, int, Piece::MoveType>(m_pos.first + dx, m_pos.second + dy, Piece::NORMAL),
									 getOwnKing(field),
									 field,
									 checkCheck);
				}
			}
		}
	}

	if (!m_hasMoved)
	{
		for (int i = 0; i <= 7; i += 7)
		{
			for (int j = 0; j <= 7; j += 7)
			{
				if (field[i][j] != nullptr)
				{
					if (field[i][j]->getTeam() == m_team && field[i][j]->getType() == ROOK && !field[i][j]->m_hasMoved)
					{
						int a, b, c;
						if (i == 0)
						{
							a = 1;
							b = 2;
							c = 2;
						}
						else
						{
							a = 4;
							b = 5;
							c = 6;
						}
						if (field[a][j] == nullptr && field[b][j] == nullptr && field[c][j] == nullptr)
						{
							moves.push_back(std::tuple<int, int, Piece::MoveType>(i, j, Piece::CASTLE));
						}
					}
				}
			}
		}
	}
	m_possibleMoves = moves;
}

void King::setCheck(Piece* field[8][8], int x, int y)
{
	bool check = false;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() != m_team)
				{
					if (field[i][j]->getType() == KING)
					{
						if (abs(field[i][j]->getPos().first - x) <= 1 && abs(field[i][j]->getPos().second - y) <= 1)
						{
							check = true;
						}

					}
					else if (field[i][j]->getType() == PAWN)
					{
						int dy_pawn;
						if (field[i][j]->getTeam() == WHITE)
						{
							dy_pawn = 1;
						}
						else
						{
							dy_pawn = -1;
						}
						if ((x == field[i][j]->getPos().first + 1 || x == field[i][j]->getPos().first - 1) && y == field[i][j]->getPos().second + dy_pawn)
						{
							check = true;
						}
					}
					else
					{
						field[i][j]->calcPossibleMoves(field, false);
						std::vector<std::tuple<int, int, Piece::MoveType>> notPossible = field[i][j]->getPossibleMoves();
						for (const auto& value : notPossible)
						{
							if (std::get<0>(value) == x && std::get<1>(value) == y)
							{
								check = true;
							}
						}
					}
				}
			}
		}
	}
	
	m_check = check;
}
