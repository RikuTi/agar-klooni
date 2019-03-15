#include <Windows.h>
#include <iostream>
#include <vector>

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

constexpr int window_height = 600;
constexpr int window_width = 800;

class WorldBounds
{
public:
	WorldBounds(float x_, float y_, float w_, float h_)
	{
		x = x_;
		y = y_;
		w = w_;
		h = h_;
	}

	float x, y, w, h;
};


std::vector<sf::CircleShape> GenerateRandomCircles(int num, WorldBounds bounds)
{
	std::vector<sf::CircleShape> circles;

	for (int i = 0; i < num; i++)
	{
		sf::CircleShape circle;
		circle.setRadius(5);
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		circle.setFillColor(sf::Color(r, g, b, 255));
		circle.setOutlineColor(sf::Color::White);
		circle.setOutlineThickness(1);


		int pos_x_distance = static_cast<int>(bounds.w + std::abs(bounds.x));
		int pos_y_distance = static_cast<int>(bounds.h + std::abs(bounds.y));

		int pos_x = rand() % pos_x_distance;
		int pos_y = rand() % pos_y_distance;
		sf::Vector2f pos;
		pos.x = (bounds.x + pos_x);
		pos.y = (bounds.y + pos_y);
		circle.setPosition(pos);
		circles.push_back(circle);
	}

	return circles;
}

bool IsPointInsideCircle(int point_x,int point_y, int circle_x,int circle_y,int radius)
{
	int dx = point_x - circle_x;
	int dy = point_y - circle_y;
	return dx*dx + dy*dy <= radius*radius;
}


int main()
{
	srand((int)time(0));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Tahoma.ttf");

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Agar", sf::Style::Default, settings);

	sf::Clock clock;

	sf::Mouse mouse;

	sf::CircleShape circle;

	sf::Vector2f halfWindow = sf::Vector2f((float)(window_width / 2), ((float)window_height / 2));

	circle.setRadius(20);
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(halfWindow);
	circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setOutlineThickness(3);
	circle.setOutlineColor(sf::Color(30, 50, 180, 255));
	


	sf::RectangleShape rect_center;
	rect_center.setFillColor(sf::Color::Red);
	rect_center.setSize(sf::Vector2f(5, 5));
	rect_center.setPosition(halfWindow);


	sf::RectangleShape rect_mouse;
	rect_mouse.setFillColor(sf::Color::Cyan);
	rect_mouse.setSize(sf::Vector2f(30, 30));
	rect_mouse.setPosition(halfWindow);


	sf::Text text;
	text.setString("FPS: 0");
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(15, 15));
	text.setFont(font);
	text.setCharacterSize(24);
	text.setOutlineThickness(1);
	text.setOutlineColor(sf::Color::Black);

	float circle_radius = 0.0f;
	int points = 0;
	sf::Text text_points;
	text_points.setString("Points: 0");
	text_points.setFillColor(sf::Color::White);
	text_points.setPosition(sf::Vector2f(660, 15));
	text_points.setFont(font);
	text_points.setCharacterSize(24);
	text_points.setOutlineThickness(1);
	text_points.setOutlineColor(sf::Color::Black);

	sf::RectangleShape fps_background;
	fps_background.setFillColor(sf::Color::Black);
	fps_background.setPosition(sf::Vector2f(0, 10));
	fps_background.setSize(sf::Vector2f(140, 30));

	WorldBounds worldSize = WorldBounds(-500, -500,800,800);

	sf::RectangleShape rect_worldbounds;
	rect_worldbounds.setFillColor(sf::Color(0, 0, 0, 0));
	rect_worldbounds.setOutlineThickness(2);
	rect_worldbounds.setOutlineColor(sf::Color::Red);
	rect_worldbounds.setSize(sf::Vector2f(worldSize.w+std::abs(worldSize.x),worldSize.h+std::abs(worldSize.y)));
	rect_worldbounds.setPosition(sf::Vector2f(worldSize.x,worldSize.y));

	sf::View view(halfWindow, sf::Vector2f((float)window_width, (float)window_height));

	sf::View topbarView(sf::Vector2f(halfWindow.x,115), sf::Vector2f((float)window_width, 200));


	int fps = 0;
	sf::Clock fps_counter;
	int last_framerate = 0;
	std::vector<sf::CircleShape> circles = GenerateRandomCircles(100,worldSize);

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}		
		}

		window.setView(view);


		sf::Vector2f mousepos = window.mapPixelToCoords(mouse.getPosition(window));


		sf::Vector2f direction;
		direction.x = (circle.getPosition().x - static_cast<float>(mousepos.x));
		direction.y = (circle.getPosition().y - static_cast<float>(mousepos.y));

		float hyp = sqrt(direction.x*direction.x + direction.y*direction.y);

		if (direction.x != 0.0f || direction.y != 0.0f || hyp != 0.0f)//there is no distance between points.... dont do anything
		{

			float moveSpeed = 200;
			float XPos = static_cast<float>(circle.getPosition().x - (dt * moveSpeed * (direction.x / hyp)));
			float YPos = static_cast<float>(circle.getPosition().y - (dt * moveSpeed * (direction.y / hyp)));

			sf::Vector2f final_pos = (sf::Vector2f(XPos, YPos));
			circle.setPosition(final_pos);
		}


		sf::Vector2f view_direction;
		view_direction.x = (view.getCenter().x - static_cast<float>(mousepos.x));
		view_direction.y = (view.getCenter().y - static_cast<float>(mousepos.y));

		float view_hyp = sqrt(view_direction.x*view_direction.x + view_direction.y*view_direction.y);

		if (view_direction.x != 0.0f || view_direction.y != 0.0f || view_hyp != 0.0f)//there is no distance between points.... dont do anything
		{
			float moveSpeed = 200;

			float cent_x = static_cast<float>(view.getCenter().x - (dt * moveSpeed * (view_direction.x / view_hyp)));
			float cent_y = static_cast<float>(view.getCenter().y - (dt * moveSpeed * (view_direction.y / view_hyp)));

			sf::Vector2f final_center = sf::Vector2f(cent_x, cent_y);
			view.setCenter(final_center);
		}
		
		WorldBounds viewBounds = WorldBounds((view.getCenter().x), (view.getCenter().y), view.getSize().x, view.getSize().y);
		
		WorldBounds circleBounds = WorldBounds((circle.getPosition().x), (circle.getPosition().y), circle.getRadius(), circle.getRadius());


		if (circleBounds.x < worldSize.x)
		{
			circle.setPosition(sf::Vector2f((worldSize.x), circle.getPosition().y));
		}
		else if (circleBounds.x > worldSize.w)
		{
			circle.setPosition(sf::Vector2f((worldSize.w), circle.getPosition().y));
		}


		if (circleBounds.y < worldSize.y)
		{
			circle.setPosition(sf::Vector2f(circle.getPosition().x, worldSize.y));
		}
		else if (circleBounds.y > worldSize.h)
		{
			circle.setPosition(sf::Vector2f(circle.getPosition().x, worldSize.h));
		}

		if (viewBounds.x < worldSize.x)
		{
			view.setCenter(sf::Vector2f((worldSize.x),view.getCenter().y));
		}
		else if (viewBounds.x > worldSize.w)
		{
			view.setCenter(sf::Vector2f(worldSize.w, view.getCenter().y));
		}

		if (viewBounds.y < worldSize.y)
		{
			view.setCenter(sf::Vector2f(view.getCenter().x, worldSize.y));
		}
		else if (viewBounds.y > worldSize.h)
		{
			view.setCenter(sf::Vector2f(view.getCenter().x, worldSize.h));
		}


		fps++;

		window.clear(sf::Color(43, 43, 43, 255));



		for (size_t i = 0; i < circles.size(); i++)
		{
			sf::CircleShape r_circle = circles.at(i);
			window.draw(r_circle);

			if (IsPointInsideCircle(circle.getPosition().x, circle.getPosition().y, r_circle.getPosition().x, r_circle.getPosition().y, circle.getRadius()))
			{
				std::vector<sf::CircleShape>::iterator it = circles.begin() + i;
				if (it != circles.end())
				{
					circles.erase(it);
					points += 10;
					circle_radius += 0.1f;
					circle.setRadius(circle.getRadius() + circle_radius);
					circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
				}
			}
		}

		window.draw(circle);


		window.draw(rect_worldbounds);

		window.setView(topbarView);

		char buf[32];
		sprintf_s(buf, "FPS: %i", last_framerate);
		text.setString(buf);
		window.draw(text);

		char pbuf[32];
		sprintf_s(pbuf, "Points: %i", points);
		text_points.setString(pbuf);
		window.draw(text_points);

		window.display();


		if (fps_counter.getElapsedTime().asSeconds() >= 1.0f)
		{
			fps_counter.restart();
			last_framerate = fps;
			fps = 0;
		}
	}

	return 0;
}
