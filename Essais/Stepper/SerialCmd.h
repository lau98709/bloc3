/*
Gestion des commandes venant du port série

Des fonctions (Str...) de traitement de chaines de caractères
disponibles 

Notice :

#include "SerialCmd.h"

void Traitement( char *commande ) {
  // fonction lancée lorsqu'une commande
  // est reçu
}

void setup() {
  ...

  // A insérer dans setup()
  SerialCmd.begin(9600,Traitement);  
}

void loop() {
  // A insérer dans loop()
  SerialCmd.run();
}

 */


void (*TraitementCommande)( char *commande ) = NULL;

class SerialCmdClass {
  public:
    char serialbuffer[32], n_serbuf;
    
    // Gestion du port série
    // -Récupère les données en attente
    // -Si un 'retour chariot' (\r) est détecte,
    //  alors interpréter la commande
    //
    void run() {
      if (Serial.available() > 0) {
        char c = Serial.read();
        if ((c == '\r') || (c == '\n')) {
          // 'retour chariot' reçu
          // interpréter la commande
          serialbuffer[n_serbuf] = 0;
          _Traitement(serialbuffer);
          n_serbuf = 0;
        } else {
          // Réception du caractère et
          // stockage dans le buffer
          serialbuffer[n_serbuf] = c;
          n_serbuf++;
        }
      }
    }
    
    // Interprétation des commandes
    void _Traitement( char* commande ) {
      if (TraitementCommande != NULL) {
        TraitementCommande(commande);
      }
    }

    void begin( int speed=9600, void (*traitement)(char *commande)=NULL ) {
      Serial.begin(speed);
      TraitementCommande = traitement;
      n_serbuf = 0;
    }
} SerialCmd;


//-----------------------------------------------------
// Fonctions utilitaires de traitement des chaines
//-----------------------------------------------------

// Vérifier si une chaine de caractères commence par la chaine 'prefix'
boolean StrBeginWith( char *str, char *prefix )
{
  for (; *prefix != 0; prefix++, str++) {
    if (*str != *prefix) return false;
    if (*str == 0) return false;
  }
  return true;
}


// Vérifier si un caractère est numérique
boolean IsNum( char c ) {
  return (('0' <= c) && (c <= '9'));
}


// Récupérer une valeur entière
long StrGetInt( char *str ) {
  long n;
  for (n=0; (*str != 0); str++) {
    if (!IsNum(*str)) break;
    n = n*10 + (*str - '0');
  }
  return n;
}


// La longueur d'une chaine
char StrLength( char *str )
{
  int n;
  for (n=0; *str!=0; str++,n++);
  return n;
}


// Extraire une sous-chaine
char *StrSubString( char *str, int start, int len )
{
  char *s, c; int i;
  s = (char *)malloc(sizeof(char)*(len+1));
  for (i=0; i<len; i++) {
    c = *(str+start+i);
    if (c == 0) break;
    *(s+i) = c;
  }
  *(s+i) = 0;
  return s;
}


// Compter le nombre de champs séparés par un caractère
int StrFieldCount( char *str, char separator )
{
  int n;

  for (n=1; *str != 0; str++) {
    if (*str == separator) n++;
  }

  return n;
}


// Déterminer la position de la nième récurrence d'un caractère
int StrCharIndex( char *str, char c, int n )
{
  int i, oc;
  for (i=0,oc=0; *str!=0; str++,i++) {
    if (*str == c) {
      oc++;
      if (oc == n) return i;
    }
  }
  return -1;
}


// Extraire un champ séparés par un caractère
char *StrGetField( char *str, char separator, int index )
{
  int a, b;
  a = (index == 0)? 0 : StrCharIndex(str,separator,index)+1;
  b = StrCharIndex(str,separator,index+1);
  if (b == -1) b = StrLength(str);
  return StrSubString(str,a,b-a);
}

