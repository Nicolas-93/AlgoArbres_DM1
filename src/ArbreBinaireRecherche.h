#ifndef __ABR_H__
#define __ABR_H__

#include <stdbool.h>
#include <string.h>
#include "ArbreBinaire.h"

#define LESS_THAN(a, b)         (!!(strcmp((a), (b)) < 0))
#define GREATHER_THAN(a, b)     (!!(strcmp((a), (b)) > 0))
#define GREATHER_OR_EQUAL(a, b) (!!(strcmp((a), (b)) >= 0))
#define LESS_OR_EQUAL(a, b)     (!!(strcmp((a), (b)) <= 0))
#define EQUALS(a, b)            (strcmp((a), (b)) == 0)

// Fonctions statistiques
Element ABR_min(Arbre a);
Element ABR_max(Arbre a);

Arbre ABR_extract_min(Arbre* a);
Arbre ABR_extract_max(Arbre* a);

Noeud* ABR_recherche(Arbre a, Element n);
bool ABR_appartient(Arbre a, Element n);
Noeud* ABR_ajout(Arbre* a, Element n);
Arbre ABR_supprime(Arbre* a, Element n);
bool ABR_est_recherche_sans_doublon(Arbre a);

int ABR_cree_arbre(const char* nom, Arbre* a);

#endif
