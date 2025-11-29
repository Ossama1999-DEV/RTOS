# 🚀 RTOS – Systèmes Temps Réels  
## 🏗️ Projet : Simulation d’un Chariot Élévateur

Ce dépôt contient le travail réalisé dans le cadre du module **Systèmes Temps Réels (STR)** du CESI.  
Le projet consiste à analyser et exploiter un **mini-RTOS propriétaire (CESI RTOS)** et à développer les fonctionnalités d’un **simulateur de chariot élévateur** sur microcontrôleur PIC18F87K22.


# 📦 Livrables

## 📝 Livrable Écrit

### **1. État de l’art sur les Systèmes Temps Réels (STR)**
- Définition des STR  
- Classifications : Hard / Firm / Soft  
- Contraintes temporelles  
- Algorithmes d’ordonnancement (RMS, EDF, LLF…)  
- Contraintes matérielles & logicielles

### **2. Analyse du CESI RTOS**
- Type de STR  
- Fonctionnement de l’ordonnanceur Round Robin préemptif  
- Gestion du Timer0  
- Sauvegarde/restauration du contexte  
- Organisation mémoire (stack, registres, zones RAM)  
- Drivers :  
  - GLCD  
  - UART  
  - ADC  
  - RFID STID  

### **3. Décomposition et Fonctionnement des Tâches**
- Description des tâches T1 à T6  
- Rôles et données manipulées  
- Interactions entre tâches  
- **Organigrammes & logigrammes** dédiés

### **4. Analyse Temporelle**
- Quantum ≈ 10 ms  
- Tour complet ≈ 60 ms (6 tâches)  
- WCET et gigue  
- Validation de la réactivité (UI, choc, badge…)

### **5. Choix Techniques & Réalisation**
- Durées de polling  
- Priorités implicites dans le round-robin  
- Synchronisations (sémaphores)  
- Protection des zones critiques  
- Optimisation de l’affichage et de l’UART  

### **6. Gestion de Projet (GitHub Workflow)**
- Branches :  
  - `main` : version stable et testée  
  - `dev` : intégration avant validation  
  - `oussama`, `danilo`, `stan` : branches personnelles  
  - `feature/...` : branches spécifiques aux nouvelles fonctions
- Processus :  
  - Pull requests  
  - Relectures croisées  
  - Tests & validations  
  - Résolution de conflits  
  - Merge vers `main` uniquement après validation collective

## 🎤 Livrable Oral

### **1. Présentation + Démonstration (20 minutes)**
- Architecture du RTOS  
- Fonctionnement des tâches  
- Démo GLCD / Touch Panel / LEDs / RFID / CHOC  
- Scénarios types (choc, badge, vitesse…)

### **2. Questions du Jury (20 minutes)**
- Choix techniques  
- Gestion du multitâche  
- Justification du découpage des tâches  
- Analyse temporelle et contraintes  
- Qualité et organisation du code  

## 📁 Structure du Dépôt
    ├── README.md
    ├── Scenarios.md
    ├── ToDo.md
    └── TP_STR.X
        ├── afficheur.c
        ├── afficheur.h
        ├── build
        │   └── default
        │       ├── debug
        │       └── production
        ├── dist
        │   └── default
        │       ├── debug
        │       └── production
        ├── main.c
        ├── main.h
        ├── Makefile
        ├── nbproject
        ├── rxtx.c
        ├── rxtx.h
        ├── semaphore.c
        ├── semaphore.h
        ├── stid.c
        ├── stid.h
        ├── systeme.c
        ├── systeme.h
        ├── T1.c
        ├── T1.h
        ├── T1-modif.c
        ├── T2.c
        ├── T2.h
        ├── T3.c
        ├── T3.h
        ├── T4.c
        ├── T4.h
        ├── T5.c
        ├── T5.h
        ├── T6.c
        ├── T6.h
        └── variables_globales.h

## 👥 Auteurs
- **DBIBIH Oussama**
