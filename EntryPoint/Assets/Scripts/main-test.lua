local ref = GetSelf()
local keke_speed = 3000
local rotation_speed = 110


function velocity_test(delta_time)
    if (ref.components.rigid_body) then
        local direction = vec2:new()
        if (IsButtonPressed(KeyCode.KEY_W)) then
            direction.y = 1
        end
        
        if (IsButtonPressed(KeyCode.KEY_A)) then 
            direction.x = -1
        end
        
        if (IsButtonPressed(KeyCode.KEY_S)) then 
            direction.y = -1
        end
        
        if (IsButtonPressed(KeyCode.KEY_D)) then 
            direction.x = 1
        end
        direction = Normalize2(direction)

        ref.components.rigid_body.velocity.x = direction.x * keke_speed * delta_time;
        ref.components.rigid_body.velocity.y = direction.y * keke_speed * delta_time;
    else
        print("No rigid body component")
    end
end

function collision_start(ent)
    print("COLLISION START")
    -- UnbindBeginCollisionFunction()
    -- entTag = 
    if (ent.components.tag) then
        if (ref.components.worldspace_text and ent.components.tag) then
            ref.components.worldspace_text.display_text = ent.components.tag.text

            
            local pomelo_ref = GetEntityWithTag("P")
            print("COLLISION START2")
            print(pomelo_ref.components.transform)

            pomelo = CreateEntityWithComponents(pomelo_ref.components)
            pomelo.components.transform.position = ent.components.transform.position

            DestroyEntity(ent)
        else
            print("No worldspace_text compoent")
        end
    end
end

function collision_end(ent)
    print("COLLISION END")
end

function OnCreate()
    BindBeginCollisionFunction("collision_start")
    print("begin collision added")
    BindEndCollisionFunction("collision_end")
    print("end collision added")
end

function Update()
    ref = GetSelf()

    delta_time = GetDeltaTime()
    -- print(ref.rigid_body.mass)
    -- print(ref.rigid_body.type)
    -- print(ref.sprite.color.x)
    velocity_test(delta_time)
    -- transform_test(delta_time)
    -- sprite_test(delta_time)
    -- rigid_body_test(delta_time)

end
