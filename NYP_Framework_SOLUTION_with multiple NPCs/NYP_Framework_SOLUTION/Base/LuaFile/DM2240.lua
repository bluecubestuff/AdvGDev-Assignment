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
legHP = 10000
torsoHP = 10000

-- Weapons
-- EG -> enemyGun
EG_magRounds = 100000000
EG_maxMagRounds = 100000000
EG_totalRounds = 100000000
EG_maxTotalRounds = 100000000
EG_timeBetweenShots = 3

grenade_magRounds = 10
grenade_maxMagRounds = 10
grenade_totalRounds = 100
grenade_maxTotalRounds = 100
grenade_timeBetweenShots = 0.333

laser_magRounds = 5
laser_maxMagRounds = 5
laser_totalRounds = 10000
laser_maxTotalRounds = 10
laser_timeBetweenShots = 0.1667

pistol_magRounds = 8
pistol_maxMagRounds = 8
pistol_totalRounds = 40
pistol_maxTotalRounds = 40
pistol_timeBetweenShots = 0.3333

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
