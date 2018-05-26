function update(props)
	return nil
end

function keyboard(props, ch)
	-- exit if q or Q is pressed
	if ch == 113 or ch == 81 then return -2 end
	return -1
end

function arrival(props)
	local map = require("map")
	props.scr = Screen.new(0, 0)
	local layer = props.scr:add_layer(0, 0, 0, 0)
	props.mapLayer = layer
	layer = props.scr:add_layer(0, 0, 0, 0)
	props.cursorLayer = layer
	layer = props.mapLayer

	for x = 0, 40 do
		for y = 0, 24 do
			layer:add_colour(y, x, {r = 0, g = 255, b = 0, a = 255})
			layer:add_icon(y, x, ". ", 2);
		end
	end
	props.scr:draw()
	return nil
end

function departure(props)
	return nil
end

function create_data(props)
	local map = require("map")
	setmetatable(props, {
		__index = {
			data = setmetatable({}, {
				__index = {
					map = map:new(40, 25),
				}
			}),
			screen = {}, -- this will be overridden by a c userdata
			cursor = {},
		}
	})
end
