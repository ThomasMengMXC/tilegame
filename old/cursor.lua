local cursor = {
	canclick = true,
	icon = "  ",
	yPos = 0, xPos = 0,
	yOld = 0, xOld = 0,
}

function cursor:new(screen)
	local obj = {screen = screen}
	return setmetatable(obj, {__index = self})
end
