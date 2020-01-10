# E-Glove
## Entwurfsspezifikation
### Autoren
Luis Deutsch
Phillip Penner

### Inhaltsverzeichnis

# 1 Einführung

## 1.1 Beschreibung
Unter dem Projektnamen "E-Glove" wird ein Eingabegerät für (Windows) Rechner entwickelt, welches unter anderem die intuitive Steuerung von Präsentationen ermöglichen soll. Bei dem Gerät handelt es sich um einen Handschuh, also ein "wearable"-Gerät, welches sich per Bluetooth mit einem Zielrechner verbindet und im Akkubetrieb funktioniert. Der E-Glove wird an der rechten Hand getragen und erkennt bestimmte Gesten um daraufhin Befehle an einen Zielrechner zu senden. Sensoren an den Fingerspitzen bieten eine simple Form von Menüführung, um zwischen verschiedenen Funktionalitäten des Handschuhs zu wechseln, so ist beispielsweise die Kontrolle von Präsentationen, sowie Multimedia und von Browsern möglich. Für alle Funktionalitäten werden aber nur wenige unterschiedliche Gesten benötigt, die intuitiv mit der jeweiligen Funktion assoziierbar sind, wodurch der Umgang mit dem E-Glove einfach erlernbar ist. Beispielsweise führt eine Wischbewegung nach Links dazu, dass in einer Präsentation eine Folie weiter geschaltet wird, ein Verhalten, welches bereits ähnlich bei Smartphones genutzt wird und welches seinen Ursprung wohl bei dem Umblättern von Seiten eines Buches hat.


## 1.2 Ziele
    - Anwendungsbereiche, Motivation, Umfang, Marktanforderungen, Alleinstellungsmerkmale
    - Informationen zu Zielbenutzergruppen und deren Merkmale (Bildung, Erfahrung, Sachkenntnis)
    - Abgrenzung (Was ist das Softwaresystem _nicht_)

# 2 Anforderungen

## 2.1 Funktionale Anforderungen
### 2.1.1 Use Case Diagramm
![UseCaseDiagramm_komplett](images/UseCaseDiagramm_komplett.png)

### 2.1.2 Anforderungsdiagramm
![Funktionale Anforderungen](images/FR.png)

## 2.2 Nicht-funktionale Anforderungen

### 2.2.1 Anforderungsdiagramm
![Nicht-funktionale Anforderungen](images/NFR.png)

### 2.2.2 Rahmenbedingungen

#### Fremde Vorgaben
1. Als Kernelement soll der **M5Stack Core** (https://m5stack.com/) genutzt werden.
2. Als Framework für die Software-Entwicklung auf dem M5Stack, soll das **ESP-IDF** (https://github.com/espressif/esp-idf) genutzt werden.
3. Es sollen die Funktionalitäten von **FreeRTOS** (https://www.freertos.org/) genutzt werden, um Tasks zu erstellen und zu verwalten.
4. Als Programmiersprache (auf dem M5Stack) soll **C / C++** genutzt werden.

### 2.2.3 Qualitätsmerkmale
    - Externe Qualitätsanforderungen (z.B. Performance, Sicherheit, Zuverlässigkeit, Benutzerfreundlichkeit)


# 3 Technische Beschreibung

## 3.1 Systemübersicht

### 3.1.1 Strukturdiagramm

![Strukturdiagramm](images/Strukturdiagramm.png)

### 3.1.2 Blockdefinitionsdiagramm: Physischer Aufbau

![Blockdefinitionsdiagramm_physisch](images/Blockdefinitionsdiagramm_physisch.png)

### 3.1.3 Kommunikationsprotokoll
Der E-Glove und der Zielrechner kommunizieren über Bluetooth, wobei der E-Glove als Server fungiert und der Zielrechner als Client.
Für den kompletten Ablauf der Kopplung zwischen Server und Client siehe 3.4, vereinfacht kann aber gesagt werden:
1. Der Server (E-Glove) bietet einen Service mit einer Charakteristik an.
2. Der Client (Zielrechner) sucht nach dem Service und abonniert dann die Charakteristik, um bei jeder Änderung des Wertes der Charakteristik benachrichtigt zu werden.
3. Der Server (E-Glove) schreibt Daten in die Charakteristik.
4. Der Client (Zielrechner) liest die Daten aus der Charakteristik.
5. Wiederhole ab 3.

**Werte, die in die Charakteristik geschrieben werden und welche virtuelle Taste sie auslösen:**

(Siehe "Microsoft Virtual-Key Codes" als Referenz: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)

| **Wert** | **Virtuelle Taste** | **Hexadezimalwert der virtuellen Taste** |
| :------ | :----- | :----- |
| 1 | RIGHT ARROW key | 0x27 |
| 2 | LEFT ARROW key | 0x25 |
| 3 | F5 key | 0x74 |
| 4 | ESC key | 0x1B |
| 5 | Next Track key | 0xB0 |
| 6 | Previous Track key | 0xB1 |
| 7 | Play/Pause Media key | 0xB3 |
| 8 | Volume Up key | 0xAF |
| 9 | Volume Down key | 0xAE |
| 10 | Browser Forward key | 0xA7 |
| 11 | Browser Back key | 0xA6 |
| 12 | UP ARROW key | 0x26 |
| 13 | DOWN ARROW key | 0x28 |


## 3.2 Verwendete Hardware

### M5Stack Core (+ Starter Kit)

![m5stack](images/photos/m5stack.jpg ':size=200')<br>
https://m5stack.com/collections/m5-core/products/m5go-iot-starter-kit-stem-education

- Betrieben durch einen ESP32.
- Bietet viele bereits eingebaute Features an (z.B. LCD-Farbdisplay, 3 Tasten, Lautsprecher, ...).
- Für uns hauptsächlich relevant:
	- Bluetooth-Funktionalität (BLE).
		- Für die kabellose Kommunikation mit dem Zielrechner, der gesteuert werden soll.
	- 9-Achsen-Sensor
		- Das enthaltene Gyroskop, sowie der Beschleunigungssensor werden genutzt, um die Gesten zu erkennen.

### Flexsensor

![flex_sensor](images/photos/flex_sensor.jpg ':size=200')<br>
https://www.antratek.de/flex-sensor-2-2?gclid=EAIaIQobChMIppH7n8m35QIVmMx3Ch376AAcEAQYAyABEgJ9efD_BwE

- An dem Ringfinger-Rücken besfestigt.
- Hat eine Länge von ca. 5,5 cm.
- Hat bei geradem Zustand einen geringen Widerstand und bei zunehmender Biegung erhöht sich auch der Widerstand.
- Wenn der Sensor (und damit der Ringfinger) gebogen ist, heißt dies, dass der Benutzer eine Geste zur Steuerung des Zielrechners ausführen will.

### Punkt Lasermodul

![laserpointer_einzeln](images/photos/laserpointer_einzeln.jpg ':size=200')<br>
https://www.reichelt.de/punkt-lasermodul-rot-650-nm-03-6-vdc-9x20-mm-klasse-1-pico-70132441-p254487.html

- Mittels einer 3D-gedruckten Halterung am Zeigefinger befestigt
- Erweiternd zu den Präsentationsfunktionen des E-Glove als Zeigewerkzeug genutzt.

### Bluetooth USB 2.0 Adapter

![bluetooth_adapter](images/photos/bluetooth_adapter.jpg ':size=200')<br>
https://www.reichelt.de/micro-bluetooth-usb-2-0-adapter-v4-0-edr-logilink-bt0015a-p170030.html?&trstct=pol_0

- Adapter, der dem angeschlossenen Rechner Bluetooth-Funktonalitäten verleiht.
- Für die Nutzung mit Rechnern, die keine eingebaute Bluetooth-Funktonalität besitzen.

### Kapazitiver Berührungssensor x4

![debo_touch](images/photos/debo_touch.png ':size=200')<br>
https://www.reichelt.de/entwicklerboards-kapazitiver-beruehrungssensor-debo-touch-p253985.html

- Befestigt an allen Fingerspitzen, außer dem Daumen.
- Dienen zur Auswahl der verschiedenen Modi des Handschuhs ("Menüführung").
- Der Daumen ist bei dem E-Glove frei, mit diesem weden die Sensoren betätigt.

## 3.3 Erklärung anhand von Beispielbildern

### E-Glove (Oberseite)

### E-Glove (Unterseite)

### Kapazitive Sensoren in der Nahaufnahme

![alle_finger](images/photos/alle_finger.jpg ':size=600')<br>

### Wechsel in den Präsentationsmodus

![zeigefinger_aktiv](images/photos/zeigefinger_aktiv.jpg ':size=600')<br>

![presentation_modus](images/photos/presentation_modus.jpg ':size=600')<br>

### Wechsel in den Multimediamodus

![mittelfinger_aktiv](images/photos/mittelfinger_aktiv.jpg ':size=600')<br>

![multimedia_modus](images/photos/multimedia_modus.jpg ':size=600')<br>

### Wechsel in den Browsermodus

![ringfinger_aktiv](images/photos/ringfinger_aktiv.jpg ':size=600')<br>

![browser_modus](images/photos/browser_modus.jpg ':size=600')<br>

### Wechsel in den Laserpointermodus

![kleiner_finger_aktiv](images/photos/kleiner_finger_aktiv.jpg ':size=600')<br>

![laserpointer_modus](images/photos/laserpointer_modus.jpg ':size=600')<br>

### Inaktiver Zustand (keine Gesten werden gelesen)

### Aktiver Zustand (Gesten werden gelesen)



## 3.4 Abläufe

### 3.4.1 Zustandsdiagramm des gesamten Systems

![Zustandsdiagramm_komplett](images/Zustandsdiagramm_komplett.png)

### 3.4.2 Zustandsdiagramm für Präsentations-, Multimedia- und Browsermodus

![Zustandsdiagramm_RestlicheModi](images/Zustandsdiagramm_RestlicheModi.png)

### 3.4.3 Zustandsdiagramm für den Laserpointermodus

![Zustandsdiagramm_Laserpointer](images/Zustandsdiagramm_Laserpointer.png)

### 3.4.4 Sequenzdiagramm für die Auswahl des Modus

![Sequenz_auswahl](images/Sequenz_auswahl.png)

### 3.4.5 Sequenzdiagramm für das Ausführen einer Geste

![Sequenz_Geste](images/Sequenz_Geste.png)


## 3.5 Entwurf
    - Detaillierte UML-Diagramme für relevante Softwarebausteine

### 3.5.1 Klassendiagramm (vereinfacht)

![Klassendiagramm_reduziert](images/Klassendiagramm_reduziert.png)

### 3.5.2 Klassendiagramm (vollständig)

![Klassendiagramm_komplett](images/Klassendiagramm_komplett.png)

## 3.6 Entwurfsmuster / Programmier-Prinzipien

### 3.6.1 Verwendung des Singeltonpatterns

![Singelton_pattern](images/singelton_pattern.png)

Da viele Komponenten bei multiplen Zugriffinstanzen Probleme verursachen können, wurde bei einigen Klassen das Singeltonpattern genutzt.
In diesem Programm verwenden folgende Klassen, dass Singeltonpattern:

* **BleHandler**
* **FlexSensor**
* **DisplayHandler**
* **GestureRecognition**

### 3.6.2 Objektorientierte Programmierung

Um eine bessere Datenkapselung und Zugriffskontrolle zu haben, wurde hauptsächlich objektorientierte Programmierung angewandt.
[siehe Klassendiagramm](#_351-klassendiagramm-vereinfacht)

# 4 Projektorganisation

## 4.1 Annahmen
    - Nicht durch den Kunden definierte spezifische Annahmen, Anforderungen und Abhängigkeiten
    - Verwendete Technologien (Programmiersprache, Frameworks, etc.)
    - Einschränkungen, Betriebsbedingungen und Faktoren, die die Entwicklung beeinflussen (Betriebssysteme, Entwicklungsumgebung)
    - Interne Qualitätsanforderungen (z.B. Softwarequalitätsmerkmale wie z.B. Erweiterbarkeit)

## 4.2 Verantwortlichkeiten
    - Zuordnung von Personen zu Softwarebausteinen aus Kapitel 3.1 und 3.2
    - Rollendefinition und Zuordnung

## 4.3 Grober Projektplan

### Meilenstein 1 (Bis: 18.11.2019)
Handschuh bauen und in die Komponenten einarbeiten.

### Meilenstein 2 (Bis: 02.12.2019)
Kabellose Verbindung zum Zielrechner herstellen und Signale übertragen.

### Meilenstein 3 (Bis: 30.12.2019)
Registrierung von Gesten.

### Meilenstein 4 (Bis: 13.01.2020)
Zusammenführung der Funktionen.


# 5 Anhänge

## 5.1 Glossar
    - Definitionen, Abkürzungen, Begriffe

## 5.2 Referenzen
    - Handbücher, Gesetze

## 5.3 Index


