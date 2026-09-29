# Minishell

Réimplémentation en C d'un interpréteur de commandes inspiré de Bash : analyse syntaxique de la ligne de commande, gestion des quotes et de l'expansion de variables, exécution de processus chaînés par pipes, redirections, heredocs, builtins et gestion des signaux.

Projet réalisé **en binôme** dans le cadre du cursus **École 42 Paris** (2023), avec [bfiguet](https://github.com/bfiguet).

> **Répartition du travail** — j'ai pris en charge l'**analyse syntaxique** (découpage, quotes, expansion, validation) et les **builtins**. Mon binôme a développé la partie **exécution** (processus, pipes, redirections, heredocs). Les en-têtes de chaque fichier indiquent son auteur.

---

## Objectif

Reproduire le comportement de Bash sur un sous-ensemble défini de fonctionnalités, en n'utilisant que les appels système et la bibliothèque `readline` — aucun recours à une bibliothèque de parsing existante.

Contraintes : C99, compilation sans avertissement sous `-Wall -Wextra -Werror`, aucune fuite mémoire, une seule variable globale autorisée (réservée à la gestion des signaux).

---

## Fonctionnalités

**Analyse syntaxique**

- Découpage de la ligne en commandes séparées par des pipes
- Gestion des guillemets simples et doubles, avec les règles de Bash : les simples inhibent toute interprétation, les doubles laissent passer l'expansion de variables
- Expansion des variables d'environnement (`$VAR`) et du code de retour (`$?`), y compris à l'intérieur des guillemets doubles et dans les cibles de redirection
- Détection des redirections (`<`, `>`, `>>`, `<<`) même accolées aux arguments, sans espace
- Validation syntaxique préalable : quotes non fermées, pipes ou redirections mal placés — l'erreur est signalée avant toute tentative d'exécution

**Builtins**

`echo` (avec l'option `-n`), `cd` (chemins relatifs et absolus, mise à jour de `PWD` et `OLDPWD`), `pwd`, `export` (déclaration, réaffectation, concaténation via `+=`, affichage trié), `unset`, `env`, `exit` (validation de l'argument numérique et propagation du code de sortie).

Les builtins doivent modifier l'environnement du shell lui-même : lorsqu'ils ne sont pas dans un pipeline, ils s'exécutent dans le processus principal, sans `fork`.

**Exécution** *(développée par mon binôme)*

Création des processus par `fork`/`execve`, résolution des exécutables dans le `PATH`, chaînage par pipes, redirections d'entrée et de sortie, heredocs, et gestion des signaux `Ctrl+C`, `Ctrl+D` et `Ctrl+\` selon le contexte — invite de commande, heredoc ou commande en cours.

---

## Structures de données

La ligne analysée est représentée par une **liste doublement chaînée de tokens**, un par commande du pipeline :

```c
typedef struct s_token
{
    char            *value;   // commande brute
    char            **args;   // arguments découpés
    char            **red;    // redirections associées
    int             fds[2];   // descripteurs d'entrée/sortie
    struct s_token  *next;
    struct s_token  *prev;
}   t_token;
```

Le chaînage bidirectionnel permet de remonter le pipeline lors de la fermeture des descripteurs, sans avoir à conserver de référence séparée.

L'environnement est maintenu sous **deux représentations simultanées** : un tableau `char **env` transmis tel quel à `execve`, et une liste chaînée `t_export` conservant le couple nom/valeur. Cette seconde structure est nécessaire car `export` peut déclarer une variable sans valeur — état que Bash distingue d'une variable vide, et qu'un simple tableau de chaînes ne permet pas de représenter.

---

## Organisation

```
srcs/
├── main/       boucle de lecture, signaux
├── init/       initialisation des structures et de l'environnement
├── parse/      découpage, quotes, expansion, validation syntaxique
├── builtin/    echo, cd, pwd, export, unset, env, exit
├── exec/       fork, execve, résolution du PATH, pipes
├── redir/      redirections et heredocs
└── utils/      listes, libérations, garde-fous
libft/          bibliothèque C personnelle réutilisée
include/        en-tête du projet
```

---

## Compilation et utilisation

Dépendance : `libreadline-dev`.

```bash
make
./minishell
```

Vérification des fuites mémoire — le fichier de suppressions écarte les allocations internes à `readline`, qui ne sont pas libérables depuis le programme :

```bash
valgrind --leak-check=full --suppressions=valgrind.suppr ./minishell
```

---

## Ce que le projet m'a apporté

- **Analyse syntaxique** — passer d'une chaîne brute à une structure exploitable oblige à formaliser des règles qu'on applique sans y penser en tant qu'utilisateur. Le comportement des quotes en est l'exemple le plus net : simple à utiliser, laborieux à reproduire fidèlement.
- **Le shell comme programme** — comprendre pourquoi `cd` ne peut pas être un exécutable externe, et plus largement ce qui distingue une commande intégrée d'un binaire du `PATH`.
- **Représentation des données** — constater qu'une structure mal choisie rend certains comportements impossibles à reproduire, et qu'il vaut mieux corriger la structure que multiplier les cas particuliers.
- **Rigueur mémoire** — libérer intégralement à chaque itération de la boucle, dans un programme conçu pour tourner indéfiniment.
- **Travail en binôme** — découper un projet suivant une interface claire entre analyse et exécution, et s'y tenir.
