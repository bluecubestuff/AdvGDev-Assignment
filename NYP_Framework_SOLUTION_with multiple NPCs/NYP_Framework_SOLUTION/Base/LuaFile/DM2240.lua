title = "A2-Lua"

-- APPLICATION
width = 1024
height = 720

-- ENEMY
max_enemies = 5

-- KEY
forward = "W"
back = "S"
right = "D"
left = "A"
reload = "R"
grenade = "G"

-- MECH
legHP = 100
torsoHP = 100

-- Weapons
-- EG -> enemyGun
EG_magRounds = 100000000
EG_maxMagRounds = 100000000
EG_totalRounds = 100000000
EG_maxTotalRounds = 100000000
EG_timeBetweenShots = 3




--[[
function SaveToLuaFile(outputString, overwrite)
	print("SaveToLuaFile...")
	local f;					-- The file
	if overwrite == 1 then		-- Wipe the contents with new data
		f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
	elseif overwrite == 0 then 	-- Append with new data
		f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
	end
	-- Write to the file
	f:write(outputString)
	-- Close the file
	f:close()
	print("OK")
end
--]]
