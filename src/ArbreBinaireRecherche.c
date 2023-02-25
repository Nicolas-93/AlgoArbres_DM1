#define _GNU_SOURCE

#include "ArbreBinaireRecherche.h"
#include "ArbreBinaire.h"
#include <stdbool.h>
#include <stdlib.h>

#define SEPARATORS " \n,;:.?!\\\"()-'’"

Noeud* ABR_recherche(Arbre a, Element n) {
    while (a && a->valeur != n) {
        if (LESS_OR_EQUAL(n, a->valeur)) {
            a = a->fg;
        } else
            a = a->fd;
    }
    return a;
}

Noeud* ABR_ajout(Arbre* a, Element n) {
    if (IS_EMPTY_TREE(*a)) {
        *a = ArbreB_alloue_noeud(n);
        return *a;
    }
    if (EQUALS((*a)->valeur, n))
        return NULL;
    if (LESS_THAN(n, (*a)->valeur))
        return ABR_ajout(&LEFT_NODE(*a), n);
    else
        return ABR_ajout(&RIGHT_NODE(*a), n);
}

Arbre ABR_supprime(Arbre* a, Element n) {
    Arbre tree = *a;
    if (IS_EMPTY_TREE(tree))
        return NULL;
    if (LESS_THAN(n, tree->valeur))
        return ABR_supprime(&((*a)->fg), n);
    if (GREATHER_THAN(n, tree->valeur))
        return ABR_supprime(&((*a)->fd), n);
    // A partir d'ici tree->a == n :

    // Cas simple où les deux sous arbres sont nuls
    if (IS_LEAF_NODE(tree)) {
        *a = NULL;
        return tree;
    }

    // Cas simples où un des deux des sous arbre est nul.
    if (IS_EMPTY_TREE(tree->fg)) {
        *a = (*a)->fd;
        return tree;
    }
    if (IS_EMPTY_TREE(tree->fd)) {
        *a = (*a)->fg;
        return tree;
    }

    // Cas où le noeud possède deux sous-arbres non nuls
    Arbre a_min = ABR_extract_min(&((*a)->fd));
    *a = a_min;
    (*a)->fg = tree->fg;
    (*a)->fd = tree->fd;

    return tree;
}

Arbre ABR_extract_min(Arbre* a) {
    Noeud* tmp;
    if (!(*a)->fg) {
        tmp = *a;
        *a = (*a)->fd;
        return tmp;
    }
    return ABR_extract_min(&(*a)->fg);
}

Arbre ABR_extract_max(Arbre* a) {
    Noeud* tmp;
    if (!(*a)->fd) {
        tmp = *a;
        *a = (*a)->fg;
        return tmp;
    }
    return ABR_extract_max(&(*a)->fd);
}

Element ABR_min(Arbre a) {
    if (!a->fg)
        return a->valeur;
    return ABR_min(a->fg);
}

Element ABR_max(Arbre a) {
    if (!a->fd)
        return a->valeur;
    return ABR_max(a->fd);
}

bool ABR_appartient(Arbre a, Element n) {
    while (a) {
        if (EQUALS(a->valeur, n))
            return true;
        if (LESS_THAN(n, a->valeur))
            a = LEFT_NODE(a);
        else
            a = RIGHT_NODE(a);
    }
    return false;
}

/**
 * @brief Crée l'arbre avec un fichier contenant des mots séparés par des
 * séparateurs.
 *
 *
 * @param nom Le nom du fichier à lire.
 * @param a L'arbre à remplir.
 * @return 0 en cas d'erreur d'ouverture du fichier, -1 en cas d'erreur
 * d'allocation, 1 sinon
 */
int ABR_cree_arbre(const char* nom, Arbre* a) {
    FILE* f = fopen(nom, "r");
    if (!f)
        return 0;

    bool memerr = false;
    char* mot;
    char* line = NULL;
    size_t len_buf = 0;
    ssize_t len_line;

    while ((len_line = getline(&line, &len_buf, f)) != EOF) {
        for (char* token = strtok(line, SEPARATORS); token;
             token = strtok(NULL, SEPARATORS)) {
            mot = strdup(token);
            if (!mot) {
                memerr = true;
                break;
            }
            if (!ABR_ajout(a, mot)) {
                free(mot);
            }
        }
        if (memerr)
            break;
    }

    // Nettoyage
    fclose(f);
    free(line);

    if (memerr) {
        ArbreB_free(a);
        return 0;
    }
    return 1;
}
