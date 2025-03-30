-- Stage 2: Reading Simple Values
a = 5
b = 7
c = a + b
d = "OneLoneCoder.com"

-- Stage 3: Reading a simple object
player = {Title = "Squire", Name = "Ciaran", Family = "Wirral", Level = 20}

-- Stage 4: Calling a Lua Function
function CalledFromCPP1(r1, r2, r3)
	io.write("[LUA S4] CalledFromCPP1("..r1..", "..r2..", "..r3..")\n")
	return r1 * r2
end


-- Stage 5: Calling a C++ Function
function CalledFromCPP2(r1, r2)
	io.write("[LUA S5] CalledFromCPP2("..r1..", "..r2..")\n")
	io.write("[LUA S5] Calling HostFunction("..r1..", "..r2..")\n")
	r = HostFunction(r1, r2)
	io.write("[LUA S5] HostFunction returned "..r.."\n")
	return r * 15
end
	
-- Stage 7: New Stuff
function CreatePlayer1() 

	io.write("[LUA S7] CreatePlayer1() called\n")

	local p = Player:new()

	p.Name = "Saladin"
	p.Family = "Sheffield"
	p.Title = "King"
	p.Level = 35 

	return p
end

-- Stage 8: Create X players
function CreatePlayer2(n)

	io.write("[LUA S8] CreatePlayer2() called\n")

	allPlayers:clear()

	for i=1,n do
		allPlayers:add(CreatePlayer1())
	end
end