#include "ArbreBinaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Arbre ArbreB_alloue_noeud(Element val) {
    Noeud* n;
    if ((n = malloc(sizeof(Noeud)))) {
        n->fg = NULL;
        n->fd = NULL;
        n->valeur = val;
    }
    return n;
}

void ArbreB_free(Arbre* a) {
    if (IS_EMPTY_TREE(*a))
        return;

    ArbreB_free(&((*a)->fg));
    ArbreB_free(&((*a)->fd));
    free((*a)->valeur);
    free(*a);

    *a = NULL;
}

int ArbreB_height(Arbre a) {
    int h_a, h_b;

    if (IS_EMPTY_TREE(a))
        return -1;

    h_a = ArbreB_height(a->fg);
    h_b = ArbreB_height(a->fd);

    return 1 + MAX(h_a, h_b);
}

int ArbreB_nb_nodes(Arbre a) {
    if (IS_EMPTY_TREE(a))
        return 0;

    return 1 + ArbreB_nb_nodes(a->fg) + ArbreB_nb_nodes(a->fd);
}

int ArbreB_nb_leafs(Arbre a) {
    if (IS_EMPTY_TREE(a))
        return 0;

    return IS_LEAF_NODE(a) + ArbreB_nb_leafs(a->fg) + ArbreB_nb_leafs(a->fd);
}

int ArbreB_nb_intern_nodes(Arbre a) {
    if (IS_EMPTY_TREE(a))
        return 0;

    return IS_INTERN_NODE(a) + ArbreB_nb_intern_nodes(a->fg) +
           ArbreB_nb_intern_nodes(a->fd);
}

int ArbreB_nb_complete_nodes(Arbre a) {
    if (IS_EMPTY_TREE(a))
        return 0;

    return IS_COMPLETE_NODE(a) + ArbreB_nb_complete_nodes(a->fg) +
           ArbreB_nb_complete_nodes(a->fd);
}

int ArbreB_est_strictement_binaire(Arbre a) {
    if (IS_EMPTY_TREE(a))
        return 1;
    if (!IS_BINARY_NODE(a))
        return 0;
    return ArbreB_est_strictement_binaire(a->fd) &&
           ArbreB_est_strictement_binaire(a->fd);
}

Element ArbreB_min(Arbre a) {
    Element min_fg, min_fd;

    if (IS_LEAF_NODE(a))
        return a->valeur;

    min_fg = a->fg ? ArbreB_min(a->fg) : a->valeur;
    min_fd = a->fd ? ArbreB_min(a->fd) : a->valeur;

    return MIN_ELEMENT(min_fg, min_fd);
}

Element ArbreB_max(Arbre a) {
    Element max_fg, max_fd;

    if (IS_LEAF_NODE(a))
        return a->valeur;

    max_fg = a->fg ? ArbreB_max(a->fg) : a->valeur;
    max_fd = a->fd ? ArbreB_max(a->fd) : a->valeur;

    return MAX_ELEMENT(max_fg, max_fd);
}

void ArbreB_print_stats(Arbre a) {
    printf(
        "Est strictement binaire : %s\n"
        "Hauteur: %d\n"
        "Nombre de noeuds: %d\n"
        "Nombre de feuilles: %d\n"
        "Nombre de noeuds internes: %d\n"
        "Nombre de noeuds internes à deux fils: %d\n"
        "Minimum : %s\n"
        "Maximum : %s\n",
        ArbreB_est_strictement_binaire(a) ? "Oui" : "Non",
        ArbreB_height(a),
        ArbreB_nb_nodes(a),
        ArbreB_nb_leafs(a),
        ArbreB_nb_intern_nodes(a),
        ArbreB_nb_complete_nodes(a),
        ArbreB_min(a),
        ArbreB_max(a));
}

void ArbreB_parcours_prefix(Arbre a, FILE* f, const char* format) {
    if (IS_EMPTY_TREE(a)) {
        return;
    }

    fprintf(f, format, a->valeur);
    ArbreB_parcours_prefix(a->fg, f, format);
    ArbreB_parcours_prefix(a->fd, f, format);
}

void ArbreB_parcours_infix(Arbre a, FILE* f, const char* format) {
    if (IS_EMPTY_TREE(a))
        return;

    ArbreB_parcours_infix(a->fg, f, format);
    fprintf(f, format, a->valeur);
    ArbreB_parcours_infix(a->fd, f, format);
}

void ArbreB_parcours_postfix(Arbre a, FILE* f, const char* format) {
    if (IS_EMPTY_TREE(a))
        return;

    ArbreB_parcours_postfix(a->fg, f, format);
    ArbreB_parcours_postfix(a->fd, f, format);
    fprintf(f, format, a->valeur);
}

void ArbreB_list(
    Arbre a, FILE* out, void (*traversal)(Arbre, FILE*, const char* format)) {
    putchar('[');
    traversal(a, out, "%s, ");
    printf("\b\b]\n");
}
