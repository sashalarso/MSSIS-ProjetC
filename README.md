Installation automatique (avec Dockerfile) :
- CLoner le dépôt : `git clone https://github.com/sashalarso/MSSIS-ProjetC.git`
- Se placer dedans `cd MSSIS-ProjetC`
- Lancer Docker
- Construire l'image `docker build -t nom_image .`
- Lancer le conteneur : ` docker run -it nom_image`

Installation manuelle (sans Dockerfile) :

Pré-requis à la compilation du programme :
- git
- gcc
- make
- libssl-dev
- 
Ces paquets sont à installer avec : apt-get install nom_du_paquet

Pour compiler le programme taper : make



Pour lancer le programme :

	- en mode génération : cat liste_de_mots | ./main -G  table de correspondance 
	- en mode lookup : cat condensats | ./main -L table de correspondance
	
Une liste de 10000 mots est fournie dans mots.txt
