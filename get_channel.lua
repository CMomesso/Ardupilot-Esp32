function update()
    for ch = 1, 10 do

        local pwm = rc:get_pwm(ch)

        if pwm then
            gcs:send_text(7, 'CH' .. ch .. " = " .. pwm)
        end
    
    return update, 1000
end

return update()
