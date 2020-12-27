﻿#include "SFML/Graphics.hpp"
#include "global.hpp"
#include <Windows.h>
#include <iostream>
#include <vector>

int artBoardWidth = 1280; //temporary. untill prompting user for size
int artBoardHight = 720; //temporary. untill prompting user for size

sf::RenderWindow artBoard(sf::VideoMode(artBoardWidth, artBoardHight), "Canvas", sf::Style::Close, sf::ContextSettings(0, 0, 0));
sf::View vw(sf::Vector2f(640.f, 360.f), sf::Vector2f(1280.f, 720.f));
int lines_number = 0;
int undo_count = 0;
bool last_cleared = false;
bool mousePressedDown = false; // When a mouse button is pressed this will change to true until a mouse button is released again

float brushSize = 2.0;
float eraserSize = 2.0;

float zoomCordX, zoomCordY;

std::vector<sf::VertexArray> vertices;
sf::Color curr_col = sf::Color::White; //temporary. untill prompting user for input
sf::Color bg_col = sf::Color::Black; //temporary. untill prompting user for input
sf::Vector2i last_Mouse_pos(0, 0);

int main()
{
	vertices.push_back(sf::VertexArray());
	vertices[0].setPrimitiveType(sf::LineStrip);

	init_toolbar(artBoard.getPosition());

	artBoard.setFramerateLimit(60);
	artBoard.setVerticalSyncEnabled(false);

	sf::Image icon;
	icon.loadFromFile("canvasIcon.png");
	artBoard.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	artBoard.clear(bg_col);
	float lastTime = 0;

	while (artBoard.isOpen())
	{
		toolbar_action();

		sf::Event evnt;
		while (artBoard.pollEvent(evnt))
		{
			//printf("Current lines number %d Current undo_count %d Current vertices size %d\n", lines_number, undo_count, vertices.size());
			if (evnt.type == sf::Event::Closed) // Handling the closure of the artBoard
			{
				if (save(artBoard) == -1)  //if cancel is clicked
				{
					continue;
				}
				artBoard.close();
			}

			if (evnt.type == sf::Event::KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Key::Q)
				{
					if (save(artBoard) == -1) //if cancel is clicked
						continue;
					artBoard.close();
				}

				if (evnt.key.code == sf::Keyboard::Key::C)
				{
					last_cleared = true;
				}

				if (evnt.key.code == sf::Keyboard::Key::Z)
				{
					if (last_cleared) {
						last_cleared = false;
					}

					else if (undo_count < (int)vertices.size())
						++undo_count;
				}

				if (evnt.key.code == sf::Keyboard::Key::X)
				{
					if (undo_count > 0)
						--undo_count;
				}

				if (evnt.key.code == sf::Keyboard::Key::Period)
				{
					zoomSelected = true;
					zoomCordX = sf::Mouse::getPosition(artBoard).x;
					zoomCordY = sf::Mouse::getPosition(artBoard).y;
				}
				else if (evnt.key.code == sf::Keyboard::Comma)
				{
					zoomSelected = false;
				}
			}

			if (penSelected)
			{
				pen_action(artBoard, evnt);
			}

			if (brushSelected)
			{
				brush_action(artBoard, evnt, brushSize);
			}

			if (eraserSelected)
			{
				sf::Color prev_col = curr_col;
				curr_col = bg_col;
				brush_action(artBoard, evnt, eraserSize);
				curr_col = prev_col;
			}

			if (lineSelected)
			{
				line_action(artBoard, evnt);
			}

			if (rectangleSelected)
			{
				rectangle_action(artBoard, evnt);
			}

			if (zoomSelected)
			{
				vw.setCenter(sf::Vector2f(zoomCordX, zoomCordY));
				vw.setSize(sf::Vector2f(artBoard.getSize().x / 3, artBoard.getSize().y / 3));
			}
			else
			{
				vw.setCenter(sf::Vector2f(640.f, 360.f));
				vw.setSize(sf::Vector2f(1280.f, 720.f));
			}
		}
		artBoard.setView(vw);
		canvas_draw(artBoard);
		artBoard.display();
	}

	return 0;
}