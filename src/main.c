#include <stdio.h>
#include <stdlib.h>
#include "ArbreBinaireRecherche.h"
#include "Graph.h"

int main(int argc, char* argv[]) {
    Arbre dico;
    BTREE_INIT(dico);

    char* s1 = malloc(256 * sizeof(char));
    strcpy(s1, "Salut");
    char* s2 = malloc(256 * sizeof(char));
    strcpy(s2, "faut");
    char* s3 = malloc(256 * sizeof(char));
    strcpy(s3, "croire");
    char* s4 = malloc(256 * sizeof(char));
    strcpy(s4, "que");
    char* s5 = malloc(256 * sizeof(char));
    strcpy(s5, "cela");
    char* s6 = malloc(256 * sizeof(char));
    strcpy(s6, "marche");


    ABR_ajout(&dico, s1);
    ABR_ajout(&dico, s2);
    ABR_ajout(&dico, s3);
    ABR_ajout(&dico, s4);
    ABR_ajout(&dico, s5);
    ABR_ajout(&dico, s6);

    Noeud* node = ABR_supprime(&dico, "faut");
    free(node->valeur);
    free(node);

    Graph_cree_graph("salut.dot", "salut.pdf", dico);

    ArbreB_free(&dico);
    return EXIT_SUCCESS;
}
