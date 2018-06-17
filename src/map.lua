local map = {}
local tile = {
	icon = ". ",
	colour = {r = 0, g = 255, b = 0, a = 255},
	mvCost = 1,
	pos = {x = 0, y = 0},
	unit = {}
}

function map:new(yLength, xLength)
	local obj = {}
	obj.yLength = yLength
	obj.xLength = xLength
	for y = 0, yLength do
		obj[y] = {}
		for x = 0, xLength do
			obj[y][x] = setmetatable({
				colour = {
					r = 0, b = 0, g = 0, a = 255,
				}
			}, {
				__index = tile
			})
		end
	end
	return setmetatable(obj, {__index = self})
end

function map:draw(layer)
	layer:clear()

	local hover = function (props, start, y, x)
		local lyr = props.cursorLayer
		if (start) then
			lyr:add_colour(y, x, {r = 255, g = 255, g = 255, a = 128})
		else
			lyr:remove_colour(y, x)
		end
	end

	for y = 0, self.yLength do
		for x = 0, self.xLength do
			local tile = self[y][x]
			layer:add_icon(y, x, tile.icon, 2)
			layer:add_colour(y, x, tile.colour)
			layer:add_hover(y, x, hover)
		end
	end
end

return map
