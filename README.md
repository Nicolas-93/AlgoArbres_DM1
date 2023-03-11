---
title: Algorithmique des arbres
subtitle: DM1
author:
    - Nicolas SEBAN
    - Amir POUYANFAR
geometry: margin=3cm
...

# Compilation

Afin de compiler le filtreur de mots, ainsi que ``test_ABR`` vous pouvez utiliser la commande suivante :

```shell
make
```

# Contenu

Ce rendu est composé de deux programmes :

- ``filtrage`` : Un outil permettant de filtrer les mots d'un texte, selon les mots présents dans un autre.
- ``test_ABR`` : Un *mini-shell* permettant de construire étape par étape un arbre binaire de recherche.

# Utilisation

- Le programme ``filtrage`` nécessite deux arguments, le premier le texte cible, le second le texte contenant les mots à supprimer du premier texte.

  Sur la sortie standard sera affiché, dans l'ordre, les mots présents dans le texte de référence, puis les mots communs aux deux textes.\
  Les arguments optionnels suivants sont disponibles :
  
  - ``-v`` : permet de créer les ``.dot`` et ``.pdf`` des arbres crées (le texte de base, le texte filtre, les mots communs et la différence de base avec filtre).
  - ``-h`` : Affiche un message d'aide.

- Le programme ``test_ABR`` vous permet de tester interactivement la construction d'un arbre.

  Vous pouvez utiliser les commandes suivantes à l'intérieur du shell :

  - ``a`` : Ajoute un mot
  - ``d`` : Supprime un mot
  - ``p`` : Parcours préfixé
  - Ctrl + D : Termine le programme

Exemple :

```shell
>>> a de
>>> a Au
>>> a clair
>>> a la
>>> a lune
>>> p
Au
clair
de
la
lune
>>> 
```

Il est également possible d'ajouter en argument un fichier texte, afin de créer un arbre de celui-ci sous forme d'un fichier Graphviz et pdf.

# Remarques

- Vous pouvez utiliser la commande ``make clean_outputs`` afin de supprimer tous les fichiers dot et pdf créés.
