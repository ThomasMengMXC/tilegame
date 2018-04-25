extern "C" {
#include <theatre/layer.h>
#include <theatre/colour.h>
}

#include <cstring>
#include <climits>

#include "map.h"	// it's own .h file
#include "button.h"
#include "hover.h"

Map::Map(void) {
	this->yLength = 25; // TEMPORARY VALUE
	this->xLength = 40; // TEMPORARY VALUE
	this->grid = init_grid();
}

Map::~Map(void) {
	free_grid();
}

Tile **Map::init_grid(void) {
	Tile *tile = NULL;
	Tile **grid = new Tile*[this->yLength];
	for (int y = 0; y < this->yLength; y++) {
		grid[y] = new Tile[this->xLength];
		for (int x = 0; x < this->xLength; x++) {
			tile = &(grid[y][x]);
			std::strncpy(tile->icon, ". ", 2);

			tile->colour.r = 0;
			tile->colour.g = 135;
			tile->colour.b = 0;
			tile->colour.a = 255;

			tile->mvCost = 1;
			tile->unit = NULL;
			tile->yPos = y; tile->xPos = x;
		}
	}
	return grid;
}

void Map::free_grid(void) {
	for (int y = 0; y < this->yLength; y++) {
		delete[] grid[y];
	}
	delete[] grid;
}

void Map::map_draw(Layer *layer) {
	for (int y = 0; y < this->yLength; y++) {
		for (int x = 0; x < this->xLength; x++) {
			Tile *tile = &(this->grid[y][x]);
			add_icon_to_layer(layer, y, x, tile->icon, 2);
			add_colour_to_layer(layer, y, x, tile->colour);
			add_hover_to_layer(layer, y, x, map_hover);
			if (this->grid[y][x].unit) {
				add_icon_to_layer(layer, y, x, tile->unit->icon, 2);
				add_button_to_layer(layer, y, x, unit_button);
			}
		}
	}
}

void Map::add_team_to_map(std::vector<Unit *> team) {
	for (size_t i = 0; i < team.size() ; i++) {
		unsigned int yPos = 0;
		unsigned int xPos = i;
		this->grid[yPos][xPos].unit = team[i];
	}
}
