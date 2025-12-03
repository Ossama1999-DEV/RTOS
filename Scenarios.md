# Scénario RTOS – Chariot Élévateur

## 1️⃣ Initialisation du système

Au démarrage :

- `initialisation_des_ports()` configure toutes les E/S :
  - activation des canaux analogiques nécessaires (joystick, températures, touchpad)
  - configuration des LEDs en sortie
  - configuration des ports numériques
- `initialisation_du_systeme()` initialise le RTOS, les timers et les interruptions.
- `Init(SEM_CAN)` initialise les sémaphores utilisés entre tâches.
- Activation du **Timer0** (`TMR0ON = 1`)
- Activation des interruptions globales (`ei()`)

👉 **Le système temps réel et les tâches sont opérationnels.**

---

## 2️⃣ Configuration matérielle

`initialisation_des_ports()` réalise :

- Désactivation des fonctions analogiques inutiles et du bus externe.
- Activation uniquement des entrées nécessaires :
  - Joystick (axes X/Y)
  - Température eau / huile
  - Touchpad (si utilisé)
- LEDs RGB configurées en sortie pour indiquer l’état du chariot.
- Pull-up internes activés sur certains ports pour stabiliser les entrées.

👉 **Tous les capteurs sont prêts, les voyants aussi.**

---

## 3️⃣ Acquisition des capteurs analogiques

La fonction :

```
unsigned char lecture_8bit_analogique(unsigned char channel)
```

effectue une conversion ADC sur 8 bits pour :

- Joystick (X, Y)
- Température moteur (eau)
- Température huile
- Autres capteurs analogiques du système

👉 **Le RTOS lit en continu l’état du joystick et les températures.**

---

## 4️⃣ Scénario principal du chariot (tâche d’affichage T1)

Cette tâche met à jour l’afficheur LCD en continu.

### 🟩 a) Affichage du statut du chariot

Informations affichées :
- Mode **Marche** : AV / AR / N
- **Badge** détecté ou message “AUCUN”
- **Siège** occupé / non occupé
- **Frein à main** activé ou non
- Représentation du **chariot**
- Températures **Eau** et **Huile**
- **Choc** détecté ou non
- **Vitesse** en km/h
- Coordonnées **Joystick X / Y**
- Niveau de **Batterie** (0 % – 100 %)

👉 L’afficheur reflète en temps réel l’état complet du chariot.

---

### 🟨 b) Gestion de la vitesse

- Les boutons `VITESSE_PLUS` et `VITESSE_MOINS` permettent d’augmenter ou diminuer la vitesse.
- Si **frein ON** + **Marche = N** → vitesse forcée à `0 km/h`.

👉 Le système empêche le chariot de bouger lors d’un arrêt sécurisé.

---

### 🟥 c) Alerte de vitesse

Si `vitesse > 30` :

- Affichage du message : **ALERTE : vitesse > 30 km/h**
- LEDs configurées en mode alerte

Sinon : message effacé.

👉 Protection simple contre les excès de vitesse.

---

### 🟦 d) Gestion de la batterie

- Batterie **min = 0 %**
- Batterie **max = 100 %**

👉 La batterie reste toujours dans des valeurs réalistes.

---

## 5️⃣ Sécurité – Tâche T4 : détection du choc

La tâche 4 surveille le capteur de choc :

- Détection d’un **front descendant** (appui sur le bouton CHOC)
- Envoi sur UART d’un rapport structuré :

```
========================================
=          CHOC DETECTE                =
========================================
Vitesse : XX km/h
Badge   : AUCUN ou XX XX XX ...
========================================
```

👉 Chaque choc déclenche un log propre, sans doublon, lisible sur le moniteur série.

---

## 6️⃣ Boucle continue du RTOS

Chaque tâche s’exécute périodiquement selon l’ordonnancement du RTOS :

- Lecture capteurs
- Mise à jour de l’affichage
- Gestion vitesse / frein
- Surveillance des événements (choc, badge…)
- Communication série protégée par sémaphore

👉 Le chariot fonctionne comme un vrai système embarqué temps réel.
