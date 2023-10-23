#include <SDL.h>
#include <stdio.h>
#include "MainLoop.h"
#include "SDL_Handler.h"
#include "Game.h"

void MainLoop::run()
{
	SDL_Handler* handler = new SDL_Handler();
	handler->init();

	handler->renderBackground();

	Game* game = new Game(handler);

	int xStart = -1;
	int yStart = -1;
	int xEnd = -1;
	int yEnd = -1;
	Piece* clickedOn = nullptr;

	while (true)
	{
		if (SDL_WaitEvent(&handler->m_event))
		{
			if (handler->m_event.type == SDL_QUIT)
			{
				break;
			}

			if (handler->m_event.type == SDL_MOUSEBUTTONDOWN)
			{
				xStart = handler->m_event.button.x / handler->FIELD_WIDTH;
				yStart = handler->m_event.button.y / handler->FIELD_HEIGHT;
				clickedOn = game->getFieldPos(xStart, yStart);
				if (clickedOn != nullptr)
				{
					if (clickedOn->getTeam() == game->getTurn())
					{
						game->renderPossibleMoves(clickedOn);
					}
				}
			}

			if (handler->m_event.type == SDL_MOUSEBUTTONUP)
			{
				if (clickedOn != nullptr)
				{
					if (clickedOn->getTeam() == game->getTurn())
					{
						game->undoRenderPossibleMoves(clickedOn);
					}
				}
				xEnd = handler->m_event.button.x / handler->FIELD_WIDTH;
				yEnd = handler->m_event.button.y / handler->FIELD_HEIGHT;
				if (clickedOn != nullptr)
				{
					if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
						&& (clickedOn->getTeam() == game->getTurn())
						&& (game->isValidMove(xEnd, yEnd, clickedOn)))
					{
						std::vector<std::tuple<int, int, Piece::MoveType>> list = game->getFieldPos(xStart, yStart)->getPossibleMoves();
						for (const auto& value : list)
						{
							if (std::get<0>(value) == xEnd && std::get<1>(value) == yEnd)
							{
								game->move(clickedOn, std::tuple<int, int, Piece::MoveType>(xEnd, yEnd, std::get<2>(value)));
							}
						}
						xStart = -1;
						yStart = -1;
						yEnd = -1;
						game->calcAllMoves();
						clickedOn = nullptr;
					}
				}
			}
		}
	}

	handler->cleanUp();
}