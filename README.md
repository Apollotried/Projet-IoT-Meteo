# Weather Monitoring System IoT

## Description du projet
Ce projet consiste à créer un **système de surveillance météorologique basé sur l’IoT**, capable de :  
- Collecter des données environnementales : température, humidité, luminosité et pluie.  
- Transmettre ces données en temps réel via **MQTT**.  
- Visualiser les données sur un tableau de bord **Node-RED** et les stocker dans le cloud via **ThingSpeak**.  
- Simuler l’ensemble du projet dans **Wokwi** avant un déploiement réel.

---

## Structure du projet

Weather-Monitoring-System-IoT/
│
├─ wokwi/ ← fichiers du projet Wokwi (ESP32 + capteurs)
├─ flows/ ← fichier Node-RED JSON (flows_meteo.json)
├─ screenshots/ ← captures d’écran Node-RED et ThingSpeak
└─ README.md

---

## Wokwi

- Contient la simulation ESP32 et des capteurs.  
- Fichiers principaux :  
  - `project.wokwi` et autres fichiers générés par Wokwi.  
- Pour ouvrir la simulation :  
  1. Ouvrir Wokwi ([https://wokwi.com/](https://wokwi.com/))  
  2. Importer le fichier `.wokwi`  
  3. Lancer la simulation  

---

## Node-RED

- Node-RED est utilisé pour **visualiser les données en temps réel** via un tableau de bord interactif.  
- Pour importer le flow :  
  1. Installer Node-RED sur ton PC  
  2. Aller dans Node-RED → Menu ≡ → Import → Clipboard  
  3. Coller le contenu du fichier `flows_meteo.json`  
  4. Lancer le flow  
- Accéder au Dashboard Node-RED : [http://localhost:1880/ui](http://localhost:1880/ui)  

**Capture d’écran du Dashboard Node-RED :**  
![Node-RED Dashboard](screenshots/node_red_dashboard.png)

---

## ThingSpeak

- Les données sont envoyées depuis Node-RED vers ThingSpeak pour **le stockage et la consultation à distance**.  
- ThingSpeak permet de visualiser les graphiques historiques et d’analyser les mesures.  

**Capture d’écran ThingSpeak :**  
![ThingSpeak Dashboard](screenshots/thingspeak_dashboard.png)

---

## Librairies et outils utilisés

- **ESP32** pour la collecte des données.  
- **LiquidCrystal_I2C** pour l’écran LCD.  
- **DHT** pour le capteur de température et humidité.  
- **PubSubClient** pour MQTT.  
- **WiFi** intégré pour la connexion réseau.  
- **Wokwi** pour la simulation.  
- **Node-RED** pour le dashboard et la visualisation.  
- **ThingSpeak** pour le stockage cloud.  

---

## Instructions pour reproduire le projet

1. Ouvrir le projet Wokwi et lancer la simulation.  
2. Lancer Node-RED et importer le flow JSON.  
3. Observer les données en temps réel dans Node-RED.  
4. Vérifier les graphiques et l’historique sur ThingSpeak.  

---

