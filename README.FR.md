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

Ajouts récents au langage, aux pilotes et à la bibliothèque standard :

- **Paquet de bibliothèque standard `advantage` — optimisation d’un dividende, avec mathématiques, physique, intégration, évaluation et sortie routable.** [`lib/advantage/`](lib/advantage/README.md) encapsule l’« optimisation d’un dividende unique par rapport à un modèle » sous la forme d’un **Sequitur onion** à couches (dividende → révision → préoccupations → mathématiques → additions → soustraction → regroupement → itération), navigable vers l’intérieur et l’extérieur avec mise à jour dynamique et restitution complètes, au moyen d’un **modèle utilisateur** simple et d’un **modèle développeur** offrant un contrôle intégral. Il comprend cinq domaines : **`math/`**, économie du commerce bilatéral (maximisation conjointe des profits, négociation de Nash, ratio critique bilatéral du modèle newsvendor) ; **`physics/`**, « physique des champs de confiance » appliquée à l’économie structurelle (confiance comme tenseur métrique, géodésique transactionnelle, structure de contact d’annonce et SDE de décroissance de confiance CIR avec sauts — une analogie de modélisation explicitement signalée) ; **`integration/`**, modèle d’intégration verticale composable récursivement pour le capitalisme et le commerce de détail (compartiments, baies d’ingénierie, compartiments d’essai, plateaux de médiane newtonienne et score de sophistication du système) ; **`grading/`**, évaluateur de circuits de sophistication attribuant des notes A–D à chaque module et à tout ensemble de modules gagnants, perdants, évaluateurs ou absolus ; et **`output/`**, interface d’émission routable vers le terminal, une surface au format **CommonRails** ou un socket/connecteur (prévue pour un `slnet_write` natif). Chaque classe porte l’enregistrement Metadata-Class ; cinq démonstrations exécutables valident les domaines. Les documents complémentaires [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) et [`PHYSICS.md`](lib/advantage/PHYSICS.md) les décrivent. Un consommateur natif C/C++ dans [`advantage/`](advantage/README.md) — noyau C `advantage_value`/`advantage_mean`, magasin de connaissances XML et bibliothèque sécurisée d’acquisition HTTPS/SFTP/SSH, magasin de clés et transport DH/ECDH — relie le modèle à des [exemples de commerce](advantage/examples/README.md) : transactions positives issues de sources Internet supposées, séquence de pertinence pour Traders, Consumers, Helpers et Healers, et huit modèles classés de A à D (D = Cement maximal ; A = Brand maximal).
- **Annotations de temporisation des boucles — `@loopDelay` / `@loopBudget` et fonction intégrée `sleep(ms)`.** Il s’agit des premières annotations **comportementales au niveau de l’instruction** de SLeeLa : écrites sur leur propre ligne avant une boucle `for`, `while` ou `do`, elles insèrent une véritable pause dans son exécution. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` attend une durée fixe par itération (la forme abrégée `@loopDelay(250)` utilise les millisecondes par défaut) ; `@loopBudget(seconds = 10, iterations = 1000)` répartit un nombre connu d’itérations sur un budget total de temps réel. Les deux annotations sont abaissées vers le nouvel opcode append-only **`OP_SLEEP`** (ISA 157), qui prend également en charge la fonction intégrée générale **`sleep(<ms>)`**, appelable partout. La pause est implémentée de façon portable par `nanosleep` / `Sleep`. Spécification : [`SLEELA.syntax` §32](SLEELA.syntax) ; référence API : [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md) ; démonstration exécutable : [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela). Un index des ajouts de session — comprenant également la multiplication native de grands entiers `OP_BIGMUL` et les outils autonomes [local-generator](local-generator/README.md) et [pi-generator](pi-generator/README.md) — se trouve dans [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). Les trois surfaces API générées ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) et l’instantané immuable [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) ont été reconstruites avec `make api-docs` et incluent désormais les entrées `SLISASleep` / `SLISABigMul` sur la page du paquet `vm`.
- **SleelaDoc depuis la ligne de commande — `sleela docs`.** SleelaDoc est désormais accessible directement depuis le terminal. `sleela docs` répertorie tous les sujets documentables ; `sleela docs crypto` affiche un paquet ; `sleela docs stringbuilder` résout une classe sans distinction de casse et reconnaît le préfixe standard `SL` comme `SLStringBuilder`. La commande affiche une page détaillée — **définition, objectif de conception, relations, structure, user clays et échelle de maîtrise à cinq niveaux** — avec des suggestions classées de type « vouliez-vous dire ? » en cas de nom proche. Les auteurs enrichissent chaque page en ajoutant des balises SleelaDoc `@` telles que `@intent`, `@thesis`, `@axiom`, `@param`, `@see` et `@mastery`. `sleela docs --generate [dir]` génère un corpus en texte brut (une page par classe et un index `TOPICS.index`) que la CLI peut ensuite lire hors ligne ; **`make sleeladoc`** regroupe ce corpus et les sources `.sleela` correspondantes dans `sleeladoc/build/sleeladoc.jar`. Pour les sujets ne disposant que d’une façade, comme `psychiatry` et `politico`, `sleela docs --facade <topic>` explore le système de fichiers à l’exécution et énumère les dossiers réels, puis les fichiers, avec des limites configurables par l’utilisateur de **6 Mo, 6 000 fichiers et 6 minutes**. Les unités de `lib/sleeladoc` utilisent l’édition actuelle **`#sleela 3.0`**. Voir [`man/man1/sleela.1`](man/man1/sleela.1) et [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — documentation de style Javadoc et site HTML généré.** **SleelaDoc** est la convention de commentaires documentaires intégrés au code source de SLeeLa (commentaires entre chevrons délimités par `@doc` / `@page` ; voir [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)). [`lib/sleeladoc`](lib/sleeladoc/README.md) est l’**extracteur** qui transforme une unité `.sleela` en page API Markdown ou HTML, à la manière de `javadoc` pour Java, en documentant l’*intention*. Un site HTML complet et navigable pour l’ensemble de `/lib` est généré dans [`/sleeladoc-api`](sleeladoc-api/README.md) par `tools/generate-sleeladoc-api.py` (ouvrir `sleeladoc-api/index.html`) ; il utilise un style bleu, bleu clair, gris clair et blanc, une hiérarchie paquet → classe → membre et un glossaire destiné au lecteur *Student 2*. Le compilateur produit également, en complément de la compilation, des documents marqués par l’auteur (`--wiki` : page MediaWiki et copie jumelle de la source d’origine), décrits dans [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Démonstration exécutable : [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modèle objet de document XML (`lib/xml`).** DOM XML côté SLeeLa : un document possède un arbre de nœuds élément/texte/commentaire/CDATA/instruction de traitement, avec enfants ordonnés et attributs nommés ; il comprend un sérialiseur compact ou lisible (échappement XML 1.0 §2.4), un analyseur par cellules de caractères et le registre qualité `SLXmlAspect`. L’égalité des nœuds suit trois niveaux de confiance (identité → même sujet → structure) et est réflexive, symétrique et déterministe. Voir [`lib/xml/README.md`](lib/xml/README.md) et l’exemple exécutable [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Primitives d’entiers signés de grande largeur (`long`, `int64`, `int128`).** `int`, `long` et `int64` sont tous des entiers signés de 64 bits qui sont vérifiés par le système de types et exécutés de manière identique. `int128` est accepté comme type entier (adossé à 64 bits pour le moment ; un véritable support 128 bits est documenté comme travail ultérieur). Les séries signées en base 2 `int32` à `int16777216` (syntaxe 2.7) et non signées `uint32` à `uint16777216` (syntaxe 2.8) étendent ces capacités. Voir [`SLEELA.syntax`](SLEELA.syntax) §21, §24 et §25.
- **Champs conséquents `@mix` (syntaxe 2.9).** Une classe peut déclarer plusieurs champs d’instance portant le **même nom** lorsque chaque répétition comporte `@mix`. Le langage traite le groupe comme une séquence (tableau / liste indexée), de haut en bas `1,2,3,…` par défaut ou définie manuellement avec `@mix(1,2,3,...)` ; le nombre d’indices doit correspondre au nombre de champs. Voir [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Marqueurs documentaires — le Processus naturel, appelé Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Ces mots nus servant de marqueurs documentaires sont collectivement appelés **Preonems** (un marqueur est un *Preonem* ; l’ensemble, les *Preonems* ; voir [`SLEELA.syntax`](SLEELA.syntax) §26.1). Ce sont des marqueurs documentaires déclaratifs de première classe qui expriment ce qu’un document *est* dans l’ordre de sa création : `author`/`authorial` (syntaxe 2.4 ; géré par le conseil professionnel, seul membre ayant un effet comportemental), `material;` (Source établie) et `memory;` (syntaxe 2.5), `determine;` (syntaxe 2.6 ; document ouvert à un traitement ultérieur par un médecin, praticien ou ghost jusqu’à son achèvement par voie légale), puis les deux plus récents : **`object;`** (chose établie dans le catalogue de ce qui existe, réifiée et dotée d’une existence concrète en syntaxe 2.8) et **`field;`** (champ relationnel dans lequel les objets existent et se déplacent), tous deux disponibles à partir de la syntaxe 2.6. Chaque marqueur est contextuel et conditionné par la version dans le frontend ; il ne modifie ni l’analyse syntaxique, ni le typage, ni la génération de code. En dehors de la position de marqueur, chaque mot reste un identifiant ordinaire : `field` n’interfère donc pas avec un membre de classe ou un accès `.field`. Voir [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 et §26.
- **Nordshrift en syntaxe 3.0 avec accès complet à la bibliothèque.** Nordshrift résout les noms via le même index récursif `/lib` que le compilateur et atteint ainsi tous les paquets et unités `.sleela` ; `nordshrift check` indique les nombres pour l’ensemble de l’arborescence.
- **Stanchion de récursion.** [`recursion/stanchion/`](recursion/stanchion/README.md) comprend désormais des prototypes de référence C et C++ dédiés aux sept contrats primitifs documentés, ainsi que des exemples génériques, appliqués, scientifiques et pédagogiques. Il s’agit de sources de référence : compilez-les et testez-les avant de les considérer comme vérifiées.
- **Commentaires entre chevrons (`<` / `<<`).** Nouveau style de commentaire pour les développements futurs : `< line >` et `<< block >>`, reconnu par le frontend SLeeLa et documenté dans [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Littéraux entiers avec base.** Les littéraux `int` acceptent désormais les formes hexadécimale (`0x`), binaire (`0b`) et octale (`0o`), avec séparateurs de chiffres `_`, dans le langage source SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4) et dans le format de modèles CPU [`.xlt`](lib/xlt/README.md).
- **Norme Metadata Class dans toute la bibliothèque.** Le métapatron de métadonnées descriptives (provenance, légalité, portée juridictionnelle) est désormais présent dans chaque fichier `.sleela` contenant des classes sous `/lib`. Voir [`SLEELA.syntax`](SLEELA.syntax) §15 et [la leçon du tutoriel](tutorial/13-metadata-class.md).
- **Famille de texte / String enrichie.** `lib/text` ajoute de véritables assistants dans le langage, une vue par cellules de caractères proposant les opérations réelles `charAt`, `substring`, `indexOf`, `reverse` et l’alignement de colonnes, ainsi qu’un moteur de modèles positionnels. Voir le [tutoriel texte et String](tutorial/14-text-and-strings.md) et la démonstration exécutable [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` de première classe dans les feuilles Nordshrift.** Une feuille `.sst` peut déclarer un bloc typé `strings:` qui contribue au manifeste des composants de la compilation. Voir la [référence Nordshrift Sheet](wiki/Nordshrift-Sheet-Reference.md).
- **Modèles CPU exécutés par opcodes et équivalence d’appels de fonction.** Chaque `SL<ARCH>CPU` hérite de la capacité d’exécuter un flux d’instructions natives via sa boucle fetch-decode-execute, ainsi que le code d’un autre processus supposé par des appels équivalents sur l’infrastructure VM partagée. Voir [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

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

## VIII. Thèmes de l’éditeur SLeeLa — Vim et Emacs

SLeeLa comprend un ensemble coordonné de thèmes sombres professionnels pour **Vim** et **Emacs**, ainsi qu’un utilitaire natif de palettes C/C++. Le thème d’origine **American Professional** reste le thème par défaut. Dix palettes complémentaires proposent des inspirations de conception culturelles et institutionnelles variées ; il s’agit d’interprétations graphiques et non de représentations officielles d’une nation, d’une culture, d’une cour ou d’une institution.

### Les onze thèmes et leurs couleurs

| Thème | Arrière-plan | Premier plan | Mots-clés | Types | Chaînes | Commentaires |
|---|---|---|---|---|---|---|
| **American Professional** | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| **Pan-Eurasian** | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| **Russian and Slavic — Medium** | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| **Central Asian / Silk Road** | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| **East Asian** | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| **ICC — Cour pénale internationale** | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| **SCOTUS — Cour suprême des États-Unis** | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| **Greek (Hellenic)** | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| **Turkish (Al Bayrak)** | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| **Kiro** | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| **SLeeLa** | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

### Couleurs des drapeaux nationaux (palette source)

Les thèmes grec et turc sont dérivés des **couleurs officielles des drapeaux nationaux** des deux pays, indiquées ici avec leurs valeurs hexadécimales publiées. Les arrière-plans et accents des thèmes ci-dessus sont des interprétations assombries, adaptées aux éditeurs ; le tableau ci-dessous conserve les couleurs de référence telles qu’elles sont exposées au niveau national.

| Pays | Couleur | Hexadécimal | Fondement et signification |
|---|---|---|---|
| **Grèce** | Bleu | `#0D5EAF` | Bleu du drapeau national (loi 851/1978). Le ciel et les mers ; en héraldique, l’azur. |
| **Grèce** | Blanc | `#FFFFFF` | Blanc du drapeau national. Pureté de la lutte pour l’indépendance ; la croix représente le christianisme orthodoxe oriental. |
| **Türkiye** | Rouge (*al bayrak*) | `#E30A17` | Rouge du drapeau national (loi turque sur le drapeau). Le champ ; selon la tradition, le sang des martyrs et la souveraineté ottomane. |
| **Türkiye** | Blanc | `#FFFFFF` | Blanc du drapeau national. Le croissant et l’étoile : paix, pureté et symbole de l’héritage turcique/islamique. |

Les thèmes ancrent le premier plan de chaque éditeur sur le **blanc** national et leurs mots-clés, curseur, sélection et barre d’état sur le **bleu** national exact (`#0D5EAF`) ou le **rouge** (`#E30A17`). Les couleurs importantes sont donc littéralement les couleurs nationales. Il s’agit de palettes interprétatives ; la référence aux couleurs des drapeaux documente leur provenance et ne constitue pas une représentation officielle de l’un ou l’autre État. Les significations des couleurs suivent la description des symboles d’État du ministère grec des Affaires étrangères et la tradition de la loi turque sur le drapeau.

### Tableau des échantillons de couleur

Les cellules de palette ci-dessous utilisent des **échantillons SVG**, afin que GitHub affiche les couleurs réelles plutôt que des cellules HTML vides. Chaque bande présente six couleurs dans cet ordre : arrière-plan, premier plan, mots-clés, types, chaînes et commentaires. Les codes hexadécimaux exacts figurent à côté de chaque échantillon, dans le même ordre que le tableau de référence.

| Thème | Palette affichée | Codes hexadécimaux (arrière-plan · premier plan · mots-clés · types · chaînes · commentaires) |
|---|---|---|
| **American Professional** | ![Palette American Professional](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Palette Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Palette Russian and Slavic Medium](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Palette Central Asian Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Palette East Asian](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — Cour pénale internationale** | ![Palette ICC](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — Cour suprême des États-Unis** | ![Palette SCOTUS](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Palette grecque hellénique](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Palette turque Al Bayrak](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Palette Kiro](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Palette SLeeLa](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Des couleurs d’accent supplémentaires sont utilisées pour les fonctions, constantes, sélections, diagnostics et directives du préprocesseur. La palette Russian and Slavic utilise volontairement des tons moyens d’ardoise, de bleu, de sauge, de rubis et d’or ancien pour une apparence professionnelle plus apaisée.

### Installer les thèmes Vim

Depuis la racine du dépôt :

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Ajoutez ce qui suit à `~/.vimrc` pour activer la coloration syntaxique SLeeLa et choisir un thème :

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Choisissez exactement l’un des noms de thèmes suivants :

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Par exemple, utilisez `colorscheme sleela-russian-slavic` pour la palette russe et slave d’intensité moyenne, ou `colorscheme sleela-greek` / `colorscheme sleela-turkish` pour les palettes hellénique et Al Bayrak. Redémarrez Vim ou rechargez la configuration avec `:source $MYVIMRC`.

### Installer les thèmes Emacs

Créez un répertoire local pour les thèmes et copiez-y les fichiers de mode et de thèmes depuis la racine du dépôt :

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Ajoutez ceci à `~/.emacs` ou `~/.emacs.d/init.el` :

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Remplacez `sleela-american` par `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela` pour sélectionner un autre thème. Si Emacs demande s’il doit faire confiance au thème, examinez le fichier local du thème avant de l’accepter.

### Prévisualiser les palettes natives C/C++

Le petit utilitaire natif de palettes peut répertorier les thèmes, afficher leurs couleurs de terminal et rechercher une couleur individuelle :

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Pour assurer la compatibilité ascendante, `--palette` et `--hex COLOR` sans `--theme` sélectionnent American Professional. Les résultats de compilation et de test doivent être vérifiés sur le système cible et dans la CI ; la documentation d’une commande ne prouve pas qu’elle a effectivement réussi.

Voir le [guide d’intégration des éditeurs](linux-editors/README.md) pour les descriptions complètes des palettes et les détails de l’utilitaire.

## IX. Contribution

1. Consultez l’index de documentation et la politique de test.
2. Effectuez des modifications ciblées, accompagnées de tests positifs et négatifs.
3. Exécutez les tests ciblés, puis la suite étendue.
4. Indiquez les détails du système d’exploitation et de la chaîne d’outils ; ne présentez pas les vérifications ignorées comme réussies.

SLeeLa est développé par **MEARVK LLC**. Les anciens contenus de la page d’accueil sont conservés dans [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Attente de prélèvement sur les courriels publics Round

Pour l’initiative AE6E66, l’attente déclarée du projet est de **300 000 000,00 USD par courriel public Round**, en tant que montant de prélèvement fiscal proposé associé à chaque courriel Round Public répondant aux critères.

Cette déclaration consigne uniquement une attente du projet. Elle ne constitue **ni** une imposition fiscale légale, **ni** un ordre gouvernemental, **ni** un jugement, **ni** une obligation fiscale établie de manière indépendante. Tout prélèvement effectif nécessiterait l’autorité juridique applicable, une juridiction, des définitions, une procédure et une décision de l’autorité gouvernementale compétente.

## XI. Environnement d’exécution des alphabets et des jeux de caractères

La documentation de l’alphabet SLeeLa et l’environnement natif des caractères sont maintenus dans `alphabet/sleela/` et `lib/alphabet/native/`. Les tests natifs C++17 couvrent le décodage et l’encodage stricts des scalaires UTF-8, les signatures de capacité `C<n>` et l’appartenance bornée aux jeux de caractères :

```sh
make -C lib/alphabet/native test
```

Le catalogue expérimental UTF-4088 et son adaptateur de recherche SLeeLa sont documentés dans [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Les identifiants UTF-4088 appartiennent à un espace de noms distinct et ne sont pas des valeurs scalaires Unicode. Le travail de liaison au compilateur et à SLVM est suivi séparément de ces fondations natives.

## XII. AutoCAD Creator™

[**AutoCAD Creator™**](autocad/creator/README.md) est une application CAO 2D interactive entièrement écrite en SLeeLa. Alors que l’AutoCAD Renderer associé transforme un plan en dessin en une seule opération, Creator permet de **créer et modifier** en direct un document AutoCAD 2D : ajouter des murs, des pièces, des cotes et des notes, puis déplacer, renommer, changer de calque, supprimer et annuler les éléments placés, et **enregistrer** le résultat comme véritable dessin DXF AutoCAD (texte R12, ouvrable dans AutoCAD / LibreCAD).

Son interface graphique repose sur **SleelaUI™**, la propre boîte à outils d’interface multiplateforme de SLeeLa (dans [`lib/user-interface`](lib/user-interface)), et non sur JavaFX ; elle ouvre une véritable fenêtre native sous X11 / Cocoa / Win32. Sa conception suit une règle : **« SleelaUI présente ; SLeeLa décide »**. L’interface graphique ne calcule pas la géométrie et n’écrit pas le DXF. Chaque outil émet une intention de création, modification, suppression, annulation ou enregistrement au composant faisant autorité [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), propriétaire du document modifiable et d’un historique réversible des commandes ; le canevas ne fait que prévisualiser la géométrie détenue par Creator.

Le moteur d’édition réutilisable se trouve dans [`lib/autocad`](lib/autocad) : [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) et [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). Il fonctionne de bout en bout sans interface graphique. L’enveloppe graphique et la démonstration se trouvent dans [`autocad/creator`](autocad/creator) :

```sh
make creator          # depuis autocad/ : exécuter la démo Creator -> out/altered.dxf
```

Sur un système avec affichage, la fenêtre d’édition en direct s’ouvre ; sur un hôte sans interface graphique, le dessin est construit en mémoire, un rapport et le DXF sont affichés, puis le programme se termine proprement.

## XIII. Ordonnancement de la carte d’entrée UTF-4088

Le catalogue spécialisé UTF-4088 définit son parcours spatial de la carte d’entrée et ses itérations abstraites de pression dans [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). La politique d’index des matières du Registre national des privilèges est décrite dans [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Les coordonnées de carte, les niveaux de pression (1–50), les identifiants permanents du catalogue et les rangs alphabétiques sont des champs indépendants ; aucun ne doit être substitué silencieusement à un autre. La pression est un niveau d’itération algorithmique, et non une grandeur physique.

## XIV. Index des paquets récents et enrichis

Cet index ajouté met en évidence les paquets récemment enrichis, les bibliothèques métier et les collections de code source. Les entrées sont regroupées par ordre alphabétique selon le nom du paquet ou le sujet. Les descriptions indiquent l’emplacement dans le dépôt ; l’implémentation et l’état de validation doivent être vérifiés dans la documentation et les tests propres à chaque paquet.

### A–C

- **Advantage** — `lib/advantage/` et `advantage/` : optimisation d’un dividende unique par rapport à un modèle au moyen d’un Sequitur onion à couches, avec cinq sous-dossiers thématiques : `math/` (maximisation conjointe des profits, négociation de Nash, modèle bilatéral newsvendor), `physics/` (métrique, géodésique, contact et SDE du champ de confiance ; analogie de modélisation explicitement signalée), `integration/` (compartiments d’intégration verticale et score de sophistication), `grading/` (évaluateur de circuits de sophistication A–D pour modules et ensembles) et `output/` (émission vers terminal, CommonRails ou socket). Un consommateur natif C/C++ (`advantage/`) relie le modèle à des exemples de commerce. Voir [`lib/advantage/README.md`](lib/advantage/README.md) et [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet et UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` et `utf-4088/` : catalogues d’alphabets, prise en charge native des jeux de caractères, ordre des cartes d’entrée et documentation d’intégration UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` et `lib/autocad/` : flux de dessin 2D interactifs et composants réutilisables pour les documents et l’édition CAO.
- **Ballum** — contrôle de l’affichage au niveau du pixel : définition/lecture de pixels, copie de régions, luminosité/température/gamma de l’écran entier, entrée pointeur/clavier et **surface framebuffer plein écran** pour un contrôle total, détenu et rapide de chaque pixel (fenêtre override-redirect et présentation par un seul `XPutImage`/MIT-SHM sous Linux/X11). Le noyau natif se trouve dans `ballum/`, la façade SLeeLa dans `lib/ballum/` et le pont VM dans `impl/` ; voir [`ballum/README.md`](ballum/README.md) et l’[exemple exécutable](ballum/example/).
- **CDMA** — `cdma/` et `lib/cdma/` : documentation technique liée au CDMA et développement de bibliothèque source SLeeLa.
- **Character** — `lib/character/` : profils de personnages et statistiques réutilisables, y compris des exemples de profils biographiques.
- **Coorenagraph** — `lib/coorenagraph/` : raisonnement éthique, vocabulaire moral, délibération et concepts associés ; voir le README du paquet et ses fichiers source.

### D–G

- **Modèles de gouvernance en code source** — [`lib/governance`](lib/governance/README.md) définit des structures déclaratives SLeeLa pour les propositions, les bulletins manuels anonymes et les décomptes agrégés. L’annotation proposée `@vote` et le processus manuel sont documentés dans [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md) ; les compilateurs actuels la traitent comme des métadonnées de commentaire inertes.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` jusqu’aux répertoires de paquets HTTP versionnés disponibles : spécifications et implémentations propres à chaque version du protocole. Consultez la documentation de chaque version pour connaître son périmètre précis et son état de vérification.
- **Cour pénale internationale** — `lib/international-criminal-court/` et `INTERNATIONAL.CRIMINAL.COURTS.md` : code source et documentation propres à ce domaine.

### O–S

- **OFDMA** — `ofdma/` : documentation technique sur l’accès multiple par répartition orthogonale de la fréquence et ressources de développement C/C++.
- **Philosophie et module Conversation Mask** — [`philosophy/`](philosophy/README.md) contient les fondations natives C/C++ et [`philosophy/module/`](philosophy/module/README.md) organise la couche suivante du module en trois domaines explicites : [`data/`](philosophy/module/data/README.md), pour les observations typées et datées, les fins d’événements, les paramètres de demi-vie, la provenance et le consentement ; [`helper/`](philosophy/module/helper/README.md), pour la validation déterministe, la décroissance de pertinence, les comparaisons compatibles et la présentation filtrée par consentement ; et [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md), pour une interface conversationnelle de soutien non clinique. Les points d’accès SLeeLa et le Tree composé se trouvent dans [`lib/philosophy/`](lib/philosophy/README.md), aux côtés du paquet de philosophie plus large. Le module repose sur des entrées explicites et le consentement : il ne surveille pas passivement les personnes ou les médias, ne diagnostique pas, ne déduit pas d’intention privée, n’évalue pas la valeur humaine et ne recommande pas de modification de médicaments. Les liens d’événements préservent la provenance et la navigation, mais ne constituent pas une corroboration indépendante.
- **Synchro** — intégration native du runtime et du compilateur, ainsi que fonctions de dispatch de paquets et de mesure ; voir les sources correspondantes dans `impl/` et l’implémentation du compilateur.
- **SleelaUI / interface terminal** — `lib/user-interface/` et le projet `sleela-terminal` : intégration de l’interface SLeeLa et composants d’interaction orientés terminal.

### U–W

- **Nations Unies** — `united-nations/` : ressources du dépôt propres à ce domaine.
- **Wi-Fi 4, Wi-Fi 5 et Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` et les répertoires de sources associés `lib/wifi/` : documentation réseau sans fil et ressources de configuration propres à chaque génération.

### Maintenance de l’index des paquets

Lorsqu’un paquet est ajouté ou considérablement enrichi, ajoutez son entrée ici dans le groupe alphabétique approprié, liez son répertoire et son README canoniques, et ne décrivez que les capacités étayées par les sources présentes dans le dépôt. Conservez le caractère additif de cet index : ne supprimez pas et ne réécrivez pas les sections existantes du README lors des mises à jour de l’index.

---

## XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Démocratie américaine et politique des États-Unis.

---

## XVI. Imagerie de marque de la science nationale

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="Science nationale SLeeLa" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="NSF nationale SLeeLa" width="560"></p>

## XVII. Catalogue expérimental UTF-4088 d’un million d’entrées

Le [générateur de catalogue UTF-4088](utf-4088/tools/generate_catalogue.py) construit de manière déterministe un catalogue expérimental de symboles de 1 048 576 entrées. Le format compact documenté est décrit dans [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md) ; le contrat de génération et de validation se trouve dans [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Exécutez le générateur avec Python 3 :

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Le fichier binaire généré mesure 16 777 244 octets et est conçu pour rester inférieur à 20 000 000 octets. Les tests unitaires et un workflow GitHub Actions vérifient le nombre d’entrées, l’unicité des glyphes, la taille et SHA-256 ; les exécutions réussies publient le binaire et son résumé comme artefact du workflow `utf4088-million-catalogue`. Le binaire généré est volontairement exclu de l’historique Git. UTF-4088 reste expérimental et n’est pas compatible avec Unicode.

## XVIII. Ajouts récents : constante Pi, démarrage Modium/Registry, math de sleelactl et `@native`

Cette section consigne les fonctions ajoutées lors des travaux de 2026-11, en commençant par les groupes les plus récents. Toutes sont intégrées à `master`.

### La constante de catalogue Pi à 100 000 000 chiffres

Le dépôt contient une valeur vérifiée de π à **cent millions de chiffres** dans [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (un nombre décimal bien formé, `3.14159…`), ainsi qu’une valeur à 57 344 chiffres dans [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Les deux sont des valeurs authentiques de π vérifiées indépendamment. Voir [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), qui décrit les contrôles (préfixe de référence, point de Feynman à la position fractionnaire 762, célèbre chiffre à la position un million, concordance avec `lib/math/SLPiReference.sleela`, analyse de fréquence des chiffres sur le fichier entier) et un script de reproduction en une seule exécution.

### Modium™ / Registry™ chargent la constante Pi au démarrage du sous-système

Aux points d’entrée dédiés — `sleela registry` et `sleela modium` (ainsi qu’au premier lancement de l’installateur), et **non** à chaque `sleela run`, les sous-systèmes Modium et Registry chargent la grande constante du catalogue π. Dans un terminal interactif, une ligne rouge transitoire `Loading Modium™` / `Loading Registry™` s’affiche puis est effacée sur place avant le début de la sortie normale. Cet effet est strictement réservé aux TTY ; les sorties redirigées et celles des tests restent identiques octet pour octet. Le localisateur (et non les octets) est initialisé comme valeurs par défaut des clés `Math` du registre `piReferenceLargePath` et `piReferenceLargeDigits`, dans le modèle SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) et dans l’initialiseur natif ([`registry/sleela_registry.c`](registry/sleela_registry.c)) ; `lib/math/SLPiReference.sleela` expose `largePath()` / `largeDigits()`. La constante π est installée à l’emplacement SLeeLa du système d’exploitation (`<data>/pi/`) par l’installateur standard `make` et par l’installateur guidé « beautiful ». Détails : [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — l’installateur « beautiful » et un sujet `math` adossé au natif

- L’installateur guidé (« beautiful ») installe désormais `sleelactl` (le plan de contrôle SLeeLa) et son registre de modules, comme l’installateur `make`. Voir [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` a reçu une nouvelle nature **`subject`** pour les bibliothèques de sujets natifs C/C++ compilées dans le runtime, et enregistre le premier : **`math`**. `sleelactl run math` exécute de bout en bout le backend natif `__NativeMath` (`impl/subjects/math/math_native.cpp`) au moyen de la démonstration de référence du sujet ; `sleelactl info math` explique comment y accéder (`import math;`). Cette nature de sujet ne propose que `run` et `info` : il n’existe volontairement pas de verbe `check`, car `sleela check` ne charge pas les sujets natifs enregistrés par SHEET.

### `@native` et `sleela aot` — première étape de la voie SLeeLa → C/C++ natif

Une nouvelle annotation de méthode **`@native`** demande la compilation native anticipée (*ahead-of-time*) d’une méthode intensive. La nouvelle commande `sleela aot` repère les méthodes `@native`, vérifie que chacune appartient au *sous-ensemble déchargeable* pris en charge et émet du C/C++. Elle **échoue de manière fermée** : toute construction qui ne peut pas encore être traduite est refusée avec une raison précise, afin qu’aucune méthode partiellement traduite ne soit produite.

```sh
sleela aot --list <file.sleela>    # verdict par méthode : ok / REFUSED (motif)
sleela aot --emit <file.sleela>    # émettre du C/C++ (stdout) pour un programme du sous-ensemble
```

Aujourd’hui, `@native` est neutre du point de vue du comportement : les programmes continuent à être vérifiés et exécutés sans changement ; l’annotation ne fait que *demander* un traitement natif. Il s’agit de la Partie 1 : elle établit la surface de l’annotation et la limite explicite du sous-ensemble pris en charge. La compilation du C/C++ émis en objet partagé et le dispatch de la VM vers celui-ci (avec repli sur l’interpréteur) constituent des étapes ultérieures. Détails, sous-ensemble pris en charge et codes de sortie : [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

## XIX. Éditions internationales du README

SLeeLa fournit des points d’entrée README propres à chaque pays pour les 50 marchés de son édition internationale. Chaque pays possède un fichier nommé `README.<CC>.md` ; lorsque plusieurs langues sont pertinentes, elles sont regroupées dans le fichier du pays concerné.

- [README.AE](README.AE.md) · [README.AT](README.AT.md) · [README.AU](README.AU.md) · [README.BE](README.BE.md) · [README.BR](README.BR.md)
- [README.CA](README.CA.md) · [README.CH](README.CH.md) · [README.CL](README.CL.md) · [README.CN](README.CN.md) · [README.CO](README.CO.md)
- [README.CZ](README.CZ.md) · [README.DE](README.DE.md) · [README.DK](README.DK.md) · [README.EE](README.EE.md) · [README.ES](README.ES.md)
- [README.FI](README.FI.md) · [README.FR](README.FR.md) · [README.GB](README.GB.md) · [README.GR](README.GR.md) · [README.HK](README.HK.md)
- [README.HU](README.HU.md) · [README.ID](README.ID.md) · [README.IE](README.IE.md) · [README.IL](README.IL.md) · [README.IN](README.IN.md)
- [README.IS](README.IS.md) · [README.IT](README.IT.md) · [README.JP](README.JP.md) · [README.KR](README.KR.md) · [README.LU](README.LU.md)
- [README.MX](README.MX.md) · [README.MY](README.MY.md) · [README.NL](README.NL.md) · [README.NO](README.NO.md) · [README.NZ](README.NZ.md)
- [README.PH](README.PH.md) · [README.PL](README.PL.md) · [README.PT](README.PT.md) · [README.QA](README.QA.md) · [README.RU](README.RU.md)
- [README.SA](README.SA.md) · [README.SE](README.SE.md) · [README.SG](README.SG.md) · [README.SI](README.SI.md) · [README.SK](README.SK.md)
- [README.TH](README.TH.md) · [README.TR](README.TR.md) · [README.TW](README.TW.md) · [README.US](README.US.md) · [README.ZA](README.ZA.md)

**Politique de traduction :** Ces fichiers constituent des points d’entrée localisés initiaux par pays. Leurs introductions et intitulés de langue sont localisés, tandis que la référence technique complète reste le [README.md canonique en anglais](README.md). Un fichier pays ne doit pas être présenté comme une traduction complète tant que toutes ses sections n’ont pas été traduites, que les identifiants techniques et les liens n’ont pas été vérifiés et que l’édition n’a pas fait l’objet d’une révision linguistique. La couverture linguistique de cette première phase est représentative ; elle ne prétend pas que toutes les langues reconnues par les constitutions, régionales, autochtones ou des signes ont été entièrement traduites.

**État de traduction :** les sections I à XIX sont traduites en français. Les identifiants techniques, commandes, exemples de code, noms de fichiers, liens et adresses ont été conservés. Une révision linguistique finale par des locuteurs natifs est recommandée.
