#define _GNU_SOURCE
#include "ArbreBinaire.h"
#include "ArbreBinaireRecherche.h"
#include "Graph.h"
#include "errno.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

Arbre interactive_build_tree(FILE* f);

int main(int argc, char* argv[]) {

    Arbre tree;

    if (argc == 1) {
        tree = interactive_build_tree(stdin);
    } else if (argc == 2) {
        if (!ABR_cree_arbre("filtre.txt", &tree)) {
            fprintf(stderr, "%s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        ArbreB_list(tree, stdout, ArbreB_parcours_infix);
        // Graph_cree_graph("test.dot", "test.pdf", tree);
    }

    ArbreB_free(&tree);

    return EXIT_SUCCESS;
}

Arbre interactive_build_tree(FILE* f) {
    Arbre tree;
    Noeud* node;
    BTREE_INIT(tree);
    char* mot;
    int n_read = 0;
    char op;
    char* user_line = NULL;
    size_t user_len;
    size_t buffer_size;

    while (1) {
        Graph_cree_graph("test.dot", "test.pdf", tree);
        mot = malloc(256 * sizeof(char));

        printf(">>> ");
        user_len = getline(&user_line, &buffer_size, f);
        n_read = sscanf(user_line, "%c %s", &op, mot);

        if (user_len == EOF) {
            break;
        } else if (n_read == 1) {
            switch (op) {
            case 'p':
                ArbreB_parcours_infix(tree, stdout, "%s\n");
                break;
            }
        } else if (n_read == 2) {
            switch (op) {
            case 'a':
                ABR_ajout(&tree, mot);
                break;
            case 'd':
                node = ABR_supprime(&tree, mot);
                free(node->valeur);
                free(node);
                break;
            default:
                fprintf(stderr, "Commande inconnue\n");
            }
        } else {
            fprintf(stderr, "Erreur dans la commande\n");
            continue;
        }
    }

    free(mot);
    free(user_line);

    return tree;
}
