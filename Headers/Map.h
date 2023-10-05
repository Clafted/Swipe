#pragma once

#ifndef MAP_H
#define MAP_H

#include "TileWarning.h"
#include <vector>

class Map
{
public:
	std::vector<TileWarning*> tileWarnings;
	Texture texture, topTexture;
	int width;
	const int tileWidth;
	

	Map(std::string texturePath, std::string topTexturePath, int width, int tileWidth = 50)
		:texture(texturePath), topTexture(topTexturePath), width(width), tileWidth(tileWidth)
	{
		// Add a tile warning for each tile on the Map Object
		for (int i = 0; i < width * width; i++)
		{
			tileWarnings.push_back(new TileWarning(((i % width) * tileWidth) - (((width - 1) * tileWidth) / 2.0f), ((i / -width) * tileWidth) + (((width - 1) * tileWidth) / 2.0f)));
		}
	}

	~Map()
	{
		for (TileWarning* warning : tileWarnings)
		{
			delete warning;
		}
		tileWarnings.clear();
	}
};

#endif 
