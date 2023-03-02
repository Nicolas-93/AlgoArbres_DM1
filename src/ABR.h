#ifndef ABR_INCLUDED
#define ABR_INCLUDED

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef char* Element;

typedef struct noeud {
    Element valeur;
    struct noeud* fg;
    struct noeud* fd;
} Noeud, *Arbre;

#define LESS_THAN(a, b)         (strcmp((a), (b)) <  0)
#define GREATHER_THAN(a, b)     (strcmp((a), (b)) >  0)
#define GREATHER_OR_EQUAL(a, b) (strcmp((a), (b)) >= 0)
#define LESS_OR_EQUAL(a, b)     (strcmp((a), (b)) <= 0)
#define EQUALS(a, b)            (strcmp((a), (b)) == 0)
#define SEPARATORS " \n,;:.?!\\\"()-'’"
#define DOT_CREATE_CMD "dot -Tpdf %s -o %s.pdf &"

#define BTREE_INIT(tree) ((tree) = NULL)
#define IS_INTERN_NODE(node) (node->fg || node->fd)
#define IS_LEAF_NODE(node) (!(node->fg || node->fd))
#define IS_EMPTY_TREE(tree) (!(tree))
#define IS_COMPLETE_NODE(node) (node->fg && node->fd)
#define IS_BINARY_NODE(node) (IS_COMPLETE_NODE(node) || IS_LEAF_NODE(node))
#define LEFT_NODE(node) ((node)->fg)
#define RIGHT_NODE(node) ((node)->fd)
#define MAX_ELEMENT(a, b) (strcmp((a), (b)) > 0 ? (a) : (b))
#define MIN_ELEMENT(a, b) (strcmp((a), (b)) < 0 ? (a) : (b))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Fonctions générales sur les arbres */

// Allocations
Arbre alloue_noeud(Element val);
void libere_noeud(Noeud* a);
void libere(Arbre *a);

// Statistique
int ArbreB_height(Arbre a);
int ArbreB_nb_nodes(Arbre a);
int ArbreB_nb_leafs(Arbre a);
int ArbreB_nb_intern_nodes(Arbre a);
int ArbreB_nb_complete_nodes(Arbre a);
int ArbreB_est_strictement_binaire(Arbre a);
void print_stats(Arbre a);

// Fonctions de parcours
void ArbreB_parcours_prefix(Arbre a, FILE* f, const char* format);
void ArbreB_parcours_infix(Arbre a, FILE* f, const char* format);
void ArbreB_parcours_postfix(Arbre a, FILE* f, const char* format);
void ArbreB_list(Arbre a, FILE* out, void (*traversal)(Arbre, FILE*, const char* format));

/* Fonctions spécifiques aux ABRs */

// Statistique
Element min(Arbre a);
Element max(Arbre a);

Arbre extract_min(Arbre* a);
Arbre extract_max(Arbre* a);

Noeud* recherche(Arbre a, Element n);
bool appartient(Arbre a, Element n);
Noeud* ajout(Arbre* a, Element n);
Arbre supprime(Arbre* a, Element n);

int cree_arbre(const char* nom, Arbre* a);

// Création de graphes
void dessine(char* filename, Arbre a);

#endif
