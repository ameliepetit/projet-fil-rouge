// Livrable Fil Rouge AAP
// Écrit par : Petit Amélie, Geffroy Océane, Schwerdorffer Dimitri, Durand Roehner Lydie
// Groupe G
//Programme 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_LEN 1000
#define MAX_FIELDS 15
#define MAX_TUPLES 60000

// 1. DEFINTION DES DIFFERENTS TYPES ET STRUCTURES

// 1.1 TYPES PRIMAIRES

typedef char t_field[MAX_LEN];       // Champ : chaîne de caractère (max 1000)
typedef t_field t_key;               // Clé : champ
typedef t_field t_value[MAX_FIELDS]; // t_value : tableau de 6000 champs maximum

typedef struct {                     //Définition des métadonnées
    char sep;                        // Séparateur (entre deux champs)
    int nbFields;                    // Nombre maximum de champs sur une ligne
    t_key key;                       // Label des champs
    t_field *fieldNames;             // FieldNames est un pointeur vers un tableau de champs
} t_metadata;

// 1.2 TYPES SECONDAIRES

typedef struct {
    t_key key;
    t_value value;
} t_tuple;                           // Tuple : couple (clé, valeur)

typedef struct {
    t_tuple tuples[MAX_TUPLES];      // Tableau de tuples
    int nbTuples;                    // Nombre de mots saisis
} t_tupletable;

typedef struct node {   			 // Définition de noeud
    t_tuple data;
    struct node *pNext;
} t_node;

typedef t_node *t_list; 			 // Une liste est un pointeur vers un noeud

typedef struct {
    int hashfunction;  				 // Numéro de la fonction de hachage
    int nbSlots; 					 // Nombre d’alvéoles
    t_list *slots; 					 // Taille définie à l'exécution
} t_hashtable;

typedef int (*function)(t_key, t_hashtable);


// 2. PROTOTYPES

// 2.1 FONCTIONS GENERALES

void split(char sep, char *txt, int nbFields, t_key key, t_field *schema_field_table);         // Documentation : 1

void print_tuples(t_metadata data, t_tupletable *dico, char *key);                             // Documentation : 2

void print_hastable(function *hashFunctionList, t_metadata data, t_hashtable hash, t_key key); // Documentation : 3


// 2.2 FONCTIONS LIEES AUX LISTES CHAINEES

int isEmpty(t_list lst);                                                                       // Documentation : 4

t_tuple getFirstVal(t_list lst); 															                                 // Documentation : 5

void showList(t_list li);																	                                     // Documentation : 6

t_list newList();																			                                         // Documentation : 7

t_list addHeadNode(t_tuple data, t_list lst);										                        		   // Documentation : 8

t_list removeHeadNode(t_list lst);															                               // Documentation : 9


// 2.3 FONCTIONS DE HACHAGE

int first_ASCII(t_key key, t_hashtable hash);											                             // Documentation : 10

int sum_ASCII(t_key, t_hashtable hash);														                             // Documentation : 11

// 3. FONCTIONS

void split(char sep, char *txt, int nbFields, t_key key, t_field *schema_field_table) {
    int l = strlen(txt);                                       // "strlen" calcule le nombre de caractères de la chaîne de caractères passée en argument.
    int k = 0, i = 0, j = 0;

    while ((txt[i] != sep) && (txt[i] != '\0')) {              // Recherche des clés
        key[i] = txt[i];
        i++;
    }

    key[i] = '\0';

    i++;                                                       //Positionne le pointeur de lecture au début du prochain champ.

    while ((i < l) && (k < nbFields)) {                        // 	Extraction de tous les champs en découpant la chaîne à chaque occurrence du séparateur, en ignorant les caractères de nouvelle ligne et de fin de chaîne.
        while ((txt[i] != sep) && (txt[i] != '\0') && (txt[i] != '\n') && (txt[i] != '\r')) {
            schema_field_table[k][j] = txt[i];
            j++;
            i++;
        }
        k++;
        i++;
        j = 0;
    }
}

void print_tuples(t_metadata data, t_tupletable *dico, char *key) {
    int index_key = 0;
    int found = 0;
    for (int i = 0; i < dico->nbTuples; i++) { // Recherche de la clé dans le dictionnaire. La fonction strcmp compare deux chaînes de caractères et retourne 0 si elles sont identiques.  
        if (strcmp(key, dico->tuples[i].key) == 0) {
            found = 1;
            break;
        }
        index_key++;
    }
    if (!found) {
        printf("Recherche de %s : échec ! nb comparaisons : %d\n", key, index_key);
    } else {
        printf("Recherche de %s : trouvé ! nb comparaisons : %d\n", key, index_key + 1);
        printf("%s : %s\n", data.key, key); // mot : key

        for (int k = 0; k < data.nbFields - 1; k++) {
            if ((dico->tuples[index_key].value[k][0] != '\0')) {
                printf("%s: %s\n", data.fieldNames[k], dico->tuples[index_key].value[k]); // anagramme k: ...
            } else if (strcmp(data.fieldNames[k], "") != 0) {
                printf("%s: X\n", data.fieldNames[k]); // anagramme k: X
            }
        }
    }
}

void print_hastable(function *hashFunctionList, t_metadata data, t_hashtable hash, t_key key) {
    int h = hashFunctionList[hash.hashfunction](key, hash);
    int nbComparisons = 0;
    t_list temp = hash.slots[h];

    while (temp && strcmp(temp->data.key, key) != 0) {
        temp = temp->pNext;
        nbComparisons++;
    }

    printf("Recherche de %s : %s ! nb comparaisons : %d\n",
           key, temp ? "trouvé" : "échec", nbComparisons + 1);

    if (temp) {
        printf("%s : %s\n", data.key, key); // mot : key
        for (int k = 0; k < data.nbFields - 1; k++) {
            printf("%s: %s\n", data.fieldNames[k],
                   temp->data.value[k][0] ? temp->data.value[k] : "X");
        }
    }
}

// 3.2 FONCTIONS DE LISTES CHAINEES

int isEmpty(t_list lst) {
    return (lst == NULL);
}

t_tuple getFirstVal(t_list lst) {
    assert(!isEmpty(lst));
    return lst->data;
}

void showList(t_list lst){
    int k = 0;
    printf("*** affichage de la liste ***\n");
    while (lst != NULL){
        printf("Maillon %d, Valeur = %.1f\n", k, lst->val);
        lst = lst->p;
        k++;
    }
}

t_list newList() {
    t_list lst = NULL;
    return lst;
}

t_list addHeadNode(t_tuple data, t_list lst) {
    t_node *n = malloc(sizeof(t_node));
    n->data = data;
    n->pNext = lst;
    return n;
}

t_list removeHeadNode(t_list lst) {
    assert(!isEmpty(lst));
    t_list p = lst->pNext;
    free(lst);
    return p;
}

// 3.3 FONCTIONS HACHAGE

int first_ASCII(t_key key, t_hashtable hash) {
    return (int) (key[0] % hash.nbSlots);
}

int sum_ASCII(t_key key, t_hashtable hash) {
    int s = 0;
    for (size_t i = 0; i < strlen(key); i++) {
        s += (int) key[i];
    }
    return (s % hash.nbSlots);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Utilisation :\n./programme <nom du fichier>\n"); // Si l'utilisateur n'utilise pas la bonne syntaxe pour lancer le programme
        exit(EXIT_FAILURE);
    }
    FILE *anagrammeFile = fopen(argv[1], "r");

    // IMPLEMENTATION METADONNEES
  
    char temp[MAX_LEN]; // Création de la chaîne de caractère temporaire qui va contenir le texte du fichier.
  
    // SEPARATEUR
  
    fgets(temp, MAX_LEN, anagrammeFile);
    while (strcspn(temp, "\n\r") > 1) { // Une ligne est lue seulement si elle contient un seul caractère (le séparateur)
        fgets(temp, MAX_LEN, anagrammeFile);
    }

    t_metadata meta;
    meta.sep = *temp; // Stockage du séparateur

    // NOMBRE DE CHAMPS ET NOMS DE CHAMPS
    if (meta.sep != '#') {
        fgets(temp, MAX_LEN, anagrammeFile);
        while (temp[0] == '#') {  // Si "#" n'est pas le séparateur, on ignore les lignes qui commencent avec #.
            fgets(temp, MAX_LEN, anagrammeFile);
        }
      
        meta.nbFields = atoi(temp); // Obtient le nombre de champs.

        t_field *field_table = malloc(sizeof(t_field) * meta.nbFields); // Liste de champs.

        fgets(temp, MAX_LEN, anagrammeFile);
        while (temp[0] == '#') {
            fgets(temp, MAX_LEN, anagrammeFile);
        }

        split(meta.sep, temp, meta.nbFields, meta.key, field_table); // Obtient les noms de champs.
        meta.fieldNames = field_table;
    } else { // Si # est le séparateur, on vérifie qu'aucune ligne n'est un commentaire après.
      
        fgets(temp, MAX_LEN, anagrammeFile);

        meta.nbFields = atoi(temp);

        fgets(temp, MAX_LEN, anagrammeFile);

        t_field *field_table = malloc(sizeof(t_field) * meta.nbFields);

        split(meta.sep, temp, meta.nbFields, meta.key, field_table);
        meta.fieldNames = field_table;
    }

    // CREATION DU DICTIONNAIRE
  
    t_tupletable *dico1 = malloc(sizeof(t_tupletable));                                                // Pointeur vers un dictionnaire.
    int len_dic = 0;

    while (fgets(temp, MAX_LEN, anagrammeFile) != NULL) {                                              // Lecture du fichier.
        split(meta.sep, temp, meta.nbFields, dico1->tuples[len_dic].key, dico1->tuples[len_dic].value);
        len_dic++;                                                                                     //Remplissage de dico1 avec des tuples.
    }
    fclose(anagrammeFile);

    dico1->nbTuples = len_dic;                                                                         // Nombre de clés ajoutées à dico1

    char finding[MAX_LEN];
    printf("Mots indexés : %d\n", len_dic);
    printf("Donnez le mot à trouver : \n");
    // printf("%s", finding);
    while (scanf("%99s", finding) != EOF) {
        print_tuples(meta, dico1, finding);
        printf("\nDonnez le mot à trouver : \n");
    }

}
