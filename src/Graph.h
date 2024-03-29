#ifndef GRAPH_INCLUDED
#define GRAPH_INCLUDED

#include "ArbreBinaire.h"

void Graph_ecrire_debut(FILE* f);
void Graph_ecrire_arbre(FILE* f, Arbre a);
void Graph_ecrire_fin(FILE* f);
void Graph_dessine(FILE* f, Arbre a);
void Graph_cree_graph(char* dot_filename, char* pdf_filename, Arbre a);

#endif
