
class Glitcher {
private:
	vector<unsigned char> data;
	sf::Uint32 headerSize;

	sf::Texture tex;
	bool needsUpdate = false;

private:
	int getJPEGHeaderSize(const vector<unsigned char>& data);

	vector<unsigned char> readAllBytes(char const* filename);

	void glitchData(sf::Int32 amount, double seed, sf::Int32 i, sf::Int32 iters);

public:
	explicit Glitcher(const string filename);

	void glitch(sf::Int32 amount, double seed, sf::Int32 iters);

	const sf::Texture& getTexture();
};
