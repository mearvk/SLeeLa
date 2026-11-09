# SLeeLa 3.0 — France (FR)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Un langage système natif, son compilateur, son environnement d’exécution et son écosystème de machines virtuelles.</strong></p>

> **Édition internationale par pays.** Ce fichier regroupe les versions linguistiques retenues pour la France. La traduction technique est réalisée progressivement. Les sections déjà traduites sont reproduites intégralement ci-dessous ; les sections qui restent à traduire sont signalées explicitement.

**The Now. The Ingredient.** Un langage ne se réduit pas à sa syntaxe : il comprend le compilateur, l’environnement d’exécution, les bibliothèques, les outils et les contrats de plateforme qui rendent les logiciels utilisables.

**Les domaines d’activité de SLeeLa.** SLeeLa s’intéresse à **Math**, **Object**, **Science** et **Reach** — les quatre dimensions de son travail : ce qui est calculable (Math), la chose établie dans le catalogue de ce qui existe (Object), l’étude rigoureuse du vrai (Science) et l’extension de la capacité d’action d’un système vers d’autres systèmes et vers les personnes (Reach). **Les intérêts nationaux demeurent** : aucune de ces quatre dimensions ne les écarte. Lorsqu’un sujet concerne l’intérêt d’une nation, cette dimension est conservée et fidèlement consignée, notamment dans les métadonnées de chaque classe, telles que `metaOrigin` et `metaNationalSchedule`.

## I. Pour commencer

- [Construire l’implémentation native](impl/README.md)
- [Machines virtuelles et Modium — guide pratique](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pile CPU dans les sources SLeeLa](lib/cpu/CPU.md) — une machine complète allant des portes logiques à l’ALU, aux cœurs, au système d’exploitation et aux programmes ; plus de 130 profils CPU et des [tensions, fréquences variables et temporisations avancées](lib/cpu/TIMING.md) réglables, permettant une exécution [aux spécifications ou en surfréquence](lib/cpu/components/README.md).
- [Index de la documentation](docs/README.md)
- [Source de données CSPML et référence de recherche Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — provenance et conseils d’intégration de données externes américaines sur les lieux, les conditions socioéconomiques, la main-d’œuvre, l’éducation et les écoles publiques.
- [Documentation API générée](docs/api/README.md) — référence HTML actuelle, édition versionnée de l’API et instructions de régénération.
- [Catalogue des API](api/README.md)
- [Normes de comparaison computationnelle](comparison/README.md) — normes de types, mémoire, ABI et conformité pour la VM SLeeLa, Java/JVM et le C/C++ natif.
- [Stanchion de récursion — prototypes C et C++](recursion/stanchion/README.md) — exemples spécialisés pour les types primitifs Boolean, Byte, Character, Integer, Long, Float et Double, ainsi que des modèles de récursion génériques, appliqués, scientifiques et pédagogiques.
- [Matrice de prise en charge et de validation des plateformes](docs/PLATFORM-SUPPORT.md)
- [Guide de marque](docs/BRAND-GUIDE.md)
- [Concepts et ressources de produits dérivés](merchandise/README.md)
- [Logo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Politique de test et critères de publication](markdown/TESTING.md)

## II. Actualités

**Traduction en attente.** Les annonces détaillées et les descriptions de fonctionnalités de la section II anglaise seront traduites dans une prochaine série. Source de référence : [README.md – section II](README.md).

## III. Construction et tests

**Prérequis :** une chaîne d’outils C/C++ (GCC ou Clang) et GNU Make ; Python 3 pour les outils de manifeste et de flux ; sous Linux, les paquets de développement X11/Xrandr pour les produits d’affichage. La commande `make deps` d’un module indique ou installe les dépendances nécessaires. La construction de `impl/` échoue de manière fermée si le manifeste SHA-256 de confiance n’est pas respecté ; le `Makefile` racine le référence automatiquement.

Pour démarrer rapidement depuis un clone propre :

```sh
make                                  # construire tous les éléments depuis la racine du dépôt
```

Ou construire et tester uniquement le cœur natif :

```sh
make -C impl all                      # construire uniquement le compilateur et le cœur d’exécution
bash ./test-suites/run-all.sh --all   # exécuter toute la suite de tests
```

La commande `make` à la racine du dépôt construit tous les produits (cœur, VM, codecs, churn, antivirus, etc.), puis termine par la construction du **registre** et la matérialisation de sa table de référence. Un arbre construit contient ainsi un registre vérifiable dès le départ.

Cibles essentielles :

```sh
make                     # tout construire (alias de make all ; registre exécuté en dernier)
make all                 # même construction complète, nommée explicitement
sudo make install        # installer les commandes et les fichiers d’exécution (voir § IV)
make clean               # supprimer les sorties de construction de tous les systèmes invoqués
make help                # afficher toutes les cibles du Makefile racine
```

Autres cibles utiles :

```sh
make registry            # construire uniquement le registre et matérialiser la table
./build-registry.sh      # même opération, exécutée directement
make tests               # construire et exécuter la suite de tests
make ballum              # construire la bibliothèque d’affichage Ballum pour ce système
bin/install-modules      # installation/construction cadencée des modules sur environ 10 à 20 minutes
```

La cible `registry` exécute [`build-registry.sh`](build-registry.sh) : elle construit le cœur du registre, initialise la table de référence dans le stockage texte portable `registry/build/sleela.sleelareg` et exporte le script de table MySQL `registry/build/registry.sql` (avec `sleela registry export`). Le registre peut être examiné avec `sleela registry` ou `registry/build/sleela-registry review`. Voir le [guide du registre](registry/README.md) et l’[architecture Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Consultez les guides de construction et de test pour les prérequis et les étapes propres à chaque plateforme. La réussite de la construction d’un composant ne prouve pas que chaque bibliothèque ou plateforme est prête pour une publication.

## IV. Installation

Après la construction des produits natifs, installez les commandes et les fichiers de prise en charge de l’environnement d’exécution :

```sh
sudo make install
make install PREFIX="$HOME/.local"        # installation pour l’utilisateur, sans sudo
make install DESTDIR="$PWD/package-root" # installation dans un répertoire de préparation de paquet
make install-check
```

L’installateur conventionnel utilise par défaut `/usr/local/bin`, `/usr/local/lib/sleela` et `/usr/local/share/sleela`. Il préserve une configuration de démarrage existante et refuse d’écraser des commandes existantes non suivies par le dépôt. La désinstallation ne supprime que les fichiers enregistrés par l’installateur ; elle préserve la configuration et les données d’exécution non suivies.

Pour un déploiement ciblé et guidé du seul paquet principal — les binaires `sleela`, `sleelvac` et `nordshrift`, la bibliothèque standard, les protocoles et les modules de prise en charge — utilisez l’installateur propre :

```sh
make clean-install       # installation guidée du seul paquet principal
```

Le processus demande d’abord si le paquet principal doit être installé, puis permet de choisir le périmètre. En cas de réussite, un sélecteur facultatif propose des modules d’amélioration sélectionnés. Le déroulement interactif complet et toutes les options sont décrits dans [Installation rapide et sûre](install/README.md).

## V. Langage et environnement d’exécution

La grammaire faisant autorité est [`SLEELA.syntax`](SLEELA.syntax). Le dépôt documente actuellement la syntaxe **3.0** ; la version du compilateur/des outils et la version de syntaxe du code source sont distinctes. La surface 3.0 ajoute les marqueurs documentaires **`pure` et `gasoline`** (§30) : deux Preonems contextuels, déclaratifs et sans effet d’exécution propre, qui expriment chacun la position d’un document par sa propre honnêteté. La syntaxe 3.0 rassemble les surfaces linguistiques 1.0 à 2.9 dans l’édition normative actuelle, tout en conservant les versions minimales requises pour chaque fonctionnalité.

La surface précédente, 2.9, ajoute les **champs conséquents `@mix`** (§28) : une classe peut déclarer plusieurs champs d’instance portant le même nom lorsque chacun est marqué `@mix`. Le langage les traite comme un groupe indexé (de haut en bas `1,2,3,…` par défaut, ou défini manuellement avec `@mix(1,2,3,...)`, dont le nombre d’indices doit correspondre au nombre de champs). Elle reprend également les familles d’entiers larges signés et non signés en base 2, de `int32` à `int16777216` (§24) et de `uint32` à `uint16777216` (§25), qui prolongent `long`, `int64` et `int128` (§21) ; les documents de prise en charge des mots-clés (§22) ; ainsi que les **Preonems** : marqueurs documentaires déclaratifs `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) et `artifact` (§35 ; statut de qualité législative évalué à l’échelle du temps et de l’espace). L’ensemble porte le nom de Preonems (§26.1).

La résolution de surcharge des constructeurs ainsi que la délégation vers `this(...)` et les constructeurs de classe de base sont explicitement hors du contrat de syntaxe 1.8 vérifié. Consultez la grammaire et les notes d’implémentation avant de vous appuyer sur une fonctionnalité. Les neuf unités sources SleelaDoc déclarent désormais `#sleela 3.0` ; le contrôle commun du compilateur accepte les syntaxes source 1.0 à 3.0. La nouvelle construction de l’API est produite par `make api-docs`.

Le dépôt comprend une implémentation native C/C++, des références de génération SLVM, des modules de bibliothèque standard, des tests, des exemples et des intégrations de plateforme. L’état d’implémentation et de validation varie selon les modules ; consultez leurs spécifications et les résultats CI actuels plutôt que de supposer une complétude uniforme.

## VI. Principes d’ingénierie

- **Les preuves avant les affirmations :** distinguer les comportements implémentés, testés, expérimentaux et prévus.
- **Un contrat par frontière :** documenter la syntaxe, les API, les formats de fichiers, les hypothèses de plateforme et les comportements en cas d’échec.
- **Conception portable :** isoler les backends propres aux plateformes et tester sur des exécuteurs natifs.
- **Échec sûr :** tester les entrées mal formées, les limites de ressources, les défaillances de processus et les chemins de nettoyage.
- **Publications reproductibles :** reconstruire proprement et publier les empreintes des artefacts ainsi que les notes de version.

## VII. Construction de l’antivirus et intégrité

Le paquet antivirus Sleelavirin comprend un scanner SHA-256 natif, un adaptateur ClamAV facultatif, des manifestes d’intégrité pour les sources et les index, ainsi que des sommes de contrôle pour les paquets de plateforme. Construisez et testez le paquet depuis la racine du dépôt :

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Le manifeste d’intégrité est généré sous `antivirus/build/`. Il couvre les sources et les documents suivis par le dépôt ainsi que les fichiers d’index de signatures protégés ; les sorties générées et le cache ClamAV modifiable en sont exclus. Les instantanés de bases de données ClamAV disposent de leurs propres manifestes. Les archives ZIP de publication comprennent `INTEGRITY-MANIFEST.json`, `SHA256SUMS` et un fichier compagnon `.sha256`. Vérifiez les empreintes des archives téléchargées à l’aide d’une valeur de confiance obtenue indépendamment : une somme de contrôle stockée à côté d’une archive ne constitue pas une signature de l’éditeur. Voir le [guide Sleelavirin](antivirus/README.md), la [liste de contrôle de sécurité](antivirus/SECURITY.md) et le [workflow de construction antivirus](.github/workflows/antivirus-binaries.yml).

Une publication n’est considérée comme vérifiée que lorsque les tâches CI pertinentes réussissent sur le commit exact de cette publication et que les artefacts obtenus ont été contrôlés.

## Sections restant à traduire

Les sections suivantes ne sont pas encore traduites et seront ajoutées dans de prochains lots : **II. Actualités ; VIII. Thèmes d’éditeur SLeeLa ; IX. Contribution ; X–XVII. Autres domaines du projet ; XVIII. Ajouts récents ; XIX. Éditions internationales README.** Jusqu’à leur traduction complète, le [README.md canonique en anglais](README.md) demeure la référence exhaustive.

---
**État de traduction :** traduction partielle – sections I et III à VII traduites. Les identifiants techniques, commandes, exemples de code, noms de fichiers et liens ont été conservés à l’identique.
