-- Envia o caractere "B" pela porta serial da Pixhawk para o ESP32

local port = serial:find_serial(0)

if not port then
    gcs:send_text(0, "ERRO: porta serial scripting nao encontrada")
    return
end

port:begin(57600)
port:set_flow_control(0)

local contador = 0

function update()
    port:write(string.byte("B"))

    contador = contador + 1
    gcs:send_text(6, "Enviado B para ESP32: " .. contador)

    return update, 1000 -- repete a cada 1000 ms
end

return update, 1000