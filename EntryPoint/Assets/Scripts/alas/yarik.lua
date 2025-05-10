keke_speed = 100
rotation_speed = 110

function handle_velocity(delta_time)
    velocity = GetVelocity()
    if (velocity) then
        if (IsButtonPressed(KeyCode.KEY_W)) then
            velocity.y = keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_A)) then 
            velocity.x = keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_S)) then 
            velocity.y = keke_speed * delta_time
        end
        
        if (IsButtonPressed(KeyCode.KEY_D)) then 
            velocity.x = keke_speed * delta_time
        end
        SetVelocity(velocity)
    else
        print("No rigid body component")
    end
end

function collision_start()
    print("COLLISION START")
    -- UnbindBeginCollisionFunction()

    worldspace_text = GetWorldspaceText()

    if (worldspace_text) then
        worldspace_text.display_text = "COLLISION START"
        SetWorldspaceText(worldspace_text)
    else
        print("No worldspace_text compoent")
    end

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

    worldspace_text = GetWorldspaceText()

    if (worldspace_text) then
        worldspace_text.display_text = "COLLISION END"
        SetWorldspaceText(worldspace_text)
    else
        print("No worldspace_text compoent")
    end

    overlay_text = GetOverlayText()

    if (overlay_text) then
        overlay_text.display_text = "COLLISION DETECTED"
        SetOverlayText(overlay_text)
    else
        print("No overlay_text compoent")
    end
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
