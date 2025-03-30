-- Stage 4: Calling a Lua Function

function CPP_Update()
    velocity = vec2:new()
	velocity.x = 0.001
    velocity.y = -0.001

    return velocity
end