/*******************************************************************************
* IoT DevKit - Primeiro contato com o modulo LoRaWAN Bee (v1.0)
* 
* Codigo usado para enviar comandos "AT" para o modulo e imprimir
* suas respostas.
* 
* Copyright 2020 RoboCore.
* Escrito por Giovanni de Castro (21/08/2020).
* Creditos: @francois - RoboCore
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version (<https://www.gnu.org/licenses/>).
*******************************************************************************/

//Verifica se o modelo de placa selecionado esta correto
#if !defined(ARDUINO_ESP32_DEV) // ESP32
#error Use this example with the ESP32
#endif

//Inclusao da biblioteca
#include "TemperatureHumidity.h"
#include "LDR.h"
#include "RoboCore_SMW_SX1276M0.h"
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <sstream>

//Declaracao dos pinos de comunicacao serial do Kit
HardwareSerial LoRaSerial(2);
#define RXD2 16
#define TXD2 17

//Criacao do objeto lorawan para a biblioteca SMW_SX1276M0
SMW_SX1276M0 lorawan(LoRaSerial);

CommandResponse resposta;

//Declaracao das variaveis que armazenam as informacoes da rede
const char appeui[] = APPEUI; // Application EUI
const char appkey[] = APPKEY; // Application Key

// Variáveis de tempo obtidas da variável de ambiente
const char intervalo_envio_char[] = INTERVALO_ENVIO;              // Intervalo de envio
unsigned int intervalo_envio = atoi(intervalo_envio_char) * 1000; // Intervalo de envio convertido

const char intervalo_desconectado_char[] = INTERVALO_DESCONECTADO;              // Intervalo de desconexão
unsigned int intervalo_desconectado = atoi(intervalo_desconectado_char) * 1000; // Intervalo de desconexão convertido

TemperatureHumidity temperatura;

//Declaracao da variavel que armazena o "ALIAS" da variavel da PROIoT
const char ALIAS[] = ""; //Altere essa variavel de acordo com o ALIAS configurado na plataforma

//Declaracao da funcao que verifica a conexao do modulo
void event_handler(Event);

//Declaracao das variaveis de intervalo de tempo
const unsigned long TEMPO_ESPERA = intervalo_envio; // 1 minuto
unsigned long intervalo;
unsigned long tempo_desconectado = millis() + intervalo_desconectado; // 1 minuto

bool reconnect = false;

void setup() {
  pinMode(13, OUTPUT);

  //Inicia o monitor serial e imprime o cabecalho
  Serial.begin(115200);
  Serial.println(F("--- SMW_SX1276M0 Bridge ---"));

  Serial.print(F("Intervalo de envio: "));
  Serial.print(intervalo_envio_char);
  Serial.println(F(" segundos"));
  
  Serial.print(F("Intervalo limite desconectado: "));
  Serial.print(intervalo_desconectado_char);
  Serial.println(F(" segundos"));

  //Definicao do pino de reset do modulo
  lorawan.setPinReset(5);
  lorawan.reset(); //Realiza um reset no modulo
  
  //Inicia a comunicacao serial com o modulo
  LoRaSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);

  //Associa a funcao que verifica a conexao do modulo ao objeto "lorawan"
  lorawan.event_listener = &event_handler;
  Serial.println(F("Evento configurado."));

  //Requisita e imprime o Device EUI do modulo
  char deveui[16];
  resposta = lorawan.get_DevEUI(deveui);

  if(resposta == CommandResponse::OK){
    Serial.print(F("DevEUI: "));
    Serial.write((uint8_t *)deveui, 16);
    Serial.println();
  } else {
    Serial.println(F("Erro ao obter o Device EUI"));
  }

  //Configura o Application EUI no modulo
  Serial.print(F("Configurando o Application EUI: "));
  Serial.write((uint8_t *)appeui, 16);
  Serial.println();

  resposta = lorawan.set_AppEUI(appeui);

  if(resposta == CommandResponse::OK){
    Serial.print(F("Application EUI configurado ("));
    Serial.write((uint8_t *)appeui, 16);
    Serial.println(')');
  } else {
    Serial.println(F("Erro ao configurar o Application EUI"));
  }

  //Configura o Application Key no modulo
  Serial.print(F("Configurando o Application Key: "));
  Serial.write((uint8_t *)appkey, 32);
  Serial.println();

  resposta = lorawan.set_AppKey(appkey);

  if(resposta == CommandResponse::OK){
    Serial.print(F("Application Key configurado ("));
    Serial.write((uint8_t *)appkey, 32);
    Serial.println(')');
  } else {
    Serial.println(F("Erro ao configurar o Application Key"));
  }

  //Condfigura o metodo de conexao como OTAA
  resposta = lorawan.set_JoinMode(SMW_SX1276M0_JOIN_MODE_OTAA);

  if(resposta == CommandResponse::OK){
    Serial.println(F("Metodo de Conexao Configurado como OTAA"));
  } else {
    Serial.println(F("Erro ao configurar o metodo OTAA"));
  }

  //Requisita conexao com a rede
  Serial.println(F("Conectando a Rede...\n"));
  lorawan.join();
    
}

void loop() {
  // Recebe informacoes do modulo
  lorawan.listen();

  if(lorawan.isConnected()){  // Verifica se o modulo esta conectado
    // Executa o envio da mensagem uma vez a cada 1 minuto
    if(intervalo < millis()){
      temperatura.measure();
      float temp = temperatura.getCurrentTemperature();

      Serial.print("Temperatura: ");
      Serial.print(temp);
      Serial.println(" C");
      
      // Cria o objeto dinamico "json" com tamanho "1" para a biblioteca
      DynamicJsonDocument json(JSON_OBJECT_SIZE(1));

      // Atrela ao objeto "json" a leitura do LDR com o nome do Alias definido
      json[ALIAS] = temp;

      // Mede o tamanho da mensagem "json" e atrela o valor somado em uma unidade ao objeto "tamanho_mensagem"
      size_t tamanho_mensagem = measureJson(json) + 1;

      // Cria a string "mensagem" de acordo com o tamanho do objeto "tamanho_mensagem"
      char mensagem[tamanho_mensagem];

      // Copia o objeto "json" para a variavel "mensagem" e com o "tamanho_mensagem"
      serializeJson(json, mensagem, tamanho_mensagem);

      // Convertendo temp de float para char
      char *mensagem2 = dtostrf(temp, 1, 2, mensagem);

      // Imprime a mensagem e envia a mensagem como texto pelo modulo
      Serial.print(F("Mensagem: "));
      Serial.println(mensagem2);
      resposta = lorawan.sendT(1, mensagem2); //Envio como texto

      intervalo = millis() + TEMPO_ESPERA; //Atualiza a contagem de tempo
      
      digitalWrite(13, HIGH);
      delay(500);
    }
  } else {    // Se o modulo nao estiver conectado
    // Caso o módulo tenha perdido a conexão, reinicia o dispositivo
    if (reconnect){
      Serial.println(F("Reiniciando o dispositivo..."));
      digitalWrite(13, LOW);
      ESP.restart();
    }

    // Caso o módulo esteja desconectado por mais do que 5 minutos, reinicia o dispositivo
    if(tempo_desconectado < millis()){
      Serial.println(F("Reiniciando o dispositivo..."));
      ESP.restart();
    }

    // Imprime um "." a cada 5 segundos
    if(intervalo < millis()){
      Serial.print("Tempo restante: ");
      Serial.print((tempo_desconectado - millis()) / 1000);
      Serial.print(" segundos     \r");
      intervalo = millis() + 5000; // Atualiza a contagem de tempo
    }

    digitalWrite(13, LOW);
    delay(500);
  }

  digitalWrite(13, HIGH);
  delay(500);
}

void event_handler(Event type){ 
  // Verifica se o modulo esta conectado e atualiza essa informacao
  if(type == Event::JOINED){
    Serial.println(F("Conectado!"));
    reconnect = true;
  }
}
