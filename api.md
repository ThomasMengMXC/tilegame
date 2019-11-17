# Motivation
+ The API should facilitate the development of multiple "TRPG"/X-COM-like games. These are non-realtime (e.g. can't make a dota-like game).

# Similarities in genre (e.g. what should be part of the core)
+ Some sort of grid based system which units move on
+ Controlling multiple units via some sort of interaction
+ Battle with other (enemy) units
+ Units have stats and inventories
+ Factions which group units

# Differences within genre (e.g. what should be scriptable)
+ Stats and inventory restrictions for units
	- This will be addressed by defining a base unit in the form of a Lua table and having every other unit derived off of it.
	- This actually may be a very bad idea so I'm going to make the stats and inventories constant and if the dev wants to change it he/she must recompile the engine.

+ Battle system and damage calculations
	- This will be addressed by defining a callback function which takes in two units does whatever the dev wants.
	- The only way that this battle system callback could work is if the player is well defined, otherwise, there are too many variables to consider.

+ Interactions between skills and the environment
	- Skills will have full access to the APIs

+ Skills and level up system
	- Exp gain will be a callback and so will levelling up

+ Out of "level" systems
	- I don't know right now. Probably just store things in backstage

+ Map properties and type
	- The types of values a tile can store cannot be changed but their values can be. This will make it easier for the other API to coexist.

+ Who controls the units (e.g. the control can be taken away from player)
	- This might be a very bad idea, probably best to just let factions define this

+ Map hazards
	- The map will have callbacks which do stuff.

# Implementation
+ There will be a single "law.lua" file which defines all of the basic mechanics. It will include:
	- Most of the stats and their types are set in stone in C++, but all of the callbacks shall be in Lua, this will hopefully give both flexibility and rigidity in modding/creating the game.
	- The combat system: there are two types of combat, melee and ranged (like in CIV 5). In melee combat, the enemy can always attack back but in ranged combat, the enemy can attack back if and only if the enemy's melee attack can reach it.
	- The movement will be calculated in C++ using values which can be changed using Lua functions. More details in section below.
	- The laws file will also contain a mapping of the movement type to the tileType -> movement cost map.
	- There will be a billion different event callbacks for basically anything that can occur in the game.


Events:
+ Levelling up
+ Killing an enemy
+ Waiting
+ Using an item
+ Healing/Assisting an ally
+ Levelling skill levels
+ Pre/post combat (There will be a way to change the probabilities without changing the shown values. This is cheeky but I will need it for something)
+ Pre/post assist
+ After a location has been chosen
+ Time based events:
	- At the start of player phase
	- At the end of player phase
	- At the start of an enemy phase
	- At the end of an enemy phase

# Game mechanics

## Movement:
The movement will be calculated in C++ using the values a certain unit has. The values are the following:
+ The movement range
+ The cost of moving over a certain tile
+ The cost of moving through enemy ZOC (zone of control)
The reasons these values are inherent to a unit is so that they can change them using items. E.g. I'm planning to have a skill which ignores enemy ZOC like the lighting warfare policy in CIV 5.

## Zone of Control
Zone of Control (ZOC) is a mechanic I plan to steal from CIV 5. The idea is that units have to spend more movement points to move through tiles near an enemy. I'm planning to make it so that the ZOC will be negated if another unit is in the ZOC (how can an enemy exert ZOC on 4 units at once?). The ZOC will be sort of like an aura that a unit puts onto the map. The computations will be something like the steps below:
+ A certain unit is about to move
+ The enemy ZOCs are added but with the unit taken off the board. (This is so a single unit can't negate an enemy's ZOC)
+ The unit calculates its movement with the ZOCs in mind.

# Tiles
A map is made up of tiles. Each tile can store loads of information such as the following:
+ The name of the tile
+ The icon of the tile
+ The colour and alpha
+ A vector containing a boolean which says if a ZOC is present. The index of the vector is the unit's ID.
+ A vector containing shorts which represent the highest movement a unit can have at a certain location.
+ A vector of events.

## Game states
Player phase
- Default state
- Open menu
	- Melee -+
	- Ranged |- These four are added using preinstalled events
	- Item   |
	- Trade -+
	- Other actions such as skills can be added using *INSERT API HERE*
	- Wait (will always be the last option)
Enemy phase
- Cycle through the enemy factions


# C to Lua API
Map, a userdatum which controls the map
- `Map:load(filename)`, load a map, returns true if it was successful.
- `Map:clear()`, clear the map.
- `Map:new_faction(factionName)`, create a faction.
- `Map:add_unit(factionName, unit, y, x)`, add a copy of the unit to the faction and map. Returns the unit's ID.
- `Map:change_faction(unitID, factionName)`, changes unit to a new faction.
- `Map:faction_size(factionName)`, gets the amount of units in this faction.
- `Map:move_unit(yOld, xOld, yNew, xNew)` moves a unit from Old to New, returns false if this failed.
- `Map:event_tile(y, x, name, colour, event)` sets a tile to have a script, an additional option "name" will appear for units occupying this tile. Can optionally give the tile a colour.
- `Map:unit_traits(unitID, traits, bool)` sets the trait to be true or false
- `Map:unit_has_trait(unitID, trait)` returns the bool of the trait.

Unit, a table used by the addUnit method to create a unit
- name: string, max 19 characters
- icon: string, max 2 characters
- hp: number!int, the maximum hp
- move: number!int, the amount of tiles the unit can move.
- str: number!int, affects damage
- spd: number!int, affects evasion and attack speed
- def: number!int, affects damage reduction
- moveType: number!int, a number that denotes which tile costs this unit should subscribe to.

Colour, a table used by various things to denote a colour
- r: number!int, red
- g: number!int, green
- b: number!int, blue
- a: number!int, alpha

Event, a table which keeps track of events. Credit to Lua Programming Gems section 21 for giving me the idea
- `Event:subscribe(subscriber, eventName, callback)` gives subscriber an event called eventName and function callback
- `Event:PostEvent(eventName, data)` calls eventName and gives data

Callbacks, an event callback should be in the following form.
`function name(subscriber, data)` subscriber is just something to identify who is recieving the event and data is the event data.

## How a level can be created
1. A map condition must be chosen. This condition is a function which returns true if the player has won or false if the player has not.
2. A name of the map must be given relative to this level file.
3. The map will be created from this file.
4. An array (table) of 2d coordinates which are the starting locations of player units.
5. A function which creates factions, units and events must be given.
6. The skills for all units are loaded.
