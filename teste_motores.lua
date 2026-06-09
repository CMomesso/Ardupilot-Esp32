local port = serial:find_serial(0)

if not port then
    gsc:send_text(0, "Erro de comunicação")
    return
end

port:begin(57600)
port:set_flow_control(0)        -- desativa o controle de fluxo RTS/CTS

local contador = 0

function update()
    port:write(string.byte("B"))

    contador = contador + 1
    gsc:send_text(7, "Enviando comando para o ESP32: " .. contador)

    return update, 1000
end

return update, 1000

