#include <SFML/Graphics.hpp>

#include <cassert>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

#include "Glitcher.hpp"



Glitcher::Glitcher(const string filename)
: data(readAllBytes(filename.c_str())) {
	headerSize = getJPEGHeaderSize(data);

	cout << "Headersize: " << headerSize << endl;

	tex.loadFromMemory(&data[0], data.size());
}


int Glitcher::getJPEGHeaderSize(const vector<unsigned char>& data) {
	int result = 417;

	for(int i = 0; i < data.size(); i++) {
		if(data[i] == 0xFF && data[i + 1] == 0xDA) {
			result = i + 2;
			break;
		}
	}

	return result;
}

vector<unsigned char> Glitcher::readAllBytes(char const* filename) {
	ifstream ifs(filename, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, ios::beg);
	ifs.read(&result[0], pos);

	std::vector<unsigned char> yy(result.size());
	for(sf::Int32 i = 0; i < result.size(); i++)
		yy[i] = (unsigned char)(result[i]);

	return yy;
}


void Glitcher::glitchData(sf::Int32 amount, double seed, sf::Int32 i, sf::Int32 iters) {
	double max_index = data.size() - headerSize - 4;

	//cout << "Max index: " << max_index << endl;

	int px_min = (max_index / iters) * i;
	int px_max = (max_index / iters) * (i + 1);

	//cout << "px_min: " << px_min << endl << "px_max: " << px_max << endl;

	int delta = px_max - px_min;
	int px_i = px_min + delta * seed;

	//cout << "px_i: " << px_i << endl;

	if(px_i > max_index)
		px_i = max_index;

	int index = headerSize + px_i;

	//	cout << "Index: " << index << " - " << hex << int(data[index]) << dec << endl;
	unsigned char new_p = data[index] + amount;
	cout << "Old: " << hex << int(data[index]) << dec << " - New: " << hex << int(new_p) << dec << endl;
	data[index] = new_p;
	//cout << "New: " << hex << int(data[index]) << dec << endl;
}


void Glitcher::glitch(sf::Int32 amount, double seed, sf::Int32 iters) {
	while(true) {
		// Copy old data
		std::vector<unsigned char> copied = data;
		for(sf::Int32 i = 0; i < iters; ++i)
			glitchData(amount, seed, i, iters);

		// Check if valid
		if(tex.loadFromMemory(&data[0], data.size())) break;
		else {
			cout << "Invalid, retrying..." << endl;
			data = copied;
			seed += 0.001;
		}
	}
}

const sf::Texture& Glitcher::getTexture() {
	return tex;
}
