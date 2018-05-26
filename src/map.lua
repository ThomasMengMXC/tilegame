map = {}

function map:new(yLength, xLength)
	local obj = {}
	obj.yLength = yLength
	obj.xLength = xLength
	for y = 0, yLength do
		obj[y] = {}
		for x = 0, xLength do
			obj[y][x] = setmetatable({}, {__index = {
				icon = ". ",
				colour = setmetatable({}, {__index = {
					r = 0, g = 0, b = 0, a = 0
				}}),
				mvCost = 1,
				pos = setmetatable({}, {__index = {x = 0, y = 0}}),
				unit = {}
			}})
		end
	end
	return setmetatable({}, {__index = obj})
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

	for y = 0, map.yLength do
		for x = 0, map.xLength do
			local tile = map[y][x]
			layer:add_icon(y, x, tile.icon, 2)
			layer:add_colour(y, x, tile.colour)
			layer:add_hover(y, x, hover)
		end
	end
end

return map
