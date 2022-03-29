#pragma once
#include "includes.h"
#include "robin_hood.h"
constexpr auto ZEROBYTEARRAY = -1;
constexpr auto MAX_BLOCK_SIZE = 256;


class Block
{
public:
    Block(std::vector<BYTE> block = {});
	std::vector<BYTE> BlockArray;
};

class pureBlock
{
public:
	pureBlock();
	pureBlock(BYTE* array, int size);
	BYTE BlockArray[MAX_BLOCK_SIZE] = {};
	int size = MAX_BLOCK_SIZE;
};

class BlockScanner
{
public:
	BlockScanner() = default;
	explicit BlockScanner(Block block);
	explicit BlockScanner(pureBlock block);
	double getEntropy();
	void getFrequency();
	robin_hood::unordered_map<BYTE, int> returnFrequency();
private:
	std::optional<Block> currentBlock;
	std::optional<pureBlock> currentPureBlock;
	robin_hood::unordered_map<BYTE, int> frequency;
};

class Entropy
{
public:
	Entropy(std::vector<double> EntropyList = {});
	double calculateAverageEntropy();
	double readByteEntropy(std::string filePath,bool safety);
private:
	std::vector<double> EntropyList;
};
