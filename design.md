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
- spd: number!int, affects evasion
- def: number!int, affects damage reduction

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

# Game states
Player phase
- Default state
- Open menu
	- Melee
	- Ranged
	- Item
	- Trade
	- Other actions such as skills can be added using *INSERT API HERE*
	- Wait (will always be the last option)
Enemy phase
- Cycle through the enemy factions

# Loading a "game"
A game file must be chosen, this will be a file which describes game how the game will be run. This includes but is not limited to: skills, map names *insert more here later*.

# How a level can be created
1. A map condition must be chosen. This condition is a function which returns true if the player has won or false if the player has not.
2. A name of the map must be given relative to this level file.
3. An array (table) of 2d coordinates which are the starting locations of player units.
4. A function which creates factions, units and events must be given.
