#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include "ArbreBinaireRecherche.h"
#include "Graph.h"

#define OUTDIR "output/"
#define COLOR_GREEN "\x1b[32;49;1;4m"
#define COLOR_YELLOW "\x1b[33;49;1;4m"
#define COLOR_RESET "\x1b[0m"

typedef struct Parameters {
    bool texte_inter_filtre;
    bool texte_moins_filtre;
    bool filtre;
    bool texte;
    bool create_pdfs;

    char* filename_texte;
    char* filename_filtre;
} Parameters;

int filtrer(Arbre* A, const Arbre filtre, Arbre *utilises);
Parameters parse_args(int argc, char* argv[]);
void print_help(char* name);
int create_trees(Parameters params);

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

    int nb_pos_args = argc - optind;

    if (nb_pos_args != 2) {
        fprintf(stderr, "Veuillez ajouter les fichiers positionnels\n");
        exit(EXIT_FAILURE);
    }

    params.filename_texte = argv[optind];
    params.filename_filtre = argv[optind + 1];

    return params;
}

void print_help(char* name) {
    printf(
        "Utilisation : %s [-OPTIONS] [fichier_texte] [fichier_filtre]\n"
        "options :\n"
        "   -v : Verbeux, crée une représentation en .dot et .pdf des arbres construits\n"
        "   -h : Ce message d'aide\n", name
    );
}


int filtrer(Arbre* A, const Arbre filtre, Arbre *utilises) {
    Noeud* found;
    Element e;

    if (IS_EMPTY_TREE(*A) || IS_EMPTY_TREE(filtre)) {
        return 0;
    }

    filtrer(A, filtre->fg, utilises);

    if ((found = ABR_supprime(A, filtre->valeur))) {
        e = found->valeur;
        free(found);
        // Si un mot était déjà dans utilises, on libere
        // celui récupéré 
        if (!ABR_ajout(utilises, e)) {
            free(e);
        }
    }

    filtrer(A, filtre->fd, utilises);

    return 1;   
}


int create_trees(Parameters params) {
    Arbre texte, filtre, commun;

    BTREE_INIT(texte);
    BTREE_INIT(filtre);
    BTREE_INIT(commun);

    if (!ABR_cree_arbre(params.filename_texte, &texte) ||
        !ABR_cree_arbre(params.filename_filtre, &filtre)) {
        return 0;
    }

    if (params.create_pdfs) {
        Graph_cree_graph(OUTDIR"texte.dot", OUTDIR"texte.pdf", texte);
        Graph_cree_graph(OUTDIR"filtre.dot", OUTDIR"filtre.pdf", filtre);
    }

    filtrer(&texte, filtre, &commun);

    if (params.texte_moins_filtre) {
        printf(COLOR_GREEN"Mots présents uniquement dans le texte de référence :\n"COLOR_RESET);
        ArbreB_parcours_infix(texte, stdout, "%s\n");
    }
    if (params.texte_inter_filtre) {
        printf(COLOR_YELLOW"Mots présents dans les deux textes :\n"COLOR_RESET);
        ArbreB_parcours_infix(commun, stdout, "%s\n");
    }

    if (params.create_pdfs) {
        Graph_cree_graph(OUTDIR"filtrage.dot", OUTDIR"filtrage.pdf", texte);
        Graph_cree_graph(OUTDIR"en_commun.dot", OUTDIR"en_commun.pdf", commun);
    }

    ArbreB_free(&texte);
    ArbreB_free(&filtre);
    ArbreB_free(&commun);

    return 1;
}


int main(int argc, char* argv[]) {
    Parameters params = parse_args(argc, argv);

    if (!create_trees(params)) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
