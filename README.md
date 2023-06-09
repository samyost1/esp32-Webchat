# esp32-Webchat
Sam Yost (1030197)

Een webchat die draait op een ESP32 microcontroller. De ESP32, gecombineer met een DHT22-temperatuur- en vochtigheidssensor, is in staat om de temperatuur te meten en deze gegevens beschikbaar te maken via een chatinterface. 

De ESP32 is verbonden met een centrale broker (netwerkenbasis.com) via WiFi. Deze broker maakt communicatie mogelijk tussen de ESP32 en een webserver. Via een webpagina kunnen we commando's naar de bot sturen en de gemeten temperatuurwaarden opvragen.


## Installatie
#### Eigen interface/ webchat
1. Run de chat app met een terminal in de uitgepakte project/webchat folder en het commando: ```docker compose up -d```.
2. Dan kan er in de webbrowser een tab met https://localhost geopend worden. 
3. In de webbrowser komt nu een pop-up tevoorschijn. Hier moet ingelogd worden. 
<br />Gebruikersnaam: ```samyost``` Wachtwoord: ```1030197```.
4. Hier is dan de chat app te zien met je user-id bovenin.
5. Via dit tabblad kan er met de centrale MQTT-broker gecommuniceerd worden (https://netwerkenbasis.com). Er kan ook met een ander localhost tabblad gecommuniceerd worden.
#### Hardware
Benodigheden:
  - ESP32
  - DHT22 sensor

De ESP32 en DHT22 sensor worden met elkaar verbonden door jumperwires. De DHT22 sensor stuurt zijn data naar de GPIO4 pin op de ESP32. De ESP32 kan met een micro-USB kabel met de laptop verbonden worden.


## Commando's
- Uitlezen temperatuur van DHT22 sensor:
```BOT-<studentnummer>: temperatuur```
- Uitlezen luchtvochtigheid van DHT22 sensor:
```BOT-<studentnummer>: vochtigheid```
- ESP32 LED aanzetten:
```BOT-<studentnummer>: led:aan```
- ESP32 LED uitzetten:
```BOT-<studentnummer>: led:uit```
