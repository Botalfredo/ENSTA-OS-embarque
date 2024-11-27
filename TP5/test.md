Voici un compte rendu détaillé pour ton TP en suivant les instructions et les étapes que nous avons déjà discutées :

---

# Compte Rendu TP5 : Construction d’un Système Linux Embarqué pour la BeagleBone Black

## 1. Introduction

Ce TP avait pour objectif de construire un système Linux embarqué personnalisé pour une carte BeagleBone Black en utilisant **Buildroot**. Le processus comprenait l'installation de Buildroot, la création d'un environnement de compilation croisée, l’installation d'un système de fichiers sur une carte SD, et la configuration de la communication entre l’hôte et la BeagleBone via TFTP.

## 2. Installation de Buildroot et Compilation

### Étapes d'installation
1. **Installation des paquets requis** : Les commandes suivantes ont été utilisées pour installer les paquets nécessaires :

   ```bash
   sudo apt-get install sed make binutils gcc g++ bash patch gzip bzip2 perl tar cpio python unzip rsync wget libncurses-dev libssl-dev
   ```

   - **sed** : Manipulation et transformation de texte.
   - **make** : Automatisation de la compilation.
   - **binutils** : Outils pour la manipulation des fichiers binaires.
   - **gcc/g++** : Compilateurs pour le C et le C++.
   - **bash** : Shell pour l'exécution de scripts.
   - **patch** : Applique des modifications aux fichiers.
   - **gzip/bzip2** : Outils de compression.
   - **perl** : Langage de programmation pour divers scripts.
   - **tar/cpio** : Manipulation d'archives.
   - **python** : Langage de programmation requis par plusieurs scripts de Buildroot.
   - **unzip** : Extraction d’archives zip.
   - **rsync** : Outil de synchronisation de fichiers.
   - **wget** : Téléchargement de fichiers depuis le web.
   - **libncurses-dev** : Bibliothèque pour interfaces en ligne de commande.
   - **libssl-dev** : Bibliothèque pour la cryptographie (OpenSSL).

2. **Téléchargement et extraction de Buildroot** : Nous avons téléchargé et extrait Buildroot depuis le site officiel.

3. **Configuration avec `menuconfig`** : La commande suivante a été exécutée pour accéder au menu de configuration de Buildroot :

   ```bash
   make menuconfig
   ```

   - **Target Options** : Configuration pour le processeur ARM Cortex-A8.
   - **Toolchain** : Sélection d’une chaîne de compilation spécifique.
   - **System Configuration** : Configuration de l’hôte et du mot de passe root.
   - **Kernel** : Sélection de la version du noyau Linux (5.3) et des en-têtes.

4. **Compilation du système** : Une fois la configuration terminée, nous avons lancé la compilation avec la commande :

   ```bash
   make
   ```

   Cette étape génère l’environnement Linux embarqué pour la BeagleBone Black.

## 3. Installation de l’Environnement Embarqué sur la Carte SD

### Création des Partitions et Système de Fichiers

1. **Préparation de la carte SD** :
   - Identification de la carte SD (`/dev/sdc` sur notre système).
   - Suppression des données initiales :

     ```bash
     sudo dd if=/dev/zero of=/dev/sdc bs=1M count=16
     ```

   - Création de deux partitions avec `cfdisk` :
     - **Partition 1** : 16 Mo, type W95 FAT16, marquée comme amorçable.
     - **Partition 2** : le reste de l’espace en ext4.

2. **Formatage des partitions** :
   - Partition 1 (FAT16) :

     ```bash
     sudo mkfs.vfat -F 16 -n BOOT /dev/sdc1
     ```

   - Partition 2 (ext4) :

     ```bash
     sudo mkfs.ext4 -L ROOTFS /dev/sdc2
     ```

### Installation du Système sur la Carte SD

1. **Copie des fichiers de démarrage** dans la partition BOOT (`/media/csn/BOOT`) :
   - MLO : Bootloader primaire.
   - u-boot.img : Bootloader secondaire.
   - zImage : Image du noyau Linux.
   - uEnv.txt : Configuration de démarrage pour u-boot.
   - am335x-boneblack.dtb : Fichier de description du matériel pour la BeagleBone.

2. **Extraction du système de fichiers** dans ROOTFS :

   ```bash
   sudo tar -C /media/enseirb/ROOTFS/ -xf /home/fredlinux/Documents/OS-embarque/TP5/output/images/rootfs.tar
   ```

## 4. Configuration de la Communication avec la Carte via TFTP

### Installation du Serveur TFTP

1. **Installation des paquets** :

   ```bash
   sudo apt-get install tftp tftpd xinetd
   ```

2. **Configuration du répertoire TFTP** :

   ```bash
   sudo mkdir /tftpboot
   sudo chmod 777 /tftpboot
   ```

3. **Fichier de configuration TFTP** : Création du fichier `/etc/xinetd.d/tftp` avec le contenu suivant :

   ```plaintext
   service tftp
   {
       socket_type = dgram
       protocol = udp
       wait = yes
       user = root
       server = /usr/sbin/in.tftpd
       server_args = -s /tftpboot
   }
   ```

4. **Redémarrage du service xinetd** :

   ```bash
   sudo killall -HUP xinetd
   sudo service xinetd start
   ```

### Configuration Réseau et Communication TFTP

1. **Configuration de l’adresse IP de l’hôte (PC)** :
   
   ```bash
   sudo ip addr add 192.168.1.2/24 dev enx000ec6647e31
   sudo ip route add default via 192.168.1.1
   ```

2. **Configuration de l’adresse IP de la BeagleBone** via le fichier `/etc/network/interfaces` :

   ```plaintext
   auto eth0
   iface eth0 inet static
   address 192.168.1.50
   netmask 255.255.255.0
   gateway 192.168.1.1
   ```

3. **Test de la connexion** avec `ping` :

   ```bash
   ping 192.168.1.50  # Depuis l'hôte vers la BeagleBone
   ping 192.168.1.2   # Depuis la BeagleBone vers l'hôte
   ```

4. **Transfert de fichiers via TFTP** :

   - Création d’un fichier de test sur l’hôte :

     ```bash
     echo "Ceci est un fichier de test" > /tftpboot/testfile.txt
     ```

   - Téléchargement du fichier sur la BeagleBone :

     ```bash
     tftp -g -r testfile.txt 192.168.1.2
     ```

## 5. Résumé et Conclusion

Ce TP nous a permis de construire un environnement Linux embarqué personnalisé pour la BeagleBone Black et de configurer un serveur TFTP pour la communication entre la BeagleBone et l’hôte. Nous avons appris à configurer Buildroot, à préparer et formater une carte SD, et à utiliser le protocole TFTP pour le transfert de fichiers, ce qui est essentiel dans le domaine des systèmes embarqués.

--- 

Tu peux copier ce rapport dans un document Word et y ajouter des détails ou captures d'écran spécifiques si nécessaire.
