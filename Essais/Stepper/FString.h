/*
Objet FString : 
Gestion des chaînes de caractères spécialisée dans l'extraction
des champs.
 */
 
class FString {
  public:
  
  char *s;
  
  FString() {
    s = NULL;
  }

  // Constructor à partir d'une chaîne char*
  FString( char *model ) {
    s = NULL;
    Copy(model);
  }

  // Constructor avec allocation de mémoire
  FString( int size ) {
    s = NULL;
    Alloc(size);
  }

  // Destructor : libération de la mémoire
  ~FString() {
    if (s != NULL) free(s);    
  }

  // Copier n caractères d'une chaîne char*
  char *Copy( char *model, int size=-1 ) {
    if (size==-1) size = strlen(model);
    if (s != NULL) free(s);
    s = (char *)malloc(sizeof(char)*(size+1));
    strcpy(s,model);
    return s;
  }

  // Renvoie le pointeur des données en char*
  char *GetData() {
    return s;
  }

  // Longueur de la chaîne
  int Length() {
    return strlen(s);
  }

  // Opérateur de copie d'une chaîne char*
  FString *operator =( char *s2 ) {
    Copy(s2);
    return this;
  }

  // Opérateur de copie d'un autre FString
  FString *operator =( FString &s2 ) {
    return (*this) = s2.GetData();
  }

  // Concaténation
  FString *operator +=( FString &s2 ) {
    int l=Length();
    char *new_s = (char *)malloc(sizeof(char)*(l+s2.Length()));
    strcpy(new_s,s);
    strcpy(new_s+l,s2.GetData());
    free(s);
    s = new_s;
    return this;
  }

  // Renvoie un pointeur d'une chaîne char* à la position n
  char *operator ()( int n ) {
    return s+n; 
  }

  // Renvoie le caractère à la position n
  char operator []( int n ) {
    return *(s+n);
  }

  // Vérifier si une chaine de caractères commence par la chaine 'prefix'
  boolean operator ==( char *s2 ) {
    char *str = s;
    for (; *s2 != 0; s2++, str++) {
      if (*str != *s2) return false;
      if (*str == 0) return false;
    }
    if (*str != *s2) return false;
    return true;
  }

  // Vérifier si une chaine de caractères commence par la chaine 'prefix'
  boolean BeginWith( char *prefix ) {
    char *str = s;
    for (; *prefix != 0; prefix++, str++) {
      if (*str != *prefix) return false;
      if (*str == 0) return false;
    }
    return true;
  }

  // Récupérer une valeur entière
  long GetInt( int first=0 ) {
    char *str = s+first;
    long n;
    for (n=0; (*str != 0); str++) {
      if (!IsNum(*str)) break;
      n = n*10 + (*str - '0');
    }
    return n;
  }
  
  // Compter le nombre de champs séparés par un caractère
  int FieldCount( char separator ) {
    char *str = s;
    int n;
  
    for (n=1; *str != 0; str++) {
      if (*str == separator) n++;
    }
  
    return n;
  }
  
  // Déterminer la position de la nième récurrence d'un caractère
  int Find( char c, int n=1 ) {
    char *str = s;
    int i, oc;
    for (i=0,oc=0; *str!=0; str++,i++) {
      if (*str == c) {
        oc++;
        if (oc == n) return i;
      }
    }
    return -1;
  }

  FString *SubString( int first, int n ) {
    FString *ss = new FString(n);
    strncpy(ss->GetData(),s+first,n);
    *(ss->GetData()+n) = 0;
    return ss;
  }

  // Extraire un champ séparés par un caractère
  FString *GetField( char separator, int index ) {
    int a, b;
    a = (index == 0)? 0 : Find(separator,index)+1;
    b = Find(separator,index+1);
    if (b == -1) b = Length();
    return SubString(a,b-a);
  }

  // Convertir les lettres en minuscule
  void ToLower() {
    char *str = s;
    int i;
    for (; *str!=0; *str++) {
      for (i=0; MAJUS[i]!=0; i++) {
        if (*str == MAJUS[i]) {
          *str = MINUS[i];
          break;
        }
      }
    }
  }

  // Convertir les lettres en majuscule
  void ToUpper() {
    char *str = s;
    int i;
    for (; *str!=0; *str++) {
      for (i=0; MINUS[i]!=0; i++) {
        if (*str == MINUS[i]) {
          *str = MAJUS[i];
          break;
        }
      }
    }
  }

  private:

  const char *MAJUS="ABCDEFGHIJKLMNOPQRSTUVWXYZAEEEEOIIUUU";
  const char *MINUS="abcdefghijklmnopqrstuvwxyzàéèêëöïîùûü";

  // Allouer de la mémoire pour la chaîne
  void Alloc( int n ) {
    if (s != NULL) free(s);
    s = (char *)malloc(sizeof(char)*(n+1));
  }
  
  // Vérifier si un caractère est numérique
  boolean IsNum( char c ) {
    return (('0' <= c) && (c <= '9'));
  }
};

