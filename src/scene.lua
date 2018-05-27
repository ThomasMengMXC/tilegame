local scene = {
	props = setmetatable({}, {__newindex = function() end}),

	update = function(props)
	end,

	keyboard = function(props, ch)
	end,

	arrival = function(props)
	end,
	
	departure =  function(props)
	end,
}

return scene
