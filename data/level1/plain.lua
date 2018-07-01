return {
	map = { -- 4x4 grid
		"....",
		"....",
		"..T.",
		".T^T",
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
		["^"] = {
			icon = "/\\",
			r = 220,
			g = 220,
			b = 220,
			a = 255,
			mvCost = 10,
		},
		["T"] = {
			icon = "77",
			r = 0,
			g = 200,
			b = 0,
			a = 255,
			mvCost = 2,
		}
	},
}
