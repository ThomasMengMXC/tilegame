function update(stage)
	local currentScene = stage.currentScene
	currentScene.update(currentScene.props)
end

function keyboard(stage, ch)
	local currentScene = stage.currentScene
	local ret = currentScene.keyboard(currentScene.props, ch)
	if type(ret) == "number" then
		if ret > 0 then
			change_scene(stage, ret)
		else
			currentScene.departure(currentScene.props)
			return -2
		end
	end
	return -1
end

function arrival(stage)
	setmetatable(_G, {
		__newindex = function(t, k, v)
			-- haha nice try
		end
	})
	setmetatable(stage, {
		__index =  {
			scenes = {}, -- an array of all the scenes
			currentScene = {}, -- a pointer for the current Scene
			backstage = {}, -- for communication between scenes
		}
	})
	local order = secure_require("order")
	load_scenes(stage, order)
end

function departure(stage)
end

function secure_require(modname)
	local tmp_ENV = _ENV
	_ENV = {
		require = require,
		io = {stderr = io.stderr}, -- this is EVIL
		Screen = Screen,
		Layer = Layer,
		tostring = tostring,
		setmetatable = setmetatable,
	}
	local ret = require(modname)
	_ENV = tmp_ENV
	return ret
end

function load_scenes(stage, order)
	stage.scenes = {}
	local scene = 0
	for i, v in pairs(order) do
		scene = secure_require(v)
		if scene.props then
			scene.props.backstage = stage.backstage
			table.insert(stage.scenes, scene)
		end
	end
	change_scene(stage, 1)
end

function change_scene(stage, sceneNum)
	local currentScene = stage.currentScene

	if currentScene.departure and currentScene.props then
		currentScene.departure(currentScene.props)
	end

	stage.currentScene = stage.scenes[sceneNum]
	currentScene = stage.currentScene

	if currentScene.arrival then
		currentScene.arrival(currentScene.props)
	end
end
