#include "Entropy.h"

Block::Block(std::vector<BYTE> block)
{
	this->BlockArray.reserve(256);
	int count = 0;
	if (block.size() == 0) {
		for (; count < 256; count++)
		{
			this->BlockArray.push_back(0);
		}
	}
	for (unsigned int i = 0; i < block.size(); i++)
	{
		this->BlockArray.push_back(block[i]);
		count++;
	}
	for (; count < 256; count++)
	{
		this->BlockArray.push_back(0);
	}
}

pureBlock::pureBlock()
{
	ZeroMemory(this->BlockArray, MAX_BLOCK_SIZE);
}

pureBlock::pureBlock(BYTE array[], int size)
{
	memmove(this->BlockArray, array, size);
	if (size != MAX_BLOCK_SIZE)
	{
		ZeroMemory(&this->BlockArray[size], MAX_BLOCK_SIZE - size);
	}
}

BlockScanner::BlockScanner(Block block)
{
	this->currentBlock = block;
}

BlockScanner::BlockScanner(pureBlock block)
{
	this->currentPureBlock = block;
}


double BlockScanner::getEntropy()
{
	double entropy = 0;

	if (frequency[0x00] > 128)
	{
		return ZEROBYTEARRAY;
	}
	
	for (auto pair : this->frequency)
	{
		double prob = (double)pair.second / MAX_BLOCK_SIZE;
		if (prob > 0) entropy -= prob * log2(prob);
	}
	return entropy;
}

robin_hood::unordered_map<BYTE, int> BlockScanner::returnFrequency()
{
	return this->frequency;
}

Entropy::Entropy(std::vector<double> EntropyList)
{
	this->EntropyList = EntropyList;
}

double Entropy::calculateAverageEntropy()
{
	double total = 0;
	int zeroBlocks = 0;
	for (auto x : this->EntropyList)
	{
		if (x != ZEROBYTEARRAY)
		{
			total += x;
		}
		else
		{
			//std::cout << "Zero byte array block found, ignoring" << std::endl;
			zeroBlocks += 1;
		}
	}
	return total / (this->EntropyList.size() - zeroBlocks);
}

double Entropy::readByteEntropy(std::string filePath,bool safety) //O(n) time, O(1) space
{
	this->EntropyList.clear();
	if (safety == true)
	{
		std::ifstream fileStream(filePath, std::ifstream::binary);
		if (fileStream)
		{
			if (fileStream.peek() == EOF)
			{
				this->EntropyList.emplace_back(0);
			}
			while (fileStream.peek() != EOF)
			{
				char buf[256];
				fileStream.read(buf, 256);
				if (fileStream.eof() == true)
				{
					this->EntropyList.emplace_back(0); //skips last 256 byte array to avoid reading junk data, no executable will be < 256 bytes therefore this consideration can be ignored
					continue;
				}
				std::vector<BYTE> byteArray;
				byteArray.reserve(256);
				for (int i = 0; i < 256; i++)
				{
					byteArray.emplace_back(*(BYTE*)&buf[i]); //extremely illegal
				}
				fileStream.seekg(256, std::ios::cur);
				Block curBlock(byteArray);
				BlockScanner curScanner(curBlock);
				curScanner.getFrequency();
				this->EntropyList.emplace_back(curScanner.getEntropy());
				//std::cout << "256 bytes read" << '\n';
			}
		}
		else
		{
			this->EntropyList.emplace_back(-2);
		}
		return this->calculateAverageEntropy();
	}
	else if(safety == false)
	{
		std::ifstream fileStream(filePath, std::ifstream::binary);
		if (fileStream)
		{
			if (fileStream.peek() == EOF)
			{
				this->EntropyList.emplace_back(0);
			}
			while (fileStream.peek() != EOF)
			{
				BYTE buf[MAX_BLOCK_SIZE];
				fileStream.read((char*)buf, 256);
				fileStream.seekg(256, std::ios::cur);
				pureBlock curBlock(buf, MAX_BLOCK_SIZE);
				BlockScanner curScanner(curBlock);
				curScanner.getFrequency();
				this->EntropyList.emplace_back(curScanner.getEntropy());
			}
		}
		else
		{
			this->EntropyList.emplace_back(-2);
		}
		return this->calculateAverageEntropy();
	}
    else
    {
        return false;
    }
}

void BlockScanner::getFrequency()
{
	this->frequency.reserve(256);
	if (this->currentBlock.has_value())
	{
		for (unsigned short x = 0; x < 256; x++)
		{
			BYTE currentByte = this->currentBlock.value().BlockArray[x];
			if (this->frequency.contains(currentByte))
			{
				this->frequency[currentByte] += 1;
			}
			else
			{
				this->frequency.insert({ currentByte,1 });
			}
		}
	}
	else if(this->currentPureBlock.has_value())
	{
		for (unsigned short x = 0; x < MAX_BLOCK_SIZE; x++)
		{
			BYTE currentByte = this->currentPureBlock.value().BlockArray[x];
			if (this->frequency.contains(currentByte))
			{
				this->frequency[currentByte] += 1;
			}
			else
			{
				this->frequency.insert({ currentByte,1 });
			}
		}
	}
}

