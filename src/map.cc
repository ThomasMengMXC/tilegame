extern "C" {
#include <theatre/layer.h>
#include <theatre/colour.h>
}

#include <cstring>
#include <climits>
#include <queue>

#include "map.h"	// it's own .h file
#include "button.h"
#include "hover.h"

// public
Map::Map(void) {
	Tile *tile = NULL;
	this->yLength = 25; // TEMPORARY VALUE
	this->xLength = 40; // TEMPORARY VALUE
	this->grid = new Tile*[this->yLength];
	for (int y = 0; y < this->yLength; y++) {
		grid[y] = new Tile[this->xLength];
		for (int x = 0; x < this->xLength; x++) {
			tile = &(grid[y][x]);
			std::memcpy(tile->icon, ". ", 3);

			tile->colour.r = 0;
			tile->colour.g = 135;
			tile->colour.b = 0;
			tile->colour.a = 255;

			tile->mvCost = 1;
			tile->unit = NULL;
			tile->pos.y = y; tile->pos.x = x;
		}
	}
}

Map::~Map(void) {
	for (int y = 0; y < this->yLength; y++) {
		delete[] this->grid[y];
	}
	delete[] this->grid;
	for (std::pair<Unit *, int16_t **> tmp: this->moveGrids) {
		free_move_grid(tmp.second);
	}
}

void Map::draw(Layer *layer) {
	clear_layer(layer);
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

void Map::place_team(std::vector<Unit *> *team) {
	Vector2D pos;
	pos.y = 0;
	pos.x = 0;
	for (size_t i = 0; i < team->size() ; i++) {
		this->place_unit((*team)[i], pos);
	}
}

bool Map::place_unit(Unit *unit, Vector2D pos) {
	pos = this->closest_without_unit(pos);
	Tile *tile = &(this->grid[pos.y][pos.x]);
	if (!tile->unit) {
		if (this->moveGrids.find(unit) == this->moveGrids.end()) {
			this->moveGrids[unit] = init_move_grid();
		}

		this->grid[pos.y][pos.x].unit = unit;
		unit->tile = &(this->grid[pos.y][pos.x]);
		return true;
	} else {
		return false;
	}
}

bool Map::remove_unit(Unit *unit, Vector2D pos) {
	Tile *tile = &(this->grid[pos.y][pos.x]);
	if (tile->unit) {
		tile->unit->tile = NULL;
		tile->unit = NULL;
		return true;
	} else {
		return false;
	}
}

bool Map::move_unit(Vector2D original, Vector2D destination) {
	Tile *orig = &(this->grid[original.y][original.x]);
	Tile *dest = &(this->grid[destination.y][destination.x]);
	if (orig->unit && !dest->unit) {
		orig->unit->tile = dest;
		dest->unit = orig->unit;
		orig->unit = NULL;
		return true;
	} else {
		return false;
	}
}

void Map::calculate_ranges(void) {
	for (std::pair<Unit *, int16_t **> tmp: this->moveGrids) {
		this->unit_range(tmp.first, tmp.second);
	}
}

Unit *Map::unit_at(Vector2D pos) {
	return this->grid[pos.y][pos.x].unit;
}

// private
int16_t **Map::init_move_grid(void) {
	int16_t **grid = new int16_t *[this->yLength];
	for (int y = 0; y < this->yLength; y++) {
		grid[y] = new int16_t[this->xLength];
		for (int x = 0; x < this->xLength; x++) {
			grid[y][x] = INT16_MIN;
		}
	}
	return grid;
}

void Map::free_move_grid(int16_t **grid) {
	for (int y = 0; y < this->yLength; y++) {
		delete[] grid[y];
	}
	delete[] grid;
}

void Map::unit_range(Unit *unit, int16_t **range) {
	for (int y = 0; y < this->yLength; y++) {
		for (int x = 0; x < this->xLength; x++) {
			range[y][x] = INT16_MIN;
		}
	}
	Vector2D pos, tmp;
	int16_t moveRange;
	std::queue<Vector2D> posQueue;
	std::queue<int16_t> moveQueue;
	posQueue.push(unit->tile->pos);
	moveQueue.push(unit->move);
	while (!posQueue.empty()) {
		pos = posQueue.front();
		moveRange = moveQueue.front();
		posQueue.pop();
		moveQueue.pop();
		if (range[pos.y][pos.x] < moveRange) {
			range[pos.y][pos.x] = moveRange;
		}
		if (moveRange < 1) continue;
		for (int i = 0; i < 4; i++) {
			tmp.y = pos.y + ((i + 0) % 2) * (i >> 1 ? +1 : -1);
			tmp.x = pos.x + ((i + 1) % 2) * (i >> 1 ? +1 : -1);
			if (tmp.y >= 0 && tmp.y < this->yLength &&
					tmp.x >= 0 && tmp.x < this->xLength) {
				posQueue.push(tmp);
				moveQueue.push(moveRange - this->grid[tmp.y][tmp.x].mvCost);
			}
		}
	}
}

Vector2D Map::closest_without_unit(Vector2D pos) {
	Vector2D tmp0, tmp1;
	int16_t **grid = init_move_grid(); // this isn't the intended use :^)
	std::queue<Vector2D> queue;
	queue.push(pos);
	while (!queue.empty()) {
		tmp0 = queue.front();
		queue.pop();
		if (tmp0.y >= this->yLength || tmp0.x >= this->xLength) continue;
		if (this->grid[tmp0.y][tmp0.x].unit == NULL) {
			pos = tmp0;
			break;
		}
		for (int i = 0; i < 4; i++) {
			tmp1.y = tmp0.y + ((i + 0) % 2) * (i >> 1 ? +1 : -1);
			tmp1.x = tmp0.x + ((i + 1) % 2) * (i >> 1 ? +1 : -1);
			if (tmp1.y >= 0 && tmp1.y < this->yLength &&
					tmp1.x >= 0 && tmp1.x < this->xLength
					&& grid[tmp1.y][tmp1.x] != 1) {
				grid[tmp1.y][tmp1.x] = 1;
				queue.push(tmp1);
			}
		}
	}
	free_move_grid(grid);
	return pos;
}

void Map::draw_move_grid(Layer *layer, Unit *unit) {
	int16_t **grid = moveGrids[unit];
	for (int y = 0; y < this->yLength; y++) {
		for (int x = 0; x < this->xLength; x++) {
			Colour col;
			col.r = 0;
			col.g = 0;
			col.b = 255;
			col.a = 128;
			if (grid[y][x] > INT16_MIN) {
				add_colour_to_layer(layer, y, x, col);
			}
		}
	}
}

void Map::remove_move_grid(Layer *layer, Unit *unit) {
	int16_t **grid = moveGrids[unit];
	for (int y = 0; y < this->yLength; y++) {
		for (int x = 0; x < this->xLength; x++) {
			if (grid[y][x] > INT16_MIN) {
				remove_colour_from_layer(layer, y, x);
			}
		}
	}
}
