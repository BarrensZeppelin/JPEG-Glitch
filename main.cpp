#include <SFML/Graphics.hpp>

#include <cassert>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

#include "Glitcher.hpp"
#include "InputParser.hpp"

void saveGlitchedToFile(Glitcher& glitcher, const std::string& outputDir, int iteration) {
	sf::Image img = glitcher.getTexture().copyToImage();
	img.saveToFile(outputDir + "/" + to_string(iteration) + ".jpeg");
}

int main(int argc, char* argv[]) {
	srand(time(0));

	sf::Int32 maxAmount = 256, iterations = 2, delay = 50;

	InputParser input(argc, argv);
	auto& filename = input.getCmdOption(0);
	if(filename.empty()) {
		cerr << "Missing filename" << endl;
		return 1;
	}

	Glitcher glitcher(filename);

	auto& maxAmountS = input.getCmdOption(1);
	if(!maxAmountS.empty()) maxAmount = stoi(maxAmountS);

	auto& iterationsS = input.getCmdOption(2);
	if(!iterationsS.empty()) iterations = stoi(iterationsS);

	auto& delayS = input.getCmdOption(3);
	if(!delayS.empty()) delay = stoi(delayS);

	auto& outputDir = input.getCmdOption("-o");
	if(!outputDir.empty()) {
		system(std::string("mkdir -p " + outputDir).c_str());
		saveGlitchedToFile(glitcher, outputDir, 0);
	}


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
	int iteration = 1;
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

			if(!outputDir.empty()) {
				saveGlitchedToFile(glitcher, outputDir, iteration);
				iteration++;
			}
		}

		window.clear();

		window.draw(spr);

		window.draw(spr2);

		window.display();
	}

	return 0;
}
