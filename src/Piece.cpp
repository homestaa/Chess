#include "Piece.h"
#include "King.h"
#include <string>
#include <iostream>

std::vector<std::tuple <int, int, Piece::MoveType>> Piece::getPossibleMoves()
{ 
	return m_possibleMoves; 
}

Piece::Team Piece::getTeam()
{
	return m_team;
}

void Piece::setPosition(std::pair<int, int> newPos) 
{ 
	m_pos = newPos; 
}

std::pair<int, int> Piece::getPos() 
{ 
	return m_pos; 
}

void Piece::setCheck(Piece* field[8][8], int x, int y)
{
	(void)field;
	(void)x;
	(void)y;
	}

bool Piece::getCheck()
{
	return 0;
}

std::vector<std::tuple<int, int, Piece::MoveType>> Piece::pushMove(std::vector<std::tuple<int, int, Piece::MoveType>> moveList,
																   std::tuple<int, int, Piece::MoveType> move,
																   Piece* king,
																   Piece* field[8][8],
																   bool checkCheck)
{
	if (!checkCheck)
	{
		moveList.push_back(move);
	}
	else
	{
		bool enemyPlace = true;
		king->setCheck(field, king->getPos().first, king->getPos().second);
		Piece* zwisch = &(*field[std::get<0>(move)][std::get<1>(move)]);
		enemyPlace = false;

		if (field[std::get<0>(move)][std::get<1>(move)] != nullptr)
		{
			enemyPlace = true;
			field[std::get<0>(move)][std::get<1>(move)] = nullptr;
		}

		std::swap(field[std::get<0>(move)][std::get<1>(move)], field[m_pos.first][m_pos.second]);
		if (m_type == KING)
		{
			king->setCheck(field, std::get<0>(move), std::get<1>(move));
		}
		else
		{
			king->setCheck(field, king->getPos().first, king->getPos().second);
		}
		std::swap(field[std::get<0>(move)][std::get<1>(move)], field[m_pos.first][m_pos.second]);

		if (enemyPlace)
		{
			field[std::get<0>(move)][std::get<1>(move)] = &(*zwisch);
		}
		if (!king->getCheck())
		{
			moveList.push_back(move);
		}
		king->setCheck(field, king->getPos().first, king->getPos().second);
	}
	return moveList;
}

Piece* Piece::getOwnKing(Piece* field[8][8])
{
	for  (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() == m_team && field[i][j]->getType() == Piece::KING)
				{
					return field[i][j];
				}
			}
		}
	}
	return nullptr;
}

Piece::Piece(Team team, std::pair<int, int> pos, SDL_Handler* handler, PieceType type)
	:m_hasMoved(false), m_texture(NULL), m_handler(handler), m_team(team), m_type(type), m_pos(pos)
{
}

Piece::Piece(const Piece& piece)
	:m_hasMoved(false), m_texture(NULL), m_handler(piece.m_handler), m_team(piece.m_team), m_type(piece.m_type), m_pos(piece.m_pos)
{
}

Piece::~Piece()
{
	SDL_DestroyTexture(m_texture);

	m_handler->undoPieceRender(m_pos.first, m_pos.second);
}


void Piece::render()
{
	SDL_Rect src = {0, 0, 60, 60};
	if (m_handler == nullptr)
	{
		sayMyName();
	}
	SDL_Rect dest = { m_handler->SCREEN_WIDTH / 8 * m_pos.first - 2,
					  m_handler->SCREEN_HEIGHT / 8 * m_pos.second,
					  m_handler->SCREEN_WIDTH / 8,
					  m_handler->SCREEN_HEIGHT / 8 };
	m_handler->DrawRectangle(src, dest, m_texture);
}

void Piece::sayMyName()
{
	std::cout << "No Piece here" << std::endl;
}
