Pré-requis à la compilation du programme :
- git
- gcc
- make
- libssl-dev
- 
Ces paquets sont à installer avec : apt-get install <paquet>

Pour compiler le programme taper : make

Pour lancer le programme :

	- en mode génération : ./main G <dictionnaire> <table de correspondance> 
	- en mode lookup : ./main L <table de correspondance> puis entrer le hash recherché
	
Une liste de 10000 mots est fournie dans mots.txt
