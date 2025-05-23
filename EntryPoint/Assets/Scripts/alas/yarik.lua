local ref = GetSelf()
local keke_speed = 300
local score = 0
local pomelo_ref = GetEntityWithTag("p")
local trees_ref = {}
local jumping = false
local jump_speed = keke_speed * 10
trees_ref[1] = GetEntityWithTag("t1")
trees_ref[2] = GetEntityWithTag("t2")
trees_ref[3] = GetEntityWithTag("t3")

function handle_velocity(delta_time)
    if (ref.components.rigid_body) then
        local direction = vec2:new()
        ref.components.rigid_body.velocity = vec2:new()
        if (IsButtonPressed(KeyCode.KEY_W) and not jumping) then
            ref.components.rigid_body.velocity.y = jump_speed * delta_time
            jumping = true
        end
        
        if (IsButtonPressed(KeyCode.KEY_A)) then 
            direction.x = -1
        end
        
        if (IsButtonPressed(KeyCode.KEY_D)) then 
            direction.x = 1
        end

        ref.components.rigid_body.velocity.x = direction.x * keke_speed * delta_time;
        ref.components.transform.rotation.z = 0;
    else
        print("No rigid body component")
    end
end

function collision_start(ent)
    print("COLLISION START")
    if (ent.components.tag) then
        if (ent.components.tag.text == "p") then
            if (ent.components.sprite.color.x > 0.5) then
                score = score + 1
            else
                score = score - 1
            end
            ref.components.overlay_text.display_text = string.format("Score: %d", score) 
            DestroyEntity(ent)
        end
        if (ent.components.tag.text == "G") then
            jumping = false;
        end
    end
end

function create_pomelo(tree_ref)
    pomelo = CreateEntityWithComponents(pomelo_ref.components)
    pomelo.components.transform.position = tree_ref.components.transform.position
    -- pomelo.components.transform.position.x = pomelo.components.transform.position + 100
    pomelo.components.transform.position.x = pomelo.components.transform.position.x + math.random(-50, 50)
    pomelo.components.sprite.color.x = math.random()
    pomelo.components.transform.position.z = 1
    -- print()
end

function collision_end(ent)
    print("COLLISION END")
end

function OnCreate()
    BindBeginCollisionFunction("collision_start")
    print("begin collision added")
    -- BindEndCollisionFunction("collision_end")
    -- print("end collision added")
end

time_passed = 0
local pomelo_cooldown = 1

function pomelo_spawner(delta_time)
    -- print(delta_time)
    time_passed = time_passed + delta_time
    -- print(time_passed)
    if (time_passed > pomelo_cooldown) then
        time_passed = 0
        create_pomelo(trees_ref[math.random( #trees_ref )])
    end
end

function Update()

    delta_time = GetDeltaTime()
    pomelo_spawner(delta_time)
    handle_velocity(delta_time)
end
