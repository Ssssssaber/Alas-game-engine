keke_speed = 100
rotation_speed = 110

function velocity_test(delta_time)
    -- velocity = vec2:new()
    velocity = GetVelocity()
    if (velocity) then
        if (IsButtonPressed(KeyCode.KEY_W)) then
            velocity.y = velocity.y + keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_A)) then 
            velocity.x = velocity.x - keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_S)) then 
            velocity.y = velocity.y - keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_D)) then 
            velocity.x = velocity.x + keke_speed * delta_time
        end
        SetVelocity(velocity)
    else
        print("No rigid body component")
    end
end


function transform_test(delta_time)
    transform = GetTransform()
    -- print(transform.rotation)
    if (transform) then
        if (IsButtonPressed(KeyCode.KEY_W)) then
            transform.position.y = keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_A)) then 
            transform.position.x = -keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_S)) then 
            transform.position.y = -keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_D)) then 
            transform.position.x = keke_speed * delta_time
        end

        transform.rotation = delta_time * rotation_speed
        transform.scale.x = delta_time * rotation_speed
        SetTransform(transform)
    else
        print("No transform component")
    end
end

function sprite_test(delta_time)
    sprite = GetSprite()
    -- print(transform.rotation)
    if (sprite) then
        sprite.color.x = delta_time
        sprite.color.y = delta_time
        SetSprite(sprite)
    else
        print("No sprite component")
    end
end

function rigid_body_test(delta_time)
    rigid_body = GetRigidBody()
    -- print(transform.rotation)
    if (rigid_body) then
        rigid_body.velocity.x = delta_time
        rigid_body.velocity.y = -delta_time
        SetRigidBody(rigid_body)
    else
        print("No rigid_body component")
    end
end

function collision_start()
    print("COLLISION START")
    -- UnbindBeginCollisionFunction()
    rigid_body = GetRigidBody()
    -- print(transform.rotation)
    if (rigid_body) then
        rigid_body.velocity.x = 50
        rigid_body.velocity.y = 10
        SetRigidBody(rigid_body)
    else
        print("No rigid_body component")
    end
end

function collision_end()
    print("COLLISION END")
    -- UnbindEndCollisionFunction()
end

function OnCreate()
    BindBeginCollisionFunction("collision_start")
    print("begin collision added")
    BindEndCollisionFunction("collision_end")
    print("end collision added")
end

function Update()
    delta_time = GetDeltaTime()
    
    velocity_test(delta_time)
    -- transform_test(delta_time)
    -- sprite_test(delta_time)
    -- rigid_body_test(delta_time)

end
