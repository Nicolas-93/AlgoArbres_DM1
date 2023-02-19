#include <stdio.h>
#include <stdlib.h>
#include "ArbreBinaire.h"
#include "Graph.h"

void Graph_ecrire_debut(FILE* f) {
    fprintf(f, "digraph arbre {\n"
    "\tnode [ shape = record , height = .1 ];\n"
    "\tedge [ tailclip = false , arrowtail = dot , dir = both ];\n");
}

void Graph_ecrire_arbre(FILE* f, Arbre a) {
    if (IS_EMPTY_TREE(a))
        return;
    
    fprintf(f,"\tn%p [label=\"<gauche> | <valeur> %s | <droit>\"];\n", (void *) a, a->valeur);
    
    if (a->fg) {
        fprintf(f, "\tn%p:gauche:c -> n%p:valeur;\n", (void *) a, (void *) a->fg);
    }
    if (a->fd) {
        fprintf(f, "\tn%p:droit:c -> n%p:valeur;\n", (void *) a, (void *) a->fd);
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


void Graph_cree_graph(char* dot_filename, char* pdf_filename, Arbre a) {
    char cmd[256] = {0};
    FILE* fdot = fopen(dot_filename, "w");
    Graph_dessine(fdot, a);
    sprintf(cmd, "dot -Tpdf %s -o %s &", dot_filename, pdf_filename);
    // fprintf(stderr, cmd);
    system(cmd);
}
