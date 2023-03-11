#include "ABR.h"
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTDIR "./"
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

int filtre(Arbre* a, const Arbre filtreur, Arbre* utilises);
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
    opterr = false;

    while ((opt = getopt(argc, argv, "hv")) != EOF) {
        switch (opt) {

        case 'v':
            params.filtre = true;
            params.texte = true;
            params.create_pdfs = true;
            break;

        case '?':
            fprintf(stderr, "L'option -%c n'existe pas.\n", optopt);
            exit(EXIT_FAILURE);
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
        "Utilisation : %s -vh [fichier_texte] [fichier_filtre]\n"
        "options :\n"
        "   -v : Verbeux, crée une représentation en .dot et .pdf des arbres "
        "construits\n"
        "   -h : Ce message d'aide\n",
        name);
}

int filtre(Arbre* a, const Arbre filtreur, Arbre* utilises) {
    Noeud* found;

    if (IS_EMPTY_TREE(*a) || IS_EMPTY_TREE(filtreur)) {
        return 0;
    }

    if ((found = suppression(a, filtreur->valeur))) {
        ajout(utilises, found->valeur);
        libere_noeud(found);
    }

    filtre(a, filtreur->fg, utilises);
    filtre(a, filtreur->fd, utilises);

    return 1;
}

int create_trees(Parameters params) {
    Arbre texte, filtreur, commun;

    BTREE_INIT(texte);
    BTREE_INIT(filtreur);
    BTREE_INIT(commun);

    if (!cree_arbre(params.filename_texte, &texte) ||
        !cree_arbre(params.filename_filtre, &filtreur)) {
        libere(&texte);
        libere(&filtreur);
        return 0;
    }

    if (params.create_pdfs) {
        dessine(OUTDIR "texte", texte);
        dessine(OUTDIR "filtre", filtreur);
    }

    filtre(&texte, filtreur, &commun);

    if (params.texte_moins_filtre) {
        printf(COLOR_GREEN "Mots présents uniquement dans le texte de "
                           "référence :\n" COLOR_RESET);
        parcours_infixe(texte, stdout, "%s\n");
    }
    if (params.texte_inter_filtre) {
        printf(COLOR_YELLOW
               "Mots présents dans les deux textes :\n" COLOR_RESET);
        parcours_infixe(commun, stdout, "%s\n");
    }

    if (params.create_pdfs) {
        dessine(OUTDIR "filtrage", texte);
        dessine(OUTDIR "en_commun", commun);
    }

    libere(&texte);
    libere(&filtreur);
    libere(&commun);

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
