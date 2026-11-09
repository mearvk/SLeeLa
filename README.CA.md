# SLeeLa 3.0 — Canada (CA)

> **Édition internationale.** Cette édition canadienne regroupe les présentations en anglais et en français. Les identifiants techniques, la syntaxe, le code source, les commandes, les noms de fichiers et les noms d’API sont conservés tels quels.

## English

SLeeLa is a native systems programming language, compiler, runtime, and virtual-machine ecosystem. This edition is intended for readers in Canada.

## Français

SLeeLa est un langage de programmation système natif, accompagné d’un compilateur, d’un environnement d’exécution et d’un écosystème de machines virtuelles. Cette édition s’adresse au public francophone du Canada.

**The Now. The Ingredient.** Un langage ne se réduit pas à sa syntaxe : il comprend aussi le compilateur, l’environnement d’exécution, les bibliothèques, les outils et les contrats de plateforme qui rendent les logiciels utilisables.

## I. Pour commencer

- [Compiler l’implémentation native](impl/README.md)
- [Machines virtuelles et Modium — guide pratique](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pile CPU en SLeeLa](lib/cpu/CPU.md) — des portes logiques à l’ALU, aux cœurs, au système d’exploitation et aux programmes; plus de 130 profils CPU et des réglages de tension, d’horloge et de temporisation dans [TIMING.md](lib/cpu/TIMING.md).
- [Index de la documentation](docs/README.md)
- [Documentation API générée](docs/api/README.md) et [catalogue API](api/README.md)
- [Normes de comparaison informatique](comparison/README.md) — types, mémoire, ABI et conformité pour SLeeLa VM, Java/JVM et C/C++ natif.
- [Recursion stanchion — prototypes C et C++](recursion/stanchion/README.md)
- [Matrice de prise en charge et de validation des plateformes](docs/PLATFORM-SUPPORT.md)
- [Guide de marque](docs/BRAND-GUIDE.md)
- [Politique de tests et critères de publication](markdown/TESTING.md)

## II. Compilation et tests

**Prérequis :** une chaîne d’outils C/C++ (GCC ou Clang), GNU Make et Python 3 pour les outils de manifeste et de flux de travail. Sous Linux, les produits graphiques peuvent également nécessiter les paquets de développement X11/Xrandr.

Depuis une copie propre du dépôt :

```sh
make
```

Pour compiler uniquement le cœur natif et exécuter la suite complète de tests :

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

La commande `make` à la racine construit les produits distribués par le dépôt. Les cibles essentielles comprennent :

```sh
make all
sudo make install
make clean
make help
```

Pour consulter les cibles disponibles, les prérequis propres à chaque plateforme et les limites de validation, utilisez les guides de compilation et de test. La réussite de la compilation d’un composant ne prouve pas que toutes les bibliothèques ou plateformes sont prêtes pour une publication.

## III. Installation

Après la compilation, installez les commandes et les fichiers de prise en charge de l’environnement d’exécution :

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

L’installation conventionnelle utilise par défaut `/usr/local/bin`, `/usr/local/lib/sleela` et `/usr/local/share/sleela`. Pour une installation locale à l’utilisateur, `PREFIX="$HOME/.local"` évite normalement de recourir à `sudo`. Le programme d’installation conserve la configuration de démarrage existante et refuse d’écraser des commandes existantes qui ne sont pas suivies par lui.

Pour installer uniquement le paquet principal — les exécutables `sleela`, `sleelvac` et `nordshrift), la bibliothèque standard, les protocoles et les modules de prise en charge — utilisez :

```sh
make clean-install
```

Consultez le [guide d’installation rapide et sûre](install/README.md) pour le déroulement guidé et les autres options.

## IV. Langage et environnement d’exécution

La grammaire normative se trouve dans [`SLEELA.syntax`](SLEELA.syntax). Le dépôt documente la syntaxe **3.0**. La version du compilateur et celle de la syntaxe du code source sont deux notions distinctes. La syntaxe 3.0 rassemble les fonctionnalités des versions 1.0 à 2.9 et conserve les exigences minimales de version propres à chaque fonctionnalité.

Les documents de référence décrivent notamment les annotations `@mix`, les familles d’entiers étendus, les marqueurs documentaires Preonem, SleelaDoc, la génération de documentation API et les outils Nordshrift. Les noms techniques et les exemples de code ne sont pas traduits afin de rester directement utilisables.

## V. Principes d’ingénierie et validation

Les principes du projet comprennent la priorité aux preuves, des contrats explicites aux frontières des composants, la portabilité dès la conception, un comportement sûr en cas d’échec et des versions reproductibles.

La validation doit être évaluée pour chaque module et chaque plateforme. Les contrôles ignorés ne doivent pas être présentés comme réussis; un résultat de compilation isolé ne constitue pas une validation globale de la publication.

## VI. Éditeurs et contributions

SLeeLa fournit des thèmes et une intégration syntaxique pour **Vim** et **Emacs**, ainsi qu’un outil natif de palettes C/C++. Les instructions figurent dans le [guide d’intégration des éditeurs](linux-editors/README.md).

Pour contribuer :

1. Lire l’index de documentation et la politique de tests.
2. Effectuer des changements ciblés avec des tests positifs et négatifs.
3. Exécuter les tests ciblés, puis la suite étendue.
4. Indiquer le système d’exploitation et la chaîne d’outils utilisés; ne pas déclarer réussis les contrôles qui ont été ignorés.

SLeeLa est développé par **MEARVK LLC**.

## VII. Autres domaines documentés

Le README principal présente également les bibliothèques Advantage, SleelaDoc et XML DOM; les primitives d’entiers; le runtime de caractères; AutoCAD Creator™; le catalogue expérimental UTF-4088; les outils Pi, Modium, Registry et `sleelactl`; ainsi que le chemin initial de compilation native demandé par `@native`.

## Documentation technique complète

Le [README.md canonique](README.md) contient le catalogue complet des fonctionnalités, la référence du langage et de l’environnement d’exécution, les instructions de compilation et d’installation, les liens API, les exemples et les notes de version. Cette édition canadienne ajoute une présentation et des instructions essentielles en français; elle ne constitue pas encore une traduction intégrale, ligne par ligne, des sections techniques I–XIX. Le README anglais demeure la référence normative jusqu’à la révision complète de chaque traduction.

- [README canonique](README.md)
- [Spécification de syntaxe SLeeLa](SLEELA.syntax)
- [Index de documentation](docs/README.md)
- [Prise en charge des plateformes](docs/PLATFORM-SUPPORT.md)
- [Dépôt GitHub](https://github.com/mearvk/SLeeLa)

**État de traduction :** présentation, démarrage, compilation, installation, principes d’ingénierie et contributions localisés en français. La traduction intégrale du README canonique reste à effectuer et à faire réviser par des spécialistes techniques francophones.
