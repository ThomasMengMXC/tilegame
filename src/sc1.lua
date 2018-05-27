local scene = require("scene")

local sc1 = setmetatable({
	props = {
		data = {},
		screen = {},
	},
}, scene)

function sc1.update(props)
	props.screen:draw()
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
end

function sc1.departure(props)
	props.data = {}
end

return sc1
