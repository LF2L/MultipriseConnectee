# Multiprise connectée

## Description
La multiprise connectée permet de contrôler individuellement chacune des prises à distance mais également de programmer le déclenchement selon des plages horaires. 
![exterieur](/images/DSC09150.JPG)
![interieur](/images/DSC09142.JPG)
  
## Fonction
* Alimenter chacune des prises en fonction de la demande de l’utilisateur 
* Déclencher les prises en fonction des plages horaires programmées 
* Couper l’alimentation électrique lors de demande délestage 

## Composants
* Node MCU ESP8266
* RTC 
* Relai 5v 6 canaux
* Convertisseur 220V AC- 5V DC
* Câbles 230V
* Multiprise

## Schéma d'assemblage électronique
![schema](/images/multiprise_bb.png)

## Boitiers
* Acrylique 3mm

## Echanges d'informations
La multiprise va se connecter à un serveur qui centralise les informations concernant les objets connectés. Il possède entre autre les informations des puces RFID avec les autorisations et les statistiques de consommation (durée et puissance).  A l’instar des formulaires de connexion sur le site web, les données d’identification doivent certainement faire l’objet d’un « hashage ».

### Informations récupérées
Pour fonctionner la prise doit uniquement recevoir une autorisation d’alimenter la prise. 

### Informations envoyées
Inversement la seule variable de fonctionnement de la prise connectée est l’UID de la puce RFID.

### Fonctionnement
Lors de la détection de la présence d’une puce RFID, la prise connectée envoie l’information de la carte (préférablement sécurisée) au serveur REST qui centralise les informations dont celles des identifiants UID des puces, des autorisations et des statistiques de consommation. En fonction, des règles de fonctionnement appliquées par le serveur, il renvoie une autorisation d’activer ou non la prise électrique. 
La prise étant connectée au serveur, il est possible de désactiver la prise lors d’une demande de délestage. 

## To do
* Réaliser les fonctions REST du serveur pour la commande de l’objet.
* Script d’envoie du statut de l’objet et de réception des commande.
* Fonction de programmation temporelle et déclenchement des prises.
