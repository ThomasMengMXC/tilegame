return {
	win = function()
		if Map:getCount("Enemies") == 0 then
			return true
		else
			return false
		end
	end,
	map = "forest.lua", -- the map is loaded before the builder function
	starting = {
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1},
	},
	builder = function()
		local tmpUnit = {
			name = "Grug",
			icon = ";(",
			hp = 10,
			move = 5,
			str = 5,
			spd = 5,
			def = 5,
		}
		Map:newFaction("Heroes")
		Map:newFaction("Enemies")
		Map:addUnit(tmpUnit, "Enemies", 3, 3)
	end,
}
