#include "global.hpp"

sf::Vector2f first_position;
sf::Vector2f firstPoint;
sf::Vector2i last_last_Mouse_pos;

bool brushTap = false;
bool penTap = false;

void brush_action(sf::RenderWindow& artBoard, sf::Event& evnt, float radius)
{
	mouseToggle(evnt);

	if (mousePressedDown)
	{
		brushTap = true;
		sf::Vector2i curr_Mouse_pos = sf::Mouse::getPosition(artBoard);

		if (last_Mouse_pos != curr_Mouse_pos)
		{
			vertices[lines_number].setPrimitiveType(sf::TriangleStrip);
			sf::Vector2i new_Mouse_pos = curr_Mouse_pos;
			if (last_Mouse_pos.x != 0 && last_Mouse_pos.y != 0) {
				brushConnect(last_Mouse_pos, new_Mouse_pos, radius, curr_col);
				brushConnect(new_Mouse_pos, last_Mouse_pos, radius, curr_col);
			}
			if (last_last_Mouse_pos.x != 0 && last_last_Mouse_pos.y != 0) {
				brushConnect(last_last_Mouse_pos, new_Mouse_pos, radius, curr_col);
				brushConnect(new_Mouse_pos, last_last_Mouse_pos, radius, curr_col);
			}

			last_Mouse_pos = new_Mouse_pos;
			last_last_Mouse_pos = last_Mouse_pos;
			//std::cout << "X Y : " << vertices[lines_number][vertices[lines_number].getVertexCount() - 1].position.x << " " << vertices[lines_number][vertices[lines_number].getVertexCount() - 1].position.y << std::endl;
		}

		//curr_col = sf::Color::Color(rand() % 255, rand() % 255, rand() % 255);
	}

	else if (!mousePressedDown && brushTap && vertices[lines_number].getVertexCount() == 0) {
		vertices[lines_number] = fillSquare((sf::Vector2f)last_Mouse_pos, brushSize, curr_col);
	}

	if (!mousePressedDown)
	{
		last_Mouse_pos.x = 0;
		last_Mouse_pos.y = 0;
		last_last_Mouse_pos.x = 0;
		last_last_Mouse_pos.y = 0;
		brushTap = false;
	}
}

void pen_action(sf::RenderWindow& artBoard, sf::Event& evnt)
{
	mouseToggle(evnt);

	if (mousePressedDown)
	{
		penTap = true;
		vertices[lines_number].setPrimitiveType(sf::LineStrip);
		if (last_Mouse_pos != sf::Mouse::getPosition(artBoard))
		{
			vertices[lines_number].append(sf::Vertex(getCoordinates((sf::Vector2f)sf::Mouse::getPosition(artBoard)), curr_col));
			last_Mouse_pos = sf::Mouse::getPosition();
		}

		//curr_col = sf::Color::Color(rand() % 255, rand() % 255, rand() % 255);
	}

	else if (!mousePressedDown && penTap && vertices[lines_number].getVertexCount() <= 2)
	{
		vertices[lines_number].setPrimitiveType(sf::PrimitiveType::Points);
		vertices[lines_number][0] = sf::Vertex(getCoordinates((sf::Vector2f)sf::Mouse::getPosition(artBoard)), curr_col);
	}

	if (!mousePressedDown)
	{
		last_Mouse_pos.x = 0;
		last_Mouse_pos.y = 0;
		penTap = false;
	}

	//(*artBoard).display();
}

void colorPalatte_action(sf::Vector2i mouse_pos)
{
	int x, y;

	sf::RenderWindow colorPalette(sf::VideoMode(80, 40), "", sf::Style::None);
	sf::Texture tex_colorPalette;
	sf::Sprite spt_colorPalette;
	sf::Event evnt;

	tex_colorPalette.loadFromFile("./Resources/img/ColorPalette.png");
	spt_colorPalette.setTexture(tex_colorPalette);

	colorPalette.setPosition(mouse_pos);

	while (colorPalette.isOpen())
	{
		x = sf::Mouse::getPosition(colorPalette).x;
		y = sf::Mouse::getPosition(colorPalette).y;

		if (x < 0 || y < 0 || x > 80 || y > 40)
		{
			colorPalatteSelected = false;
			colorPalette.close();
			break;
		}

		while (colorPalette.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::MouseButtonPressed)
			{
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (x >= 0 && x < 20 && y >= 0 && y < 20)
					{
						curr_col = sf::Color::White;
					}
					else if (x >= 20 && x < 40 && y >= 0 && y < 20)
					{
						curr_col = sf::Color::Cyan;
					}
					else if (x >= 40 && x < 60 && y >= 0 && y < 20)
					{
						curr_col = sf::Color::Yellow;
					}
					else if (x >= 60 && x < 80 && y >= 0 && y < 20)
					{
						curr_col = sf::Color::Magenta;
					}
					else if (x >= 0 && x < 20 && y >= 20 && y < 40)
					{
						curr_col = sf::Color::Black;
					}
					else if (x >= 20 && x < 40 && y >= 20 && y < 40)
					{
						curr_col = sf::Color::Red;
					}
					else if (x >= 40 && x < 60 && y >= 20 && y < 40)
					{
						curr_col = sf::Color::Blue;
					}
					else if (x >= 60 && x < 80 && y >= 20 && y < 40)
					{
						curr_col = sf::Color::Green;
					}
					colorPalatteSelected = false;
					colorPalette.close();
				}
			}
		}
		colorPalette.clear(sf::Color::White);
		colorPalette.draw(spt_colorPalette);
		colorPalette.display();
	}
}

sf::Color colorMixer_action(sf::Vector2i mouse_pos, sf::Color old_col)
{
	int mixerX, mixerY;

	bool redSelected = false;
	bool greenSelected = false;
	bool blueSelected = false;
	bool alphaSelected = false;
	bool mousePressedDown = false;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow colorMixer(sf::VideoMode(438, 110), "", sf::Style::None, settings);
	sf::Color new_col = old_col;

	sf::CircleShape crcl_red(8);
	sf::CircleShape crcl_green(8);
	sf::CircleShape crcl_blue(8);
	sf::CircleShape crcl_alpha(8);

	sf::RectangleShape bar_r(sf::Vector2f(255.0f, 3.0f));
	sf::RectangleShape bar_g(sf::Vector2f(255.0f, 3.0f));
	sf::RectangleShape bar_b(sf::Vector2f(255.0f, 3.0f));
	sf::RectangleShape bar_a(sf::Vector2f(255.0f, 3.0f));
	sf::RectangleShape rect_currCol(sf::Vector2f(80.0f, 47.0f));
	sf::RectangleShape rect_newCol(sf::Vector2f(80.0f, 47.0f));

	colorMixer.setPosition(mouse_pos);

	sf::Text tb_r("R", font_arial, 16);
	tb_r.setPosition({ 8, 8 });
	tb_r.setFillColor(sf::Color(180, 180, 180));
	sf::Text tb_rVal(std::to_string((int)old_col.r), font_arial, 16);
	tb_rVal.setPosition({ 310, 8 });
	tb_rVal.setFillColor(sf::Color(180, 180, 180));

	sf::Text tb_g("G", font_arial, 16);
	tb_g.setPosition({ 8, 33 });
	tb_g.setFillColor(sf::Color(180, 180, 180));
	sf::Text tb_gVal(std::to_string((int)old_col.g), font_arial, 16);
	tb_gVal.setPosition({ 310, 33 });
	tb_gVal.setFillColor(sf::Color(180, 180, 180));

	sf::Text tb_b("B", font_arial, 16);
	tb_b.setPosition({ 8, 58 });
	tb_b.setFillColor(sf::Color(180, 180, 180));
	sf::Text tb_bVal(std::to_string((int)old_col.b), font_arial, 16);
	tb_bVal.setPosition({ 310, 58 });
	tb_bVal.setFillColor(sf::Color(180, 180, 180));

	sf::Text tb_a("A", font_arial, 16);
	tb_a.setPosition({ 8, 83 });
	tb_a.setFillColor(sf::Color(180, 180, 180));
	sf::Text tb_aVal(std::to_string((int)old_col.a), font_arial, 16);
	tb_aVal.setPosition({ 310, 83 });
	tb_aVal.setFillColor(sf::Color(180, 180, 180));

	crcl_red.setPosition((float)old_col.r + 30.0f, 10.0f);
	crcl_red.setOutlineThickness(2);
	crcl_red.setOutlineColor(sf::Color(200, 200, 200));
	crcl_red.setFillColor(sf::Color(old_col.r, 0, 0, 255));

	crcl_green.setPosition((float)old_col.g + 30.0f, 35.0f);
	crcl_green.setOutlineThickness(2);
	crcl_green.setOutlineColor(sf::Color(200, 200, 200));
	crcl_green.setFillColor(sf::Color(0, old_col.g, 0, 255));

	crcl_blue.setPosition((float)old_col.b + 30.0f, 60.0f);
	crcl_blue.setOutlineThickness(2);
	crcl_blue.setOutlineColor(sf::Color(200, 200, 200));
	crcl_blue.setFillColor(sf::Color(0, 0, old_col.b, 255));

	crcl_alpha.setPosition((float)old_col.a + 30.0f, 85.0f);
	crcl_alpha.setOutlineThickness(2);
	crcl_alpha.setOutlineColor(sf::Color(200, 200, 200));
	crcl_alpha.setFillColor(sf::Color(0, 0, 0, old_col.a));

	rect_newCol.setPosition(350.0, 8.0);
	rect_newCol.setFillColor(new_col);

	rect_currCol.setPosition(350.0, 55.0);
	rect_currCol.setFillColor(old_col);

	bar_r.setPosition(38.0, 17.0);
	bar_r.setFillColor(sf::Color::Red);

	bar_g.setPosition(38.0, 42.0);
	bar_g.setFillColor(sf::Color::Green);

	bar_b.setPosition(38.0, 67.0);
	bar_b.setFillColor(sf::Color::Blue);

	bar_a.setPosition(38.0, 92.0);
	bar_a.setFillColor(sf::Color::White);

	while (colorMixer.isOpen())
	{
		mixerX = sf::Mouse::getPosition(colorMixer).x;
		mixerY = sf::Mouse::getPosition(colorMixer).y;

		if (mixerX < 0 || mixerY < 0 || mixerX > 438 || mixerY > 110)
		{
			colorMixerSelected = false;
			colorMixer.close();
			return new_col;
		}

		sf::Event evnt;
		while (colorMixer.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
			{
				colorMixer.close();
				return old_col;
			}
			else if (evnt.type == sf::Event::MouseButtonPressed)
			{
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					mousePressedDown = true;
					int x = sf::Mouse::getPosition(colorMixer).x;
					int y = sf::Mouse::getPosition(colorMixer).y;
					int redX = (int)crcl_red.getPosition().x;
					int redY = (int)crcl_red.getPosition().y;
					int greenX = (int)crcl_green.getPosition().x;
					int greenY = (int)crcl_green.getPosition().y;
					int blueX = (int)crcl_blue.getPosition().x;
					int blueY = (int)crcl_blue.getPosition().y;
					int alphaX = (int)crcl_alpha.getPosition().x;
					int alphaY = (int)crcl_alpha.getPosition().y;
					if ((x >= redX && x <= redX + 16 && y >= redY && y <= redY + 16) || (x >= 38 && x <= 293 && y >= 10 && y <= 27))
					{
						redSelected = true;
						greenSelected = false;
						blueSelected = false;
						alphaSelected = false;
					}
					else if ((x >= greenX && x <= greenX + 16 && y >= greenY && y <= greenY + 16) || (x >= 38 && x <= 293 && y >= 35 && y <= 52))
					{
						redSelected = false;
						greenSelected = true;
						blueSelected = false;
						alphaSelected = false;
					}
					else if ((x >= blueX && x <= blueX + 16 && y >= blueY && y <= blueY + 16) || (x >= 38 && x <= 293 && y >= 60 && y <= 77))
					{
						redSelected = false;
						greenSelected = false;
						blueSelected = true;
						alphaSelected = false;
					}
					else if ((x >= alphaX && x <= alphaX + 16 && y >= alphaY && y <= alphaY + 16) || (x >= 38 && x <= 293 && y >= 85 && y <= 102))
					{
						redSelected = false;
						greenSelected = false;
						blueSelected = false;
						alphaSelected = true;
					}
				}
			}
			else if (evnt.type == sf::Event::MouseButtonReleased)
			{
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					mousePressedDown = false;
					redSelected = false;
					greenSelected = false;
					blueSelected = false;
					alphaSelected = false;
				}
			}
			else if (evnt.type == sf::Event::KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Key::Escape)
				{
					colorMixerSelected = false;
					colorMixer.close();
					return old_col;
				}
				else if (evnt.key.code == sf::Keyboard::Key::Enter)
				{
					colorMixerSelected = false;
					colorMixer.close();
					return new_col;
				}
			}
		}
		if (redSelected)
		{
			float x = sf::Mouse::getPosition(colorMixer).x - crcl_red.getRadius() / 2;
			float sliderPos = (float)x - 30.0f;
			if (x >= 30.0 && x < 286.0)
			{
				crcl_red.setPosition({ x, 10.0 });
				crcl_red.setFillColor(sf::Color((int)sliderPos % 256, 0, 0, 255));
				new_col.r = (int)sliderPos % 256;
				tb_rVal.setString(std::to_string((int)new_col.r));
			}
		}
		if (greenSelected)
		{
			float x = sf::Mouse::getPosition(colorMixer).x - crcl_green.getRadius() / 2;
			float sliderPos = (float)x - 30.0f;
			if (x >= 30.0 && x < 286.0)
			{
				crcl_green.setPosition({ x, 35.0 });
				crcl_green.setFillColor(sf::Color(0, (int)sliderPos % 256, 0, 255));
				new_col.g = (int)sliderPos % 256;
				tb_gVal.setString(std::to_string((int)new_col.g));
			}
		}
		if (blueSelected)
		{
			float x = sf::Mouse::getPosition(colorMixer).x - crcl_blue.getRadius() / 2;
			float sliderPos = (float)x - 30.0f;
			if (x >= 30.0 && x < 286.0)
			{
				crcl_blue.setPosition({ x, 60.0 });
				crcl_blue.setFillColor(sf::Color(0, 0, (int)sliderPos % 256, 255));
				new_col.b = (int)sliderPos % 256;
				tb_bVal.setString(std::to_string((int)new_col.b));
			}
		}
		if (alphaSelected)
		{
			float x = sf::Mouse::getPosition(colorMixer).x - crcl_alpha.getRadius() / 2;
			float sliderPos = (float)x - 30.0f;
			if (x >= 30.0 && x < 286.0)
			{
				crcl_alpha.setPosition({ x, 85.0 });
				crcl_alpha.setFillColor(sf::Color(0, 0, 0, (int)sliderPos % 256));
				new_col.a = (int)sliderPos % 256;
				tb_aVal.setString(std::to_string((int)new_col.a));
			}
		}

		rect_newCol.setFillColor(new_col);

		colorMixer.clear(sf::Color(60, 60, 60));

		colorMixer.draw(tb_r);
		colorMixer.draw(tb_rVal);
		colorMixer.draw(tb_g);
		colorMixer.draw(tb_gVal);
		colorMixer.draw(tb_b);
		colorMixer.draw(tb_bVal);
		colorMixer.draw(tb_a);
		colorMixer.draw(tb_aVal);
		colorMixer.draw(bar_r);
		colorMixer.draw(bar_g);
		colorMixer.draw(bar_b);
		colorMixer.draw(bar_a);
		colorMixer.draw(crcl_red);
		colorMixer.draw(crcl_green);
		colorMixer.draw(crcl_blue);
		colorMixer.draw(crcl_alpha);
		colorMixer.draw(rect_newCol);
		colorMixer.draw(rect_currCol);
		colorMixer.display();
	}

	return new_col;
}

void line_action(sf::RenderWindow& artBoard, sf::Event& evnt)
{
	//if((int)vertices[lines_number].getVertexCount() > 1)
	sf::Color guide_col = curr_col;
	guide_col.a = curr_col.a / 2;

	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (undo_count > 0) {
			while (undo_count) {
				vertices.pop_back();
				--lines_number;
				--undo_count;
			}
		}

		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			undo_count = 0;
			if (last_cleared) {
				vertices.clear();
				lines_number = -1;
				last_cleared = false;
			}

			vertices.push_back(sf::VertexArray(sf::TriangleStrip, 4));
			lines_number++;
			//printf("Current line number %d vector size %d\n", lines_number, vertices.size());
			first_position = (sf::Vector2f)sf::Mouse::getPosition(artBoard);
			mousePressedDown = true;
		}
	}

	if (evnt.type == sf::Event::MouseButtonReleased)
	{
		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			mousePressedDown = false;
			last_Mouse_pos.x = 0;
			last_Mouse_pos.y = 0;
		}
	}

	if (mousePressedDown)
	{
		if (last_Mouse_pos != sf::Mouse::getPosition(artBoard))
		{
			rectangleConnect((sf::Vector2f)sf::Mouse::getPosition(artBoard), first_position, brushSize, guide_col, 0);
			rectangleConnect(first_position, (sf::Vector2f)sf::Mouse::getPosition(artBoard), brushSize, guide_col, 1);
			last_Mouse_pos = sf::Mouse::getPosition();
		}
	}
	if (evnt.type == sf::Event::MouseButtonReleased)
	{
		rectangleConnect((sf::Vector2f)sf::Mouse::getPosition(artBoard), first_position, brushSize, curr_col, 0);
		rectangleConnect(first_position, (sf::Vector2f)sf::Mouse::getPosition(artBoard), brushSize, curr_col, 1);
	}
}

float brushSize_action(sf::Vector2i mouse_pos, float current)
{
	int x, y;
	float size = current;

	sf::RenderWindow sizePicker(sf::VideoMode(80, 30), "", sf::Style::None);
	sf::Texture tex_sizePicker;
	sf::Sprite spt_sizePicker;
	sf::Event evnt;

	tex_sizePicker.loadFromFile("./Resources/img/size_icons.png");
	spt_sizePicker.setTexture(tex_sizePicker);

	sizePicker.setPosition(mouse_pos);

	while (sizePicker.isOpen())
	{
		x = sf::Mouse::getPosition(sizePicker).x;
		y = sf::Mouse::getPosition(sizePicker).y;

		if (x < 0 || y < 0 || x > 80 || y > 30)
		{
			sizePicker.close();
			break;
		}

		while (sizePicker.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::MouseButtonPressed)
			{
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (x >= 0 && x < 26 && y >= 0 && y < 30)
					{
						size = 4.0;
					}
					else if (x >= 26 && x < 53 && y >= 0 && y < 30)
					{
						size = 8.0;
					}
					else if (x >= 53 && x < 80 && y >= 0 && y < 30)
					{
						size = 10.5;
					}
					sizePicker.close();
				}
			}
		}
		sizePicker.clear(sf::Color(50, 50, 50));
		sizePicker.draw(spt_sizePicker);
		sizePicker.display();
	}
	return size;
}

void rectangle_action(sf::RenderWindow& artBoard, sf::Event& evnt)
{
	sf::Color guide_col = curr_col;
	guide_col.a = curr_col.a / 4;

	sf::Vector2f pos0, pos1, pos2, pos3, apos0, aapos0, apos1, apos2, aapos2, apos3, aapos3;

	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (undo_count > 0) {
			while (undo_count) {
				vertices.pop_back();
				--lines_number;
				--undo_count;
			}
		}

		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			undo_count = 0;
			if (last_cleared) {
				vertices.clear();
				lines_number = -1;
				last_cleared = false;
			}

			vertices.push_back(sf::VertexArray(sf::TriangleStrip, 16));
			lines_number++;
			if (filledRectangle)
			{
				vertices[lines_number].setPrimitiveType(sf::Quads);
			}
			first_position = sf::Vector2f(sf::Mouse::getPosition(artBoard));
			//vertices[lines_number][0] = sf::Vertex(getCoordinates((sf::Vector2f)sf::Mouse::getPosition(artBoard)), guide_col);
			mousePressedDown = true;
		}
	}

	if (mousePressedDown)
	{
		if (last_Mouse_pos != sf::Mouse::getPosition(artBoard))
		{
			pos2 = (sf::Vector2f)sf::Mouse::getPosition(artBoard);
			pos0 = first_position;
			pos1.x = pos2.x; pos1.y = pos0.y;
			pos3.x = pos0.x; pos3.y = pos2.y;

			if (filledRectangle)
			{
				vertices[lines_number].clear();
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos0), guide_col));
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos1), guide_col));
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos2), guide_col));
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos3), guide_col));
			}

			else if (!filledRectangle)
			{
				apos0.x = pos0.x - 2 * brushSize; apos0.y = pos0.y;
				apos1.x = pos1.x; apos1.y = pos1.y + brushSize;
				apos2.x = pos2.x; apos2.y = pos2.y + brushSize;
				aapos2.x = pos2.x - 2 * brushSize, aapos2.y = pos2.y;
				apos3.x = pos3.x - 2 * brushSize, apos3.y = pos3.y;
				aapos3.x = pos3.x; aapos3.y = apos3.y - brushSize;
				aapos0.x = pos0.x; aapos0.y = apos0.y + brushSize;

				rectangleConnect(apos0, pos1, brushSize, guide_col, 0);
				rectangleConnect(pos1, apos0, brushSize, guide_col, 1);
				rectangleConnect(apos1, apos2, brushSize, guide_col, 2);
				rectangleConnect(apos2, apos1, brushSize, guide_col, 3);
				rectangleConnect(aapos2, apos3, brushSize, guide_col, 4);
				rectangleConnect(apos3, aapos2, brushSize, guide_col, 5);
				rectangleConnect(aapos3, aapos0, brushSize, guide_col, 6);
				rectangleConnect(aapos0, aapos3, brushSize, guide_col, 7);
			}

			last_Mouse_pos = sf::Mouse::getPosition();
		}
	}

	if (evnt.type == sf::Event::MouseButtonReleased)
	{
		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			mousePressedDown = false;
			last_Mouse_pos.x = 0;
			last_Mouse_pos.y = 0;
			pos2 = (sf::Vector2f)sf::Mouse::getPosition(artBoard);
			pos0 = first_position;
			pos1.x = pos2.x; pos1.y = pos0.y;
			pos3.x = pos0.x; pos3.y = pos2.y;

			if (filledRectangle)
			{
				vertices[lines_number].clear();
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos0), curr_col));
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos1), curr_col));
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos2), curr_col));
				vertices[lines_number].append(sf::Vertex(getCoordinates(pos3), curr_col));
			}

			else if (!filledRectangle)
			{
				apos0.x = pos0.x - 2 * brushSize; apos0.y = pos0.y;
				apos1.x = pos1.x; apos1.y = pos1.y + brushSize;
				apos2.x = pos2.x; apos2.y = pos2.y + brushSize;
				aapos2.x = pos2.x - 2 * brushSize, aapos2.y = pos2.y;
				apos3.x = pos3.x - 2 * brushSize, apos3.y = pos3.y;
				aapos3.x = pos3.x; aapos3.y = apos3.y - brushSize;
				aapos0.x = pos0.x; aapos0.y = apos0.y + brushSize;

				rectangleConnect(apos0, pos1, brushSize, curr_col, 0);
				rectangleConnect(pos1, apos0, brushSize, curr_col, 1);
				rectangleConnect(apos1, apos2, brushSize, curr_col, 2);
				rectangleConnect(apos2, apos1, brushSize, curr_col, 3);
				rectangleConnect(aapos2, apos3, brushSize, curr_col, 4);
				rectangleConnect(apos3, aapos2, brushSize, curr_col, 5);
				rectangleConnect(aapos3, aapos0, brushSize, curr_col, 6);
				rectangleConnect(aapos0, aapos3, brushSize, curr_col, 7);
			}
		}
	}
}

void circle_action(sf::RenderWindow& artBoard, sf::Event& evnt)
{
	sf::Color guide_col = curr_col;
	guide_col.a = curr_col.a / 3;

	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (undo_count > 0) {
			while (undo_count) {
				vertices.pop_back();
				--lines_number;
				--undo_count;
			}
		}

		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			undo_count = 0;
			if (last_cleared) {
				vertices.clear();
				lines_number = -1;
				last_cleared = false;
			}

			vertices.push_back(sf::VertexArray(sf::TriangleStrip));
			lines_number++;

			firstPoint = sf::Vector2f(sf::Mouse::getPosition(artBoard));

			if (filledCircle)
			{
				vertices[lines_number].setPrimitiveType(sf::TrianglesFan);
			}
			//firstPoint = getCoordinates(firstPoint);
			mousePressedDown = true;
		}
	}

	if (mousePressedDown)
	{
		if (last_Mouse_pos != sf::Mouse::getPosition(artBoard))
		{
			sf::Vector2f curr_pos = sf::Vector2f(sf::Mouse::getPosition(artBoard));
			float radius = 0.5f * sqrt((curr_pos.x - firstPoint.x) * (curr_pos.x - firstPoint.x) + (curr_pos.y - firstPoint.y) * (curr_pos.y - firstPoint.y));
			//curr_pos = getCoordinates(curr_pos);
			sf::Vector2f center;
			center.x = firstPoint.x + 0.5f * (curr_pos.x - firstPoint.x);
			center.y = firstPoint.y + 0.5f * (curr_pos.y - firstPoint.y);
			//center = getCoordinates(center);
			vertices[lines_number].clear();
			if (filledCircle)
				vertices[lines_number].append(sf::Vertex(getCoordinates(center), guide_col));
			circleConnect(center, radius, guide_col);
			last_Mouse_pos = sf::Mouse::getPosition();
		}
	}

	if (evnt.type == sf::Event::MouseButtonReleased)
	{
		if (evnt.mouseButton.button == sf::Mouse::Left) {
			last_Mouse_pos.x = 0;
			last_Mouse_pos.y = 0;
			mousePressedDown = false;
			sf::Vector2f curr_pos = sf::Vector2f(sf::Mouse::getPosition(artBoard));
			float radius = 0.5f * sqrt((curr_pos.x - firstPoint.x) * (curr_pos.x - firstPoint.x) + (curr_pos.y - firstPoint.y) * (curr_pos.y - firstPoint.y));
			//curr_pos = getCoordinates(curr_pos);
			sf::Vector2f center;
			center.x = firstPoint.x + 0.5f * (curr_pos.x - firstPoint.x);
			center.y = firstPoint.y + 0.5f * (curr_pos.y - firstPoint.y);
			//center = getCoordinates(center);
			vertices[lines_number].clear();
			if (filledCircle)
				vertices[lines_number].append(sf::Vertex(getCoordinates(firstPoint), curr_col));
			circleConnect(center, radius, curr_col);
		}
	}
}

void gradient_action(sf::RenderWindow& artBoard, sf::Event& evnt)
{
	sf::Color guide_col;

	if (bg_col != sf::Color::White)
	{
		guide_col = sf::Color::White;
	}
	else
		guide_col = sf::Color::Black;

	guide_col.a = curr_col.a / 2;

	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (undo_count > 0) {
			while (undo_count) {
				vertices.pop_back();
				--lines_number;
				--undo_count;
			}
		}

		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			undo_count = 0;
			if (last_cleared) {
				vertices.clear();
				lines_number = -1;
				last_cleared = false;
			}

			vertices.push_back(sf::VertexArray(sf::TriangleStrip, 4));
			lines_number++;
			//printf("Current line number %d vector size %d\n", lines_number, vertices.size());
			first_position = (sf::Vector2f)sf::Mouse::getPosition(artBoard);
			mousePressedDown = true;
		}
	}

	if (evnt.type == sf::Event::MouseButtonReleased)
	{
		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			mousePressedDown = false;
			last_Mouse_pos.x = 0;
			last_Mouse_pos.y = 0;
		}
	}

	if (mousePressedDown)
	{
		if (last_Mouse_pos != sf::Mouse::getPosition(artBoard))
		{
			rectangleConnect((sf::Vector2f)sf::Mouse::getPosition(artBoard), first_position, 1.0f, guide_col, 0);
			rectangleConnect(first_position, (sf::Vector2f)sf::Mouse::getPosition(artBoard), 1.0f, guide_col, 1);
			last_Mouse_pos = sf::Mouse::getPosition();
		}
	}
	if (evnt.type == sf::Event::MouseButtonReleased)
	{
		sf::Vector2f pos0, pos1, pos2, pos3;
		pos0 = first_position;
		pos2 = (sf::Vector2f)sf::Mouse::getPosition(artBoard);
		pos1.x = pos2.x; pos1.y = pos0.y;
		pos3.x = pos0.x; pos3.y = pos2.y;
		vertices[lines_number].clear();
		vertices[lines_number].setPrimitiveType(sf::Quads);
		vertices[lines_number].append(sf::Vertex(getCoordinates(pos0), curr_col));
		vertices[lines_number].append(sf::Vertex(getCoordinates(pos1), grad_col));
		vertices[lines_number].append(sf::Vertex(getCoordinates(pos2), grad_col));
		vertices[lines_number].append(sf::Vertex(getCoordinates(pos3), curr_col));
	}
}

void fill_action(sf::RenderWindow& artBoard, sf::Event& evnt)
{
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (undo_count > 0) {
			while (undo_count) {
				vertices.pop_back();
				--lines_number;
				--undo_count;
			}
		}

		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			undo_count = 0;
			if (last_cleared) {
				vertices.clear();
				lines_number = -1;
				last_cleared = false;
			}

			vertices.push_back(sf::VertexArray(sf::TriangleStrip));
			lines_number++;
			sf::Texture curr_texture;
			curr_texture.create(artBoard.getSize().x, artBoard.getSize().y);
			curr_texture.update(artBoard);
			sf::Image curr_state = curr_texture.copyToImage();
			sf::Vector2i start = sf::Mouse::getPosition(artBoard);
			sf::Color prevCol = curr_state.getPixel(start.x, start.y);

			vertices[lines_number].append(sf::Vertex(getCoordinates((sf::Vector2f)start), curr_col));
			vertices[lines_number].append(sf::Vertex(getCoordinates((sf::Vector2f)start), curr_col));
			floodfill(start, curr_state, prevCol, artBoard);
			vertices[lines_number].append(sf::Vertex(getCoordinates((sf::Vector2f)start), curr_col));
			vertices[lines_number].append(sf::Vertex(getCoordinates((sf::Vector2f)start), curr_col));
		}
	}
}
bool fillingOptions(sf::Vector2i mouse_pos, bool fillStatus)
{
	int x, y;

	sf::RenderWindow win_shapeFillOption(sf::VideoMode(60, 40), "", sf::Style::None);
	win_shapeFillOption.setPosition(mouse_pos);
	sf::Event evnt;

	sf::RectangleShape btn_bg_fill(sf::Vector2f(60.0f, 20.0f));
	btn_bg_fill.setPosition(0.0f, 0.0f);
	btn_bg_fill.setFillColor(sf::Color(70, 70, 70));
	sf::RectangleShape btn_bg_noFill(sf::Vector2f(60.0f, 20.0f));
	btn_bg_noFill.setPosition(0.0f, 20.0f);
	btn_bg_noFill.setFillColor(sf::Color(70, 70, 70));

	sf::Text txt_fill("Fill ", font_arial, 13);
	txt_fill.setPosition({ btn_bg_fill.getPosition().x + 20.0f, btn_bg_fill.getPosition().y + 2.0f });
	sf::Text txt_noFill("No Fill ", font_arial, 13);
	txt_noFill.setPosition({ btn_bg_noFill.getPosition().x + 11.0f, btn_bg_noFill.getPosition().y + 2.0f });

	while (win_shapeFillOption.isOpen())
	{
		x = sf::Mouse::getPosition(win_shapeFillOption).x;
		y = sf::Mouse::getPosition(win_shapeFillOption).y;

		if (x < 0 || y < 0 || x > 60 || y > 40)
		{
			win_shapeFillOption.close();
			break;
		}

		while (win_shapeFillOption.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::MouseButtonPressed)
			{
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (x >= 0 && x < 60 && y >= 0 && y < 20)
					{
						fillStatus = true;
					}
					else if (x >= 0 && x < 60 && y >= 20 && y < 40)
					{
						fillStatus = false;
					}
					win_shapeFillOption.close();
					return fillStatus;
				}
			}
		}

		/*if (x >= 0 && x < 60 && y >= 0 && y < 20)
		{
			btn_bg_fill.setFillColor(sf::Color(46, 46, 46));
			btn_bg_noFill.setFillColor(sf::Color(70, 70, 70));
		}
		else if (x >= 0 && x < 60 && y >= 20 && y < 40)
		{
			btn_bg_fill.setFillColor(sf::Color(70, 70, 70));
			btn_bg_noFill.setFillColor(sf::Color(46, 46, 46));
		}*/

		if (fillStatus)
		{
			btn_bg_fill.setFillColor(sf::Color(46, 46, 46));
		}
		else if (!fillStatus && x >= 0 && x < 60 && y >= 0 && y < 20)
		{
			btn_bg_fill.setFillColor(sf::Color(60, 60, 60));
		}
		else
		{
			btn_bg_fill.setFillColor(sf::Color(70, 70, 70));
		}

		if (!fillStatus)
		{
			btn_bg_noFill.setFillColor(sf::Color(46, 46, 46));
		}
		else if (fillStatus && x >= 0 && x < 60 && y >= 20 && y < 40)
		{
			btn_bg_noFill.setFillColor(sf::Color(60, 60, 60));
		}
		else
		{
			btn_bg_noFill.setFillColor(sf::Color(70, 70, 70));
		}

		win_shapeFillOption.clear(sf::Color(50, 50, 50));
		win_shapeFillOption.draw(btn_bg_fill);
		win_shapeFillOption.draw(btn_bg_noFill);
		win_shapeFillOption.draw(txt_fill);
		win_shapeFillOption.draw(txt_noFill);
		win_shapeFillOption.display();
	}
	return fillStatus;
}

void eyedropper_action(sf::RenderWindow& artBoard, sf::Event& evnt)
{
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			sf::Texture curr_texture;
			curr_texture.create(artBoard.getSize().x, artBoard.getSize().y);
			curr_texture.update(artBoard);
			sf::Image curr_state = curr_texture.copyToImage();
			sf::Vector2i currPos = sf::Mouse::getPosition(artBoard);
			curr_col = curr_state.getPixel(currPos.x, currPos.y);
		}
	}
}