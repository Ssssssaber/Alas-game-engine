keke_speed = 5
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


        -- if (IsButtonPressed(KeyCode.KEY_W)) then
        --     velocity.y = keke_speed * delta_time
        -- end
        
        -- if (IsButtonPressed(KeyCode.KEY_A)) then 
        --     velocity.x = -keke_speed * delta_time
        -- end
        
        -- if (IsButtonPressed(KeyCode.KEY_S)) then 
        --     velocity.y = -keke_speed * delta_time
        -- end
        
        -- if (IsButtonPressed(KeyCode.KEY_D)) then 
        --     velocity.x = keke_speed * delta_time
        -- end
        
        SetVelocity(velocity)
    else
        io.write("No rigid body component")
    end
end


function transform_test(delta_time)
    transform = GetTransform()
    -- io.write(transform.rotation)
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
        io.write("No transform component")
    end
end

function sprite_test(delta_time)
    spite1 = GetSprite()
    -- io.write(transform.rotation)
    if (spite1) then
        sprite1.color.x = delta_time
        sprite1.color.y = delta_time
        SetSprite(sprite1)
    else
        io.write("No sprite component")
    end
end

function rigid_body_test(delta_time)
    rigid_body = GetRigidBody()
    -- io.write(transform.rotation)
    if (rigid_body) then
        rigid_body.velocity.x = delta_time
        rigid_body.velocity.y = -delta_time
        SetRigidBody(rigid_body)
    else
        io.write("No rigid_body component")
    end
end

function Update()    
    delta_time = GetDeltaTime()
    
    velocity_test(delta_time)
    -- transform_test(delta_time)
    -- sprite_test(delta_time)
    -- rigid_body_test(delta_time)

end