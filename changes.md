# Proposed changes to the structure
## Main Components
+ map
+ overmap
+ unit
+ gui
+ mapL

## How it currently is
The mapL is invoked by overmap to create the map using the Lua script. The overmap can then get the map from this luastate using a function exposed to it by the header files The map object is then used by overmap to do stuff like draw it and interact with it. The overmap is essentially the glue between the player and the game's data.

The unit is something that the overmap can give the map access to (I haven't implemented this yet) or be created via the script.

The gui literally does nothing.

## Things I can change
This part is mainly about integrating the GUI better with the other components and to put things in more appropriate places.

The mapL still loads the script to create the map and add the enemies to their respective factions but now the overmap can add players to the base faction (aka, the faction that the user plays as).

The gui should probably be renamed the hud but the terms are interchangeable so whatever. The gui is owned by the overmap and receives information from the overmap so that it can draw stuff to the screen. The gui will recieve a layer from the overmap that it can draw onto. Every subseqent call shall be from the overmap and will tell the gui what unit to draw.
