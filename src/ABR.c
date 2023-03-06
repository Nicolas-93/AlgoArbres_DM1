#include "ABR.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

void Graph_ecrire_debut(FILE* f);
void Graph_ecrire_arbre(FILE* f, Arbre a);
void Graph_ecrire_fin(FILE* f);

Arbre alloue_noeud(Element val) {
    Noeud* n;
    if ((n = malloc(sizeof(Noeud)))) {
        n->fg = NULL;
        n->fd = NULL;
        n->valeur = val;
    }
    return n;
}

void libere(Arbre* a) {
    if (IS_EMPTY_TREE(*a))
        return;

    libere(&((*a)->fg));
    libere(&((*a)->fd));
    free((*a)->valeur);
    free(*a);

    *a = NULL;
}

void libere_noeud(Noeud* a) {
    if (IS_EMPTY_TREE(a))
        return;

    free(a->valeur);
    free(a);
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

void print_stats(Arbre a) {
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
        min(a),
        max(a));
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


Arbre extract_min(Arbre* a) {
    Noeud* tmp;
    if (!(*a)->fg) {
        tmp = *a;
        *a = (*a)->fd;
        return tmp;
    }
    return extract_min(&(*a)->fg);
}

Arbre extract_max(Arbre* a) {
    Noeud* tmp;
    if (!(*a)->fd) {
        tmp = *a;
        *a = (*a)->fg;
        return tmp;
    }
    return extract_max(&(*a)->fd);
}

Element min(Arbre a) {
    if (!a->fg)
        return a->valeur;
    return min(a->fg);
}

Element max(Arbre a) {
    if (!a->fd)
        return a->valeur;
    return max(a->fd);
}

Noeud* ajout(Arbre* a, Element n) {
    if (IS_EMPTY_TREE(*a)) {
        *a = alloue_noeud(n);
        return *a;
    }
    if (EQUALS((*a)->valeur, n))
        return NULL;
    if (LESS_THAN(n, (*a)->valeur))
        return ajout(&LEFT_NODE(*a), n);
    else
        return ajout(&RIGHT_NODE(*a), n);
}

Arbre supprime(Arbre* a, Element n) {
    Arbre tree = *a;
    if (IS_EMPTY_TREE(tree))
        return NULL;
    if (LESS_THAN(n, tree->valeur))
        return supprime(&((*a)->fg), n);
    if (GREATHER_THAN(n, tree->valeur))
        return supprime(&((*a)->fd), n);
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
    Arbre a_max = extract_max(&((*a)->fg));
    *a = a_max;
    (*a)->fg = tree->fg;
    (*a)->fd = tree->fd;

    return tree;
}

bool appartient(Arbre a, Element n) {
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

int cree_arbre(const char* nom, Arbre* a) {
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
            if (!ajout(a, mot)) {
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
        libere(a);
        return 0;
    }
    return 1;
}

void Graph_ecrire_debut(FILE* f) {
    fprintf(
        f,
        "digraph arbre {\n"
        "\tnode [ shape = record , height = .1 ];\n"
        "\tedge [ tailclip = false , arrowtail = dot , dir = both ];\n");
}

void Graph_ecrire_arbre(FILE* f, Arbre a) {
    if (IS_EMPTY_TREE(a))
        return;

    fprintf(
        f,
        "\tn%p [label=\"<gauche> | <valeur> %s | <droit>\"];\n",
        (void*) a,
        a->valeur);

    if (a->fg) {
        fprintf(f, "\tn%p:gauche:c -> n%p:valeur;\n", (void*) a, (void*) a->fg);
    }
    if (a->fd) {
        fprintf(f, "\tn%p:droit:c -> n%p:valeur;\n", (void*) a, (void*) a->fd);
    }
    Graph_ecrire_arbre(f, a->fg);
    Graph_ecrire_arbre(f, a->fd);
}

void Graph_ecrire_fin(FILE* f) {
    fprintf(f, "}\n");
}

void Graph_dessine(FILE* f, Arbre a) {
    Graph_ecrire_debut(f);
    Graph_ecrire_arbre(f, a);
    Graph_ecrire_fin(f);
    fclose(f);
}

void dessine(char* filename, Arbre a) {
    int filename_len = strlen(filename);
    int filename_ext_len = filename_len + 4 + 1;
    char filename_dot[filename_ext_len];
    char cmd[sizeof(DOT_CREATE_CMD) + filename_ext_len * 2];

    sprintf(filename_dot, "%s.dot", filename);

    FILE* fdot = fopen(filename_dot, "w");
    Graph_dessine(fdot, a);
    sprintf(cmd, DOT_CREATE_CMD, filename_dot, filename);
    system(cmd);
}
