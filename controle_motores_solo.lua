local port = serial:find_serial(0)

if not port then
    gsc:send_text(0, "Erro de comunicação")
    return
end

port:begin(57600)
port:set_flow_control(0)        -- desativa o controle de fluxo RTS/CTS

function update()

    local pwm6 = rc:get_pwm(6)

    if not pwm6 then
        gcs:send_text(6, "Ch6 sem sinal")
        return update, 500
    end

    if pwm6 > 1500 then
        port:write(string.byte("A"))
        gcs:send_text(6, "Ligado")
    else
        port:write(string.byte("C"))
        gcs:send_text(6, "Desligado")
    end
    return update, 200
end

return update()
