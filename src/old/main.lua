function update(stage)
end

function keyboard(stage, ch)
	if ch == string.byte("q") or ch == string.byte("Q") then
		return -2
	end
end

function arrival(stage)
	setmetatable(_G, {
		__newindex = function(t, k, v)
			-- haha nice try
		end
	})
end

function departure(stage)
end

function secure_require(modname)
	local tmp_ENV = _ENV
	local tmp_require = require
	_ENV = {
		io = {stderr = io.stderr}, -- this is EVIL
		tostring = tostring,
		setmetatable = setmetatable,
	}
	local ret = tmp_require(modname)
	_ENV = tmp_ENV
	return ret
end
