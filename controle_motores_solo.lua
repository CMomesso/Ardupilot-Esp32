local port = serial:find_serial(0)

if not port then
    gsc:send_text(0, "Erro de comunicação")
    return
end

port:begin(57600)
port:set_flow_control(0)        -- desativa o controle de fluxo RTS/CTS

-- enviando um pacote binário
function write_byte(b)
    port:write(b)
end

function send_packet(habilitado, velocidade, direcao)
    local start = 0xAA -- Byte que marca o início do pacote (ESP32 utiliza esse byte para saber onde começa uma mensagem nova)

    local checksum = (habilitado + velocidade + direcao) % 256

    write_byte(start)
    write_byte(habilitado)
    write_byte(velocidade)
    write_byte(direcao)
    write_byte(checksum)
end


function update()

    local ch1 = rc:get_pwm(1)
    local ch3 = rc:get_pwm(3)
    local ch6 = rc:get_pwm(6)

    if not ch1 or not ch3 or not ch6 then
        gcs:send_text(6, "Ch6 sem sinal")
        return update, 500
    end

    local habilitado = 0

    if ch6 > 1500 then
        habilitado = 1
        gcs:send_text(6, "Ligado")
    else
        habilitado = 0
    end

    local velocidade = 0

    if ch3 < 1050 then
        velocidade = 0
    elseif ch3 < 1300 then
        velocidade = 1
    elseif ch3 < 1700 then
        velocidade = 2
    else
        velocidade = 3
    end

    local direcao = 1

    if ch1 < 1300 then
        direcao = 0
    elseif ch1 > 1700 then
        direcao = 2
    else
        direcao = 1
    end

    send_packet(habilitado, velocidade, direcao)

    return update, 200
end

return update()
