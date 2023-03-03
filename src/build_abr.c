#define _GNU_SOURCE
#include "ABR.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Arbre interactive_build_tree();

int main(int argc, char* argv[]) {

    Arbre tree;
    BTREE_INIT(tree);

    if (argc == 1) {
        tree = interactive_build_tree();
    } else if (argc == 2) {
        if (!cree_arbre(argv[1], &tree)) {
            fprintf(stderr, "%s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        dessine(argv[1], tree);
    }

    libere(&tree);

    return EXIT_SUCCESS;
}

Arbre interactive_build_tree() {
    Arbre tree;
    Noeud* node;
    BTREE_INIT(tree);
    char* mot;
    int n_read = 0;
    char op;
    char* user_line = NULL;
    size_t user_len = 0;
    size_t buffer_size = 0;

    while (1) {
        dessine("test", tree);
        mot = malloc(256 * sizeof(char));

        printf(">>> ");
        user_len = getline(&user_line, &buffer_size, stdin);
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
                if (!ajout(&tree, mot)) {
                    free(mot);
                }
                break;
            case 'd':
                node = supprime(&tree, mot);
                libere_noeud(node);
                break;
            default:
                fprintf(stderr, "Commande inconnue\n");
            }
        } else {
            fprintf(stderr, "Erreur dans la commande\n");
            continue;
        }
        if (op != 'a')
            free(mot);
    }

    free(mot);
    free(user_line);

    return tree;
}
