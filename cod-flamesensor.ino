#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "iPhone de Igor";  // Rede Wi-Fi
const char* password = "colorado";     // Senha Wi-Fi
const char* serverURL = "http://172.20.10.11:5000"; // IP do backend

const int pinSensor = A0;
const int pinLed = 2;

bool luzApagada = false;
int threshold = 512;  // Sensibilidade padrão do sensor

WiFiClient client;  // Criando o objeto WiFiClient

void setup() {
  Serial.begin(9600);
  pinMode(pinSensor, INPUT);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, HIGH);

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");
}

void enviarDadosParaBackend(int valorSensor) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String endpoint = String(serverURL) + "/logging/post_logs";
    
    http.begin(client, endpoint);  // Passando WiFiClient para o método begin
    http.addHeader("Content-Type", "application/json");

    // Construir JSON
    String payload = "{\"valor_sensor\": " + String(valorSensor) + "}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Resposta do servidor: ");
      Serial.println(response);
      Serial.print("Payload enviado: ");
      Serial.println(payload);

    } else {
      Serial.println("Erro na requisição HTTP");
    }
    http.end();
  } else {
    Serial.println("Erro: Wi-Fi não conectado.");
  }
}

void receberConfiguracao() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String endpoint = String(serverURL) + "/controle/get_config";
    
    http.begin(client, endpoint);  // Passando WiFiClient para o método begin
    
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      String response = http.getString();
      threshold = response.toInt(); // Exemplo de resposta simples
      Serial.print("Novo threshold recebido: ");
      Serial.println(threshold);
    }
    http.end();
  }
}

void loop() {
  // Lê valor do sensor
  int valorSensor = analogRead(pinSensor);
  Serial.print("Valor do sensor: ");
  Serial.println(valorSensor);

  // Envia dados ao backend
  enviarDadosParaBackend(valorSensor);

  // Recebe nova configuração
  receberConfiguracao();

  // Controle do LED com base no threshold
  if (valorSensor < threshold) {
    digitalWrite(pinLed, LOW);
    Serial.println("Fogo detectado! LED desligado.");
  } else {
    digitalWrite(pinLed, HIGH);
  }

  delay(2000);  // Delay de 2 segundos entre leituras
}
