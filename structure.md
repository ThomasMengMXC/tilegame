# The structure of files and stuff

## Main
Main just sets up ncurses, the backstage, the stage, creating the scenes and starting the show
### Relies on:
+ overmap
+ backstage

## Backstage
This is a class which contains the data that will persist between scenes.
### This contains:
+ The list of player owned units
+ The amount of money.
### This does:
+ Nothing by itself
### Relies on:
+ unit

## Unit
This is a class which contains the data for a gameplay unit.
### This contains:
+ the unit's name and icon
+ its current hp
+ a pointer to the tile it is currently standing on
+ the stats
+ a mapping between tile types and movement costs
+ the factionID for faction it is currently in
+ the unitID which is unique within the entire game
### This does:
+ Nothing by itself
### Relies on:
+ tile
+ stats

## Tile
This is a class which contains the data for a single tile on the map's grid.
### This contains:
+ the tile's icon
+ the type of tile (used by other classes in things like movement cost)
+ the position
+ The unit which is currently standing on the tile
### This does:
+ Nothing by itself
### Relies on:
+ unit :^) circular requiring

## Stats
This contains the stats of a unit.
### This contains:
+ the maximum hp
+ the movement range
+ strength
+ speed
+ def
### This does:
+ Nothing by itself
### Relies on:
+ Nothing

## Overmap
This is a scene and class which runs the each level defined by the lua files. This really should be called submap in the context of the game but I can't be bothered to change it around. This file contains the four functions required to create a scene, the update, keyboard, arrival and departure functions.
### Update:
This behaviour will be dependent on the phase. Currently this function does nothing but draw the screen.
### Keyboard:
This gets input from the keyboard and does stuff with it. Currently, this function just draws "!!" if left is pressend and exits the game if q or Q is pressed.
### Arrival:
Creates the screen and loads the overmap from the lua file in a set location. Then adds the map layer to the screen. Then draws the map.
### Departure:
Deletes everything that was created in arrival
### This does:
+ Running the game
+ Taking in the player input
+ Managing the game's state
### Relies on:
+ map
+ backstage
+ mapL

## Map
This is a class which handles the map and units moving around.
### This contains:
+ the grid of tiles
+ the list of units (really?)
+ the list of factions (why?)
+ the layer that it shall draw onto
### This does:
+ placing units onto the grid
+ moving units around the grid
+ checking the validity of the moves
+ creating movement grids for each player
