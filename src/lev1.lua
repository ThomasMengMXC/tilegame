return {
	map = { -- 4x4 grid
		"....",
		"....",
		"....",
		"..X.",
	},

	tiles = {
		["."] = {
			icon = ". ",
			r = 0,
			g = 255,
			b = 0,
			a = 255,
			mvCost = 1,
		},
		["X"] = {
			icon = "><",
			r = 0,
			g = 255,
			b = 0,
			a = 255,
			mvCost = 1,
		},
	},
}
