function CPP_Update()
    cpp_update() -- Call C++ to update state
    
    -- Now read the updated values
    local mass = cpp_get_mass()
    local velocity = cpp_get_velocity()
    local position = cpp_get_position()
    
    -- Use these values for further logic
end