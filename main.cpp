#include <SFML/Graphics.hpp>

#include <cassert>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

#include "Glitcher.hpp"




int main(int argc, char* argv[]) {
	srand(time(0));
	remove("newpic.jpg");

	std::string filename = "pic.jpg";
	sf::Int32 maxAmount = 256, iterations = 2, delay = 50;
	if(argc >= 2) {
		maxAmount = atoi(argv[1]);
		if(argc >= 3) {
			iterations = atoi(argv[2]);

			if(argc >= 4) {
				delay = atoi(argv[3]);

				if(argc >= 5) {
					filename = argv[4];
				}
			}
		}
	}


	/*ofstream output("newpic.jpg", ios::binary | ios::out);
	copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(output));
	output.close();*/


	Glitcher glitcher(filename);

	sf::Texture tex;
	tex.loadFromFile(filename);

	sf::Vector2u windowSize = tex.getSize();
	windowSize += {0, windowSize.y + 10};

	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "G-Glitch", sf::Style::Close);

	sf::Sprite spr(glitcher.getTexture());

	sf::Sprite spr2(tex);
	spr2.move(0, spr.getLocalBounds().height + 10);


	sf::Event event;
	sf::Time bank;
	sf::Clock dClock;
	sf::Time dt;
	sf::Time delayTime = sf::milliseconds(delay);
	while(window.isOpen()) {
		dt = dClock.restart();

		while(window.pollEvent(event)) {
			switch(event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		bank += dt;
		if(bank >= delayTime) {
			bank -= delayTime;

			sf::Int32 amount = rand() % maxAmount;
			double seed = (rand() % 1000) / 1000.0;
			glitcher.glitch(amount, seed, iterations);
			spr.setTexture(glitcher.getTexture());
		}

		window.clear();

		window.draw(spr);

		window.draw(spr2);

		window.display();
	}

	return 0;
}
