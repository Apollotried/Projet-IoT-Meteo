#include <Wire.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <PubSubClient.h>  // Utiliser PubSubClient pour la gestion MQTT

// === Pins capteurs ===
#define LDR_PIN 32
#define DHTPIN 15
#define RAINPIN 34

// Initialisation de l'écran LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialisation du capteur DHT22
DHT dht(DHTPIN, DHT22);

// === WiFi ===
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
WiFiClient espClient;
PubSubClient client(espClient);  // Utilisation de PubSubClient pour MQTT

// Constants pour le calcul du capteur de lumière
const float GAMMA = 0.7;
const float RL10 = 50.0;

const char* mqtt_server = "broker.hivemq.com";  // Serveur MQTT

void setup() {
  // Console de débogage
  Serial.begin(115200);

  // Initialiser le capteur DHT
  dht.begin();

  // Initialiser l'écran LCD
  lcd.init();
  lcd.backlight();

  // Définir les modes des broches
  pinMode(LDR_PIN, INPUT);
  pinMode(RAINPIN, INPUT);
  pinMode(DHTPIN, INPUT);

  // Configurer la résolution de lecture analogique
  analogReadResolution(12);

  // Afficher un message de démarrage sur l'écran LCD
  lcd.setCursor(0, 0);
  lcd.print("Weather Monitor");
  lcd.setCursor(4, 1);
  lcd.print("System");
  delay(4000);
  lcd.clear();

  // Connexion au WiFi
  connectToWiFi();

  // Configurer MQTT
  setupMQTT();
}

void connectToWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connexion au WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connecté au WiFi!");
}

void setupMQTT() {
  client.setServer(mqtt_server, 1883);  // Connexion au serveur MQTT
  while (!client.connected()) {
    Serial.print("Connexion MQTT...");
    if (client.connect("ESP32_Meteo_Client")) {
      Serial.println("Connecté à MQTT");
    } else {
      Serial.print("Erreur de connexion, code: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void publishMQTT(float temp, float hum, int rain, int light) {
  // Créer un message JSON
  String payload = "{\"temperature\": " + String(temp) + ", ";
  payload += "\"humidite\": " + String(hum) + ", ";
  payload += "\"pluie\": " + String(rain) + ", ";
  payload += "\"luminosite\": " + String(light) + "}";

  // Publier les données sur un topic MQTT
  client.publish("meteo/data", payload.c_str());
  Serial.println("Données publiées via MQTT : " + payload);
}

void updateDisplay() {
  displayTemperature();
  delay(2000);
  displayHumidity();
  delay(2000);
  displayRain();
  delay(2000);
  displayLight();
  delay(2000);
}

void displayTemperature() {
  float t = dht.readTemperature();

  if (isnan(t)) {
    lcd.clear();
    lcd.print("Erreur capteur");
    return;
  }

  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.println(" C");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature :");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.print(" C");
}

void displayHumidity() {
  float h = dht.readHumidity();

  if (isnan(h)) {
    lcd.clear();
    lcd.print("Erreur capteur");
    return;
  }

  Serial.print("Humidite : ");
  Serial.print(h);
  Serial.println(" %");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidite :");
  lcd.setCursor(0, 1);
  lcd.print(h);
  lcd.print(" %");
}

void displayRain() {
  int rain = readRain();

  Serial.print("Pluie : ");
  Serial.print(rain);
  Serial.println(" %");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Niveau pluie :");
  lcd.setCursor(0, 1);
  lcd.print(rain);
  lcd.print(" %");
}

void displayLight() {
  int light = readLightPercent();

  Serial.print("Luminosite : ");
  Serial.print(light);
  Serial.println(" %");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luminosite :");
  lcd.setCursor(0, 1);
  lcd.print(light);
  lcd.print(" %");
}

int readRain() {
  int value = analogRead(RAINPIN);
  value = map(value, 0, 4095, 0, 100);
  return 100 - value;
}

float readLight() {
  int analogValue = analogRead(LDR_PIN);
  float voltage = analogValue / 4095.0 * 3.3;
  float resistance = (3.3 - voltage) * RL10 / voltage;
  return pow(RL10 * 1000 * pow(10.0, GAMMA) / resistance, (1 / GAMMA));
}

int readLightPercent() {
  int analogValue = analogRead(LDR_PIN);
  return map(analogValue, 0, 4095, 100, 0);
}

unsigned long lastPublish = 0;
void loop() {
  // Vérifie si 5 secondes se sont écoulées
  if (millis() - lastPublish > 5000) {  // Envoi des données toutes les 5 secondes
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int rain = readRain();
    int light = readLightPercent();

    // Gérer la connexion MQTT
    if (!client.connected()) {
      setupMQTT();
    }
    client.loop();  // Assure que la connexion reste active

    // Publier les données via MQTT
    publishMQTT(temperature, humidity, rain, light);

    lastPublish = millis();  // Met à jour le moment de dernier envoi
  }

  // Mise à jour de l'affichage de manière plus fluide sans blocage
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay > 2000) {  // Mise à jour de l'affichage tous les 2 secondes
    lastDisplay = millis();
    displayTemperature();
    displayHumidity();
    displayRain();
    displayLight();
  }

  delay(100);  // Un petit délai pour éviter de surcharger la CPU
}


