#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "ArbreBinaireRecherche.h"
#include "Graph.h"

typedef struct Parameters {
    bool texte_inter_filtre;
    bool texte_moins_filtre;
    bool filtre;
    bool texte;
    bool create_pdfs;

    char* filename_texte;
    FILE* file_texte;
    char* filename_filtre;
    FILE* file_filtre;

} Parameters;

int filtrer(Arbre* A, const Arbre filtre, Arbre *utilises);
Parameters parse_args(int argc, char* argv[]);
void print_help(char* name);

int filtrer(Arbre* A, const Arbre filtre, Arbre *utilises) {
    Noeud* found;
    Element e;

    if (IS_EMPTY_TREE(*A) || IS_EMPTY_TREE(filtre)) {
        return 0;
    }

    filtrer(A, filtre->fg, utilises);
    filtrer(A, filtre->fd, utilises);

    if ((found = ABR_supprime(A, filtre->valeur))) {
        e = found->valeur;
        free(found);
        ABR_ajout(utilises, e);
    }

     
    return 1;   
}

int main(int argc, char* argv[]) {
    Parameters params = parse_args(argc, argv);
    
    Arbre texte, filtre, utilise;

    BTREE_INIT(texte);
    BTREE_INIT(filtre);

    ABR_cree_arbre(params.filename_texte, &texte);
    ABR_cree_arbre(params.filename_filtre, &filtre);

    filtrer(&texte, filtre, &utilise);

    Graph_cree_graph("test_filtrage.dot", "test_filtrage.pdf", texte);

    return EXIT_SUCCESS;

}

void print_help(char* name) {
    printf(
        "Utilisation : %s\n"
        "options :\n"
        "   -v : Crée les fichiers pdf des arbres des fichiers texte, filtre, leur intersection et texte - filtre\n"
        "   -h : Ce message d'aide\n", name
    );
}


Parameters parse_args(int argc, char* argv[]) {
    Parameters params = {
        .texte_inter_filtre = true,
        .texte_moins_filtre = true,
        .filtre = false,
        .texte = false,
        .create_pdfs = false,
    };

    int opt;

    while ((opt = getopt(argc, argv, "hv")) != EOF) {
        switch (opt) {
            
        case 'v':
            params.filtre = true;
            params.texte = true;
            params.create_pdfs = true;
            break;

        case '?':
        case 'h':
            print_help(argv[0]);
            exit(EXIT_SUCCESS);
        }
    }

    // Vérifier ici le nombre d'args positionnels
    params.filename_texte = argv[optind];
    params.filename_filtre = argv[optind + 1];

    /*
    if (!(params.file_texte = fopen(params.filename_texte, "r")) || 
        !(params.file_filtre = fopen(params.filename_filtre, "r"))
        ) {
        // strerror ici
        fprintf(stderr, "Erreur lors l'ouverture d'un des fichiers\n");
        exit(EXIT_FAILURE);
    }*/
    
    return params;
}
