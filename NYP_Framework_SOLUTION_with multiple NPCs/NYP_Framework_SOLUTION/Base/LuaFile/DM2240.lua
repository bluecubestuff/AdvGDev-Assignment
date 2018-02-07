title = "A2-Lua"

-- APPLICATION
width = 1024
height = 720

-- ENEMY
max_enemies = 5


-- PLAYER
forward = "W"
back = "S"
right = "D"
left = "A"

reload = "R"
grenade = "G"


Services.Input:addEventListener(nil, onMouseMoveEvent, InputEvent.EVENT_MOUSEMOVE)
Services.Input:addEventListener(nil, onMouseDownEvent, InputEvent.EVENT_MOUSEDOWN)

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
