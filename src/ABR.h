#ifndef ABR_INCLUDED
#define ABR_INCLUDED

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef char* Element;

typedef struct noeud {
    Element valeur;
    struct noeud* fg;
    struct noeud* fd;
} Noeud, *Arbre;

// Fonctions de comparaison nécessaires pour un ABR
#define LESS_THAN(a, b)         (strcmp((a), (b)) <  0)
#define GREATHER_THAN(a, b)     (strcmp((a), (b)) >  0)
#define GREATHER_OR_EQUAL(a, b) (strcmp((a), (b)) >= 0)
#define LESS_OR_EQUAL(a, b)     (strcmp((a), (b)) <= 0)
#define EQUALS(a, b)            (strcmp((a), (b)) == 0)

// Séparateurs de mots pour la lecture d'un fichier
#define SEPARATORS " \n,;:.?!\\\"()-'’"

// Commandes pour générer un graphe des arbres
#define DOT_CREATE_CMD "dot -Tpdf %s -o %s.pdf &"

// Macros pour les arbres

// Initialisation d'un arbre
#define BTREE_INIT(tree) ((tree) = NULL)
#define IS_INTERN_NODE(node) (node->fg || node->fd)
#define IS_LEAF_NODE(node) (!(node->fg || node->fd))
#define IS_EMPTY_TREE(tree) (!(tree))
#define IS_COMPLETE_NODE(node) (node->fg && node->fd)
#define IS_BINARY_NODE(node) (IS_COMPLETE_NODE(node) || IS_LEAF_NODE(node))
#define LEFT_NODE(node) ((node)->fg)
#define RIGHT_NODE(node) ((node)->fd)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Fonctions générales sur les arbres */

// Allocations //

/**
 * @brief Alloue un noeud pour un arbre.
 * 
 * @param val 
 * @return Arbre 
 */
Arbre alloue_noeud(Element val);

/**
 * @brief Libère un noeud de l'arbre, et sa valeur associée.
 * 
 * @param a 
 */
void libere_noeud(Noeud* a);

/**
 * @brief Libère un arbre.
 * 
 * @param a 
 */
void libere(Arbre *a);

// Statistique //

/**
 * @brief Retourne la hauteur d'un arbre.
 * 
 * @param a 
 * @return int 
 */
int ArbreB_height(Arbre a);

/**
 * @brief Retourne le nombre de noeuds d'un arbre.
 * 
 * @param a 
 * @return int 
 */
int ArbreB_nb_nodes(Arbre a);

/**
 * @brief Retourne le nombre de feuilles d'un arbre.
 * 
 * @param a 
 * @return int 
 */
int ArbreB_nb_leafs(Arbre a);

/**
 * @brief Retourne le nombre de noeuds internes d'un arbre.
 * 
 * @param a 
 * @return int 
 */
int ArbreB_nb_intern_nodes(Arbre a);

/**
 * @brief Retourne le nombre de noeuds complets d'un arbre.
 * (un noeud complet est un noeud possédant deux fils)
 * @param a 
 * @return int 
 */
int ArbreB_nb_complete_nodes(Arbre a);

/**
 * @brief Détermine si un arbre est strictement binaire.
 * (un arbre est strictement binaire si il possède deux fils, ou aucun fils)
 * 
 * @param a 
 * @return int 
 */
int ArbreB_est_strictement_binaire(Arbre a);

/**
 * @brief Affiche les statistiques sur un arbre
 * à l'aide fonctions précédentes.
 * 
 * @param a 
 */
void print_stats(Arbre a);

// Fonctions de parcours //

/**
 * @brief Parcours un arbre en prefixe.
 * 
 * @param a 
 * @param f Fichier sur lequel écrire le parcours
 * @param format Chaine de formatage devant contenir un %s
 */
void parcours_prefixe(Arbre a, FILE* f, const char* format);

/**
 * @brief Parcours un arbre en infixe.
 * 
 * @param a 
 * @param f Fichier sur lequel écrire le parcours
 * @param format Chaine de formatage devant contenir un %s
 */
void parcours_infixe(Arbre a, FILE* f, const char* format);

/**
 * @brief Parcours un arbre en postfixe.
 * 
 * @param a 
 * @param f Fichier sur lequel écrire le parcours
 * @param format Chaine de formatage devant contenir un %s
 */
void parcours_postfix(Arbre a, FILE* f, const char* format);

/**
 * @brief Affiche un arbre sous forme d'une liste, selon
 * un parcours défini par la fonction ``traversal``.
 * 
 * @param a 
 * @param out Fichier sur lequel écrire le parcours
 * @param traversal Fonction de parcours (prefixe, infixe, postfixe)
 */
void ArbreB_list(Arbre a, FILE* out, void (*traversal)(Arbre, FILE*, const char* format));

/* Fonctions spécifiques aux ABRs */

/**
 * @brief Renvoie le minimum d'un arbre binaire de recherche.
 * 
 * @param a 
 * @return Element 
 */
Element min(Arbre a);

/**
 * @brief Renvoie le maximum d'un arbre binaire de recherche.
 * 
 * @param a 
 * @return Element 
 */
Element max(Arbre a);

/**
 * @brief Extrait le noeud minimum d'un arbre binaire de recherche.
 * 
 * @param a 
 * @return Arbre 
 */
Arbre extract_min(Arbre* a);

/**
 * @brief Extrait le noeud maximum d'un arbre binaire de recherche.
 * 
 * @param a 
 * @return Arbre 
 */
Arbre extract_max(Arbre* a);

/**
 * @brief Détermine si un élément appartient à un arbre binaire de recherche.
 * 
 * @param a 
 * @param n 
 * @return true 
 * @return false 
 */
bool appartient(Arbre a, Element n);

/**
 * @brief Ajoute un mot à un arbre binaire de recherche.
 * Si l'élément existe déjà, son adresse est renvoyée
 * @param a 
 * @param n L'élément à ajouter.
 * @return Noeud* Adresse du noeud nouvellement créé, ou l'adresse
 * de l'élément existant, sinon NULL en cas d'erreur d'allocation
 */
Noeud* ajout(Arbre* a, Element n);

/**
 * @brief Supprime un élément d'un arbre binaire de recherche.
 * 
 * @param a 
 * @param n L'élement à supprimer
 * @return Arbre L'arbre résultant de la suppression, ou NULL
 * si l'élément n'existe pas.
 */
Arbre suppression(Arbre* a, Element n);

/**
 * @brief Crée un arbre avec un fichier contenant des mots séparés par des
 * séparateurs définis par la macro ``SEPARATORS``.
 *
 * @param nom Le nom du fichier à lire.
 * @param a L'arbre à remplir.
 * @return 0 en cas d'erreur d'ouverture du fichier, 0 en cas d'erreur
 * d'allocation, 1 sinon
 */
int cree_arbre(const char* nom, Arbre* a);

// Création de graphes //

/**
 * @brief Crée un fichier .dot et .pdf représentant un arbre binaire.
 * 
 * @param filename Nom du fichier à créer (sans extension)
 * @param a L'arbre à représenter
 */
void dessine(char* filename, Arbre a);

#endif
