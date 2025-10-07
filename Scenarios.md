# Scénario RTOS : Chariot Élévateur

## 1️⃣ Initialisation du système

Appels de fonctions :
- `initialisation_des_ports()` : configure tous les ports E/S (analogiques et numériques)
- `initialisation_du_systeme()` : configure le RTOS, les timers, etc.
- `Init(SEM_CAN)` : initialise les sémaphores (communication entre tâches)

Ensuite :
- Démarrage du Timer0 (`TMR0ON=1`)
- Activation des interruptions globales (`ei()`)

> 💡 Cela met en route le système temps réel et les tâches du RTOS.

---

## 2️⃣ Configuration matérielle

Dans `initialisation_des_ports()` :
- Désactivation du bus externe et des fonctions analogiques inutiles
- Activation uniquement des entrées analogiques nécessaires :
    - Touchpad (`RA0`, `RA1`)
    - Capteurs de température (`RA2`, `RA3`)
    - Joystick (`RF5`, `RF6`)
- Configuration des LEDs (rouge, verte, bleue) en sortie
- Activation des pull-up internes sur certains ports

> 💡 Le chariot peut ainsi lire ses capteurs et indiquer son état par LED.

---

## 3️⃣ Lecture des capteurs analogiques

Fonction :
```c
unsigned char lecture_8bit_analogique(unsigned char channel)
```
Effectue une conversion analogique-numérique (ADC) sur 8 bits pour :
- Joystick (axes X/Y)
- Température moteur / huile
- Capteur de hauteur du mât

> 💡 Permet au système de connaître les mouvements du joystick, la hauteur du mât et la température du moteur.

---

## 4️⃣ Scénario principal : `scenario_chariot_elevateur()`

Le cœur du comportement automatique du chariot :

### 🟩 a) Phase de démarrage
- Message : “INITIALISATION DU CHARIOT...”
- LED verte allumée
- Petite attente (`attendre(1000)`)

> 💡 Simule le démarrage et la mise sous tension du chariot.

---

### 🟨 b) Lecture du joystick et mouvement

Lecture des axes :
```c
joystick_x = lecture_8bit_analogique(JOYSTICK_X);
joystick_y = lecture_8bit_analogique(JOYSTICK_Y);
```
Actions :
- Si le joystick est poussé vers l’avant → `avancer_chariot()`
- Si tiré vers l’arrière → `reculer_chariot()`
- Sinon → `arreter_chariot()`

> 💡 Le chariot avance, recule ou reste immobile selon la position du joystick.

---

### 🟦 c) Contrôle du mât (levage / descente)

Lecture du capteur de hauteur (`CAPTEUR_HAUTEUR`) :
- Si bas → `lever_materiel()`
- Si trop haut → `descendre_materiel()`
- Sinon → `stabiliser_hauteur()`

> 💡 Le mât s’ajuste automatiquement selon la position détectée.

---

### 🟥 d) Sécurité – température moteur

Lecture du capteur de température (`CAPTEUR_TEMP_MOTEUR`) :
- Si > 220 :
    - Allume LED rouge
    - Appelle `alerter("SURCHAUFFE MOTEUR !")`
    - Arrête le chariot

> 💡 Simule une protection thermique du moteur.

---

### 🌀 e) Boucle de surveillance continue

Boucle infinie exécutée périodiquement :
```c
surveiller_obstacles();
gerer_vitesse();
mettre_a_jour_affichage();
attendre(50);
```

> 💡 Boucle principale du RTOS : surveiller l’environnement, ajuster la vitesse, rafraîchir l’afficheur LCD, etc.

