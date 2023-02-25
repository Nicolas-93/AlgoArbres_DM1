#ifndef AB_INCLUDED
#define AB_INCLUDED

#include <stdio.h>
#include <string.h>

typedef char* Element;

typedef struct noeud {
    Element valeur;
    struct noeud* fg;
    struct noeud* fd;
} Noeud, *Arbre;

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

// Fonctions d'allocations
Arbre ArbreB_alloue_noeud(Element val);
void ArbreB_free(Arbre *a);

// Fonctions statistiques
int ArbreB_height(Arbre a);
int ArbreB_nb_nodes(Arbre a);
int ArbreB_nb_leafs(Arbre a);
int ArbreB_nb_intern_nodes(Arbre a);
int ArbreB_nb_complete_nodes(Arbre a);
int ArbreB_est_strictement_binaire(Arbre a);
Element ArbreB_min(Arbre a);
Element ArbreB_max(Arbre a);
void ArbreB_print_stats(Arbre a);

// Fonctions de parcours
void ArbreB_parcours_prefix(Arbre a, FILE* f, const char* format);
void ArbreB_parcours_infix(Arbre a, FILE* f, const char* format);
void ArbreB_parcours_postfix(Arbre a, FILE* f, const char* format);
void ArbreB_list(Arbre a, FILE* out, void (*traversal)(Arbre, FILE*, const char* format));

#endif
