# BPM4STREAM v1.0
![AlienTest](https://github.com/iveinsomnia/BPM4STREAM/blob/main/pic1.jpg?raw=true)
![AlienTest](https://github.com/iveinsomnia/BPM4STREAM/blob/main/screenAlien.jpg?raw=true)
EN:  
Simple heartbeat for streamer with ESP8266 & MAX30102 (and MAX30105)  

I made this small and easy project for streamer who wants to do horror games on halloween streams ^^  
The setup require soldering for the MAX3010x board and simple connections on the ESP8266  
IMPORTANT: don't forget to solder your wanted voltage on the back of the board (3.3v here)  

Then  
VIN - SDA - SCL - GRD <- MAX30105  
3v3 -  D2 -  D1 - GRD <- ESP8266 (D1 Mini here, please check your manual for SDA & SCL)  

You can setup the colors, fonts and graph color here:  
line 76: font color  
line 77: font size  
line 78: Font  
line 92: line color  
Also don't forget to setup the wifi line 23  

Enjoy ^^  
And show me the result !  

====================================================================  

FR:  
Simple capteur cardiaque pour streamer avec ESP8266 & MAX30102 (ou MAX30105)  

J'ai fais ce petit projet facile pour les streamer qui veulent du jeu d'horreur en stream pour halloween ^^  
Ce DIY nécessite de faire de la soudure  sur la carte MAX3010x et une connectique simple sur le ESP8266  
IMPORTANT: N'oubliez pas de souder le voltage voulu sur l'arrière de la carte (3.3v dans mon cas)  

Ensuite:  
VIN - SDA - SCL - GRD <- MAX30105  
3v3 -  D2 -  D1 - GRD <- ESP8266 (ici sur un D1 Mini, Vérifier le manuel pour trouver le SDA & SCL)  

Les couleurs, police, tailles et couleur du graphique peuvent être modifié ici:  
ligne 76: font color  
ligne 77: font size  
ligne 78: Font  
ligne 92: line color  
N'oubliez pas non plus de setup le wifi en ligne 23  

Enjoy ^^  
Et montrez moi vos résultats !  
