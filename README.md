# Códigos para o esp32dev-lorawan
Repositório com um código de exemplo de uso do LoRa no IoT DevKit - LoRaWAN

### Tabela de conteúdos

- [Requisitos](#requisitos)
- [Envio de temperatura para a TTN](#envio-de-temperatura-para-a-ttn)
  - [Preparativos](#preparativos)
  - [Compilando e gravando](#compilando-e-gravando)

## Requisitos

É necessário ter o PlatformIO instalado de forma a poder compilar e gravar na placa os códigos de exemplo.

Caso esteja usando a distribuição Linux **openSUSE Tumbleweed**, é possível instalar o PlatformIO com o seguinte comando:

```bash
sudo zypper install python311-platformio
```

Caso esteja usando outra distribuição Linux:

 -  para Python 3.11 ou superior:

```bash
pipx install platformio
```

 - para Python 3.10 ou inferior:

```bash
pip install platformio
```

O binário é instalado em `~/.local/bin/pio`.

[Neste link](https://github.com/jpmsb/preparando-computador-para-engenharia-de-tele/blob/main/guias-de-aplicacoes/PlatformIO.md) há algumas dicas sobre o uso do PlatformIO que podem ser úteis.

## Envio de temperatura para a TTN

### Preparativos

Primeiramente, clone esse repositorio:

```bash
git clone https://github.com/jpmsb/esp32dev-lorawan
```

Em seguida, entre no diretório do projeto:

```bash
cd esp32dev-lorawan/LoRaWan-TTN-Temperatura
```

Ajuste as variáveis `appeui` e `appkey` no arquivo [`gravar`](LoRaWan-TTN-Temperatura/gravar) com as chaves de aplicação e de dispositivo fornecidas pela TTN, ficando parecido com o exemplo abaixo:

```bash
# APPEUI e APPKEY para conexão na TTN
appeui="0000000000000001"
appkey="ABCDEFGHIJKLMNOPQRSTUVWXYZ123456"
```

O valor de `appeui` deve conter **16** caracteres e o valor de `appkey` deve conter **32** caracteres.

Opcionalmente, você pode ajustar o intervalo em que a temperatura é envada para a TTN ajustando a variável `intervalo_envio` e pode ajustar o tempo limite para reconexão ajustando a variável `intervalo_desconectado`. Abaixo, são mostrados os valores já definidos para essas variáveis:

```
# Intervalos em segundos
intervalo_envio=60           # 1 minuto entre cada envio
intervalo_desconectado=300   # 5 minutos de limitar máximo de desconexão
```

### Compilando e gravando

Após ajustar o arquivo `gravar`, execute o script `gravar` para compilar e gravar o código na placa:

```bash
./gravar
```

Caso tudo ocorra de acordo, você passará a observar a saída serial no terminal. Verifique na TTN se os dados estão chegando.