local scene = require("scene")

local sc1 = setmetatable({
	props = {
		data = {},
		screen = {},
	},
}, scene)

function sc1.update(props)
	local data = props.data
	local map = props.data.map
	for y = 0, map.yLength do
		for x = 0, map.xLength do
			local col = map[y][x].colour
			if ((data.frame + x) % map.yLength == 0) then
				col.r = 255
			else
				col.r = 0
			end
		end
	end
	map:draw(props.data.mapLayer)
	props.screen:draw()
	data.frame = data.frame + 1
end

function sc1.keyboard(props, ch)
	if ch == 113 or ch == 81 then return 0 end
end

function sc1.arrival(props)
	local map = require("map")
	local scr = Screen.new(0, 0)
	props.screen = scr
	local data = props.data
	data.map = map:new(25, 40)
	data.mapLayer = scr:add_layer(0, 0, 25, 40)
	data.rangeLayer = scr:add_layer(0, 0, 25, 40)
	data.cursorLayer = scr:add_layer(0, 0, 0, 0)
	data.map.draw(data.map, data.mapLayer)
	data.frame = 0
end

function sc1.departure(props)
	props.data = {}
end

return sc1
