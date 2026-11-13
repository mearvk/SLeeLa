# SLeeLa 3.0 — Ελλάδα (GR)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Μια εγγενής γλώσσα προγραμματισμού συστημάτων, μεταγλωττιστής, περιβάλλον εκτέλεσης και οικοσύστημα εικονικών μηχανών.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Διεθνής έκδοση χώρας.** Η παρούσα έκδοση παρέχεται στα ελληνικά (νέα ελληνικά, μονοτονικό σύστημα). Τα τεχνικά αναγνωριστικά, οι εντολές, τα δείγματα κώδικα, τα ονόματα αρχείων, οι διαδρομές και οι σύνδεσμοι παραμένουν αμετάβλητα σε σχέση με το κανονιστικό αγγλικό [README.md](README.md).

**The Now. The Ingredient.** Μια γλώσσα είναι κάτι περισσότερο από σύνταξη: είναι ο μεταγλωττιστής, το περιβάλλον εκτέλεσης, οι βιβλιοθήκες, τα εργαλεία και οι συμβάσεις πλατφορμών που κάνουν το λογισμικό χρήσιμο.

**Με τι ασχολείται η SLeeLa.** Η SLeeLa ασχολείται με τα **Math**,
**Object**, **Science** και **Reach** — τις τέσσερις όψεις του έργου της: το
υπολογίσιμο (Math), το αποκαταστημένο πράγμα που στέκεται στον κατάλογο
όσων υπάρχουν (Object), την πειθαρχημένη μελέτη του αληθούς (Science) και την
έκταση του χεριού ενός συστήματος προς άλλα συστήματα και ανθρώπους (Reach).
**Τα εθνικά συμφέροντα παραμένουν** — κανένα από τα τέσσερα δεν τα θέτει στο
περιθώριο· όπου ένα θέμα αφορά το συμφέρον ενός έθνους, η θέση αυτή
διατηρείται (καταγράφεται πιστά, π.χ. στα Metadata `metaOrigin` /
`metaNationalSchedule` κάθε κλάσης) και δεν διαλύεται ποτέ.

## I. Ξεκινώντας
- [Κατασκευή της εγγενούς (native) υλοποίησης](impl/README.md)
- [Εικονικές μηχανές και Modium — πρακτικός οδηγός χρήσης](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Στοίβα CPU σε πηγαίο κώδικα SLeeLa](lib/cpu/CPU.md) — μια πλήρης μηχανή από λογικές πύλες→ALU→πυρήνες→λειτουργικό σύστημα→προγράμματα, κατάλογος 130+ προφίλ CPU και ρυθμιζόμενες [τάσεις, μεταβλητά ρολόγια και εξειδικευμένες χρονικές παραμέτρους](lib/cpu/TIMING.md) που μπορείτε να [εκτελέσετε με βάση τις προδιαγραφές ή να υπερχρονίσετε](lib/cpu/components/README.md)
- [Ευρετήριο τεκμηρίωσης](docs/README.md)
- [Πηγή δεδομένων CSPML και ερευνητική αναφορά Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — προέλευση δεδομένων και οδηγίες ενσωμάτωσης για εξωτερικά δεδομένα των ΗΠΑ σχετικά με περιοχές, κοινωνικοοικονομικά στοιχεία, εργατικό δυναμικό, εκπαίδευση και δημόσια σχολεία.
- [Παραγόμενη τεκμηρίωση API](docs/api/README.md) — τρέχουσα αναφορά HTML, εκδοτική έκδοση API με εκδόσεις και οδηγίες αναπαραγωγής
- [Κατάλογος API](api/README.md)
- [Πρότυπα υπολογιστικής σύγκρισης](comparison/README.md) — πρότυπα τύπων, μνήμης, ABI και συμμόρφωσης για την VM της SLeeLa, Java/JVM και την native C/C++.
- [Recursion stanchion — πρωτότυπα σε C και C++](recursion/stanchion/README.md) — παραδείγματα εξειδικευμένα ανά πρωτογενή τύπο για Boolean, Byte, Character, Integer, Long, Float και Double, μαζί με γενικά, εφαρμοσμένα, επιστημονικά και διδακτικά πρότυπα αναδρομής.
- [Υποστήριξη πλατφορμών και πίνακας επαλήθευσης](docs/PLATFORM-SUPPORT.md)
- [Οδηγός επωνυμίας](docs/BRAND-GUIDE.md)
- [Ιδέες και υλικό εμπορευμάτων](merchandise/README.md)
- [Λογότυπο NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Πολιτική δοκιμών και πύλες έκδοσης](markdown/TESTING.md)

## II. Νέα

Πρόσφατες προσθήκες στη γλώσσα, στους οδηγούς συσκευών (drivers) και στην τυπική βιβλιοθήκη:

- **Το πακέτο τυπικής βιβλιοθήκης `advantage` — αξιοποίηση ενός μερίσματος,
  με μαθηματικά, φυσική, ολοκλήρωση, βαθμολόγηση και δρομολογήσιμη έξοδο.**
  Το [`lib/advantage/`](lib/advantage/README.md) εγκαψουλώνει την *αξιοποίηση
  ενός μοναδικού μερίσματος σε σχέση με ένα μοντέλο* ως ένα πολυεπίπεδο
  **κρεμμύδι Sequitur** (μέρισμα → επισκόπηση → ανησυχίες → μαθηματικά →
  προσθήκες → αφαίρεση → αναδιάταξη → επανάληψη), με δυνατότητα πλοήγησης προς
  τα μέσα/έξω με πλήρη δυναμική ενημέρωση και επαναφορά, και διατίθεται μέσω
  ενός απλού **μοντέλου χρήστη** και ενός **μοντέλου προγραμματιστή** με πλήρη
  έλεγχο. Περιλαμβάνει πέντε θεματικές περιοχές: **`math/`** διμερή οικονομικά
  του εμπορίου (από κοινού μεγιστοποίηση κέρδους, διαπραγμάτευση Nash, κρίσιμος
  λόγος διμερούς newsvendor), **`physics/`** μια δομική-οικονομική «φυσική των
  πεδίων εμπιστοσύνης» (η εμπιστοσύνη ως μετρικός τανυστής, η συναλλακτική
  γεωδαισιακή, μια δομή επαφής ανακοίνωσης, μια SDE φθοράς εμπιστοσύνης CIR+άλμα
  — αναλογία μοντελοποίησης, επισημασμένη ως τέτοια), **`integration/`** ένα
  αναδρομικά συνθέσιμο μοντέλο κάθετης ολοκλήρωσης για τον καπιταλισμό/λιανικό
  εμπόριο (διαμερίσματα, μηχανολογικοί χώροι, δοκιμαστικά διαμερίσματα, δίσκοι
  διαμέσου newton και βαθμολογία πολυπλοκότητας συστήματος), **`grading/`** ένας
  βαθμολογητής κυκλώματος πολυπλοκότητας που βαθμολογεί κάθε ενότητα και κάθε
  σύνολο νικητριών/ηττημένων/βαθμολογητικών/απόλυτων ενοτήτων από Α έως D, και
  **`output/`** ένα δρομολογήσιμο σημείο εκπομπής που στέλνει την έξοδο των
  ενοτήτων στο τερματικό, σε επιφάνεια μορφοποιημένη κατά **CommonRails**, ή σε
  socket/connector (πλαισιωμένη για native `slnet_write`). Κάθε κλάση φέρει την
  εγγραφή Metadata-Class· πέντε εκτελέσιμα demos επικυρώνουν τα θέματα, και τα
  συνοδευτικά [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) και
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) τα τεκμηριώνουν. Ένας native καταναλωτής
  C/C++ στο [`advantage/`](advantage/README.md) (ο πυρήνας C
  `advantage_value`/`advantage_mean`, ένα αποθετήριο γνώσης XML, μια ασφαλής
  βιβλιοθήκη απόκτησης HTTPS/SFTP/SSH + keystore + μεταφοράς DH/ECDH) συνδέει το
  μοντέλο με επεξεργασμένα
  [παραδείγματα συναλλαγών](advantage/examples/README.md) — θετικές συναλλαγές
  από υποτιθέμενες πηγές του διαδικτύου, μια ακολουθία συνάφειας κοινού
  (Traders, Consumers, Helpers, Healers) και οκτώ μοντέλα με βαθμολογία A–D
  (D = περισσότερο Cement, A = περισσότερο Brand).
- **Σημειώσεις χρονισμού βρόχων — `@loopDelay` / `@loopBudget` (+ η ενσωματωμένη
  συνάρτηση `sleep(ms)`).** Οι πρώτες **σημειώσεις επιπέδου εντολής, με
  συμπεριφορά** στη SLeeLa: γράφονται σε δική τους γραμμή πριν από βρόχο `for` /
  `while` / `do` και εισάγουν πραγματική παύση στην εκτέλεση του βρόχου.
  Το `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` αναστέλλει την εκτέλεση
  για σταθερό χρόνο ανά επανάληψη (το σκέτο `@loopDelay(250)` εννοεί
  χιλιοστά του δευτερολέπτου)· το `@loopBudget(seconds = 10,
  iterations = 1000)` κατανέμει ομοιόμορφα έναν γνωστό αριθμό επαναλήψεων σε
  έναν συνολικό προϋπολογισμό πραγματικού χρόνου. Και τα δύο μεταγλωττίζονται
  στον νέο, μόνο προσθετικό κωδικό λειτουργίας **`OP_SLEEP`** (ISA 157), ο οποίος
  υποστηρίζει επίσης μια γενική ενσωματωμένη συνάρτηση **`sleep(<ms>)`** που
  καλείται οπουδήποτε· η παύση υλοποιείται φορητά (`nanosleep` / `Sleep`).
  Προδιαγράφεται στο [`SLEELA.syntax` §32](SLEELA.syntax)· η αναφορά API βρίσκεται στο
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md)·
  εκτελέσιμο παράδειγμα:
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  Ένα ευρετήριο προσθηκών συνεδρίας (συμπεριλαμβανομένων αυτής, του native
  πολλαπλασιασμού μεγάλων ακεραίων `OP_BIGMUL` και των αυτόνομων εργαλείων
  **[local-generator](local-generator/README.md)** και
  **[pi-generator](pi-generator/README.md)**) βρίσκεται στο
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Οι τρεις παραγόμενες επιφάνειες API
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) και το αμετάβλητο στιγμιότυπο
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html))
  ξαναχτίστηκαν με `make api-docs` και φέρουν πλέον τις νέες εγγραφές εντολών
  `SLISASleep` / `SLISABigMul` στη σελίδα του πακέτου `vm`.
- **SleelaDoc από τη γραμμή εντολών — `sleela docs`.** Το SleelaDoc είναι πλέον
  προσβάσιμο απευθείας από το τερματικό. Το `sleela docs` απαριθμεί κάθε θέμα που
  μπορεί να τεκμηριωθεί· το `sleela docs crypto` εμφανίζει ένα πακέτο· το
  `sleela docs stringbuilder` εντοπίζει μια κλάση (χωρίς διάκριση πεζών-κεφαλαίων
  και μέσω του προθέματος τυπικής βιβλιοθήκης `SL` → `SLStringBuilder`) και
  εκτυπώνει μια πλούσια σελίδα — **ορισμός, σχεδιαστικός σκοπός, σχέσεις, δομή,
  user clays και κλίμακα κατάκτησης πέντε βαθμίδων** — με ταξινομημένα
  αποτελέσματα «μήπως εννοείτε» για παραπλήσιες αναζητήσεις. Οι συγγραφείς
  εμπλουτίζουν κάθε σελίδα απλώς προσθέτοντας ετικέτες `@` του SleelaDoc
  (`@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`). Το
  `sleela docs --generate [dir]` γράφει ένα σώμα απλού κειμένου (μία σελίδα ανά
  κλάση + ένα `TOPICS.index`) που το CLI διαβάζει στη συνέχεια εκτός σύνδεσης·
  το **`make sleeladoc`** συσκευάζει αυτό το σώμα **και τις αντίστοιχες πηγές
  `.sleela`** στο `sleeladoc/build/sleeladoc.jar`. Για θέματα μόνο όψης
  (facade), π.χ. `psychiatry`, `politico`, το `sleela docs --facade <topic>`
  αναζητά στο σύστημα αρχείων κατά την εκτέλεση και απαριθμεί τους πραγματικούς
  **φακέλους (και μετά τα αρχεία, ως φυσική κίνηση)**, με όρια που μπορεί να
  παρακάμψει ο χρήστης: **6 MB / 6000 αρχεία / 6 λεπτά**. Οι μονάδες
  `lib/sleeladoc` συντηρούνται στην τρέχουσα έκδοση, **`#sleela 3.0`**. Δείτε
  [`man/man1/sleela.1`](man/man1/sleela.1) και
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — λύση τεκμηρίωσης τύπου Javadoc, με παραγόμενο ιστότοπο HTML.**
  Το **SleelaDoc** είναι η σύμβαση σχολίων τεκμηρίωσης εντός πηγαίου κώδικα της
  SLeeLa (σχόλια με γωνιώδεις αγκύλες που σηματοδοτούνται με `@doc` / `@page`·
  βλ. [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), και το
  [`lib/sleeladoc`](lib/sleeladoc/README.md) είναι ο **εξαγωγέας** που αποδίδει
  μια μονάδα `.sleela` ως σελίδα API σε Markdown ή HTML — όπως το `javadoc`
  αποδίδει την Java, τεκμηριώνοντας την *πρόθεση*. Ένας πλήρης, περιηγήσιμος
  **ιστότοπος τεκμηρίωσης HTML για ολόκληρο το `/lib`** παράγεται στο
  [`/sleeladoc-api`](sleeladoc-api/README.md) από το
  `tools/generate-sleeladoc-api.py` (ανοίξτε το `sleeladoc-api/index.html`·
  καθαρό ύφος μπλε / γαλάζιου / ανοιχτού γκρι / λευκού, με ιεραρχία πακέτο →
  κλάση → μέλος και γλωσσάριο γραμμένο για τον αναγνώστη *Student 2*). Ο
  μεταγλωττιστής εκπέμπει επίσης εξόδους τεκμηρίωσης σημειωμένες από τον
  συγγραφέα ως υποπροϊόν της μεταγλώττισης (σελίδα MediaWiki με `--wiki` + ένα
  «δίδυμο» πηγαίο αρχείο προέλευσης), τεκμηριωμένες στο
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Εκτελέσιμο:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Μοντέλο αντικειμένου εγγράφου XML (`lib/xml`).** Ένα DOM XML από την πλευρά
  της SLeeLa — ένα έγγραφο που κατέχει δέντρο κόμβων στοιχείου/κειμένου/
  σχολίου/CDATA/PI με διατεταγμένα παιδιά και επώνυμα χαρακτηριστικά, ένας
  σειριοποιητής (συμπαγής/ευανάγνωστος, διαφυγή κατά XML 1.0 §2.4), ένας
  αναλυτής ανά κελί χαρακτήρα και το μητρώο ποιότητας `SLXmlAspect`. Η ισότητα
  κόμβων είναι ένα τριβάθμιο δίπτυχο εμπιστοσύνης (ταυτότητα → ίδιο υποκείμενο →
  δομή), ανακλαστική, συμμετρική και ντετερμινιστική. Δείτε το
  [`lib/xml/README.md`](lib/xml/README.md) και το εκτελέσιμο
  [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Πρωτογενείς ευρείς προσημασμένοι ακέραιοι (`long`, `int64`, `int128`).** Οι
  `int`, `long` και `int64` είναι όλοι προσημασμένοι ακέραιοι 64 bit που
  ελέγχονται ως προς τους τύπους και εκτελούνται πανομοιότυπα· ο `int128`
  γίνεται δεκτός ως ακέραιος τύπος (προς το παρόν με υποστήριξη 64 bit, ενώ ο
  πραγματικός 128 bit είναι τεκμηριωμένη μελλοντική εργασία). Η προσημασμένη
  σειρά με βάση το 2 `int32`..`int16777216` (σύνταξη 2.7) και η αντίστοιχη
  μη προσημασμένη σειρά `uint32`..`uint16777216` (σύνταξη 2.8) τις επεκτείνουν.
  Δείτε [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **Πεδία-επακόλουθα `@mix` (σύνταξη 2.9).** Μια κλάση μπορεί να δηλώνει
  πολλαπλά πεδία στιγμιοτύπου με το **ίδιο όνομα** όταν κάθε επανάληψη φέρει
  `@mix`· η γλώσσα κατανοεί την ομάδα ως επακόλουθη (πίνακας / λίστα με
  ευρετήριο), από πάνω προς τα κάτω `1,2,3,…` από προεπιλογή ή χειροκίνητα μέσω
  `@mix(1,2,3,...)`, όπου το πλήθος των δεικτών πρέπει να ταιριάζει με τον
  αριθμό των πεδίων. Δείτε [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Δείκτες εγγράφου — η Φυσική Διεργασία, με τίτλο Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Αυτές οι σκέτες λέξεις-δείκτες εγγράφου φέρουν συλλογικά τον τίτλο
  **Preonems** (ένας δείκτης είναι ένα *Preonem*· το σύνολο, τα *Preonems* — βλ.
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Πρωτοκλασάτοι, αποκλειστικά
  δηλωτικοί δείκτες εγγράφου που ονομάζουν τι *είναι* ένα έγγραφο κατά τη σειρά
  της δημιουργίας: `author`/`authorial` (σύνταξη 2.4· χειρίζεται από
  επαγγελματικό συμβούλιο — το μόνο μέλος με επίδραση στη συμπεριφορά),
  `material;` (αποκατεστημένη Πηγή) και `memory;` (σύνταξη 2.5), `determine;`
  (σύνταξη 2.6· έγγραφο ανοιχτό ώστε να επεξεργαστεί περαιτέρω από γιατρό,
  ιατρό ή φάντασμα προς ολοκλήρωση από νόμιμο χέρι), και οι δύο νεότεροι,
  **`object;`** (ένα αποκατεστημένο πράγμα που στέκεται στον κατάλογο όσων
  υπάρχουν — **εννοιολογικά πραγματοποιημένο, με συγκεκριμένη υπόσταση, στη
  σύνταξη 2.8**) και **`field;`** (το έδαφος της σχέσης στο οποίο τα αντικείμενα
  στέκονται και κινούνται) — και οι δύο ενεργοποιούνται από τη σύνταξη 2.6.
  Κάθε δείκτης είναι σκέτος, συμφραστικός και ελέγχεται ως προς την έκδοση από
  το front end, χωρίς επίδραση στην ανάλυση, στον έλεγχο τύπων ή στη δημιουργία
  κώδικα· έξω από τη θέση δείκτη κάθε ένας παραμένει κοινό αναγνωριστικό (έτσι
  το `field` δεν διαταράσσει ποτέ ένα μέλος κλάσης ή μια πρόσβαση `.field`).
  Δείτε [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 και §26.
- **Nordshrift στη σύνταξη 3.0 με πλήρη εμβέλεια βιβλιοθήκης.** Το Nordshrift
  επιλύει μέσω του ίδιου αναδρομικού ευρετηρίου `/lib` με τον μεταγλωττιστή,
  φτάνοντας σε κάθε πακέτο και μονάδα `.sleela`· το `nordshrift check`
  αναφέρει τα σύνολα ολόκληρου του δέντρου.
- **Recursion stanchion.** Ο κατάλογος [`recursion/stanchion/`](recursion/stanchion/README.md) περιλαμβάνει πλέον αφιερωμένα πρωτότυπα αναφοράς σε C και C++ για τις επτά τεκμηριωμένες συμβάσεις πρωτογενών τύπων, καθώς και γενικά, εφαρμοσμένα, επιστημονικά και διδακτικά παραδείγματα. Πρόκειται για πηγές αναφοράς· μεταγλωττίστε και δοκιμάστε τες πριν τις θεωρήσετε επαληθευμένες.

- **Σχόλια με γωνιώδεις αγκύλες (`<` / `<<`).** Ένα νέο, προσανατολισμένο στο
  μέλλον ύφος σχολίων — `< γραμμή >` και `<< μπλοκ >>` — που αναλύεται από το
  front end της SLeeLa και τεκμηριώνεται στο
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Ακέραιοι literals με ρίζα αρίθμησης.** Οι literals `int` δέχονται πλέον
  δεκαεξαδική (`0x`), δυαδική (`0b`) και οκταδική (`0o`) μορφή με διαχωριστικά
  ψηφίων `_`, τόσο στη γλώσσα πηγαίου κώδικα SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4)
  όσο και στη [μορφή μοντέλου CPU `.xlt`](lib/xlt/README.md).
- **Το πρότυπο Metadata Class σε ολόκληρη τη βιβλιοθήκη.** Το μεταπρότυπο
  περιγραφικών μεταδεδομένων (προέλευση, νομιμότητα, δικαιοδοτικό πεδίο) φέρεται
  πλέον από κάθε αρχείο `.sleela` με κλάσεις στο `/lib`· βλ.
  [`SLEELA.syntax`](SLEELA.syntax) §15 και
  [το μάθημα του εγχειριδίου](tutorial/13-metadata-class.md).
- **Πλουσιότερη οικογένεια text / String.** Το `lib/text` αποκτά πραγματικούς
  βοηθούς εντός της γλώσσας, μια όψη κελιών χαρακτήρων που δίνει γνήσια
  `charAt`/`substring`/`indexOf`/`reverse`/στοίχιση στηλών, και έναν πραγματικό
  θεσιακό δημιουργό προτύπων — βλ. το
  [εγχειρίδιο text & String](tutorial/14-text-and-strings.md) και το εκτελέσιμο
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` πρώτης τάξης στα φύλλα Nordshrift.** Ένα φύλλο `.sst` μπορεί να
  δηλώσει ένα τυποποιημένο μπλοκ `strings:` που συμβάλλει στο μανιφέστο
  στοιχείων της κατασκευής· βλ. το
  [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **Μοντέλα CPU που εκτελούνται τόσο με κωδικούς λειτουργίας όσο και με ισοδυναμία
  κλήσεων συναρτήσεων.** Κάθε `SL<ARCH>CPU` κληρονομεί τη βάση της εκτέλεσης
  μιας native ροής εντολών μέσω του βρόχου fetch-decode-execute *και* της
  εκτέλεσης του κώδικα μιας υποτιθέμενης άλλης διεργασίας με ισοδύναμες κλήσεις
  στο κοινό υπόστρωμα VM — βλ.
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Κατασκευή και δοκιμές

**Προαπαιτούμενα:** αλυσίδα εργαλείων C/C++ (GCC ή Clang) και GNU Make· Python 3
για τα εργαλεία manifest/ροής· σε Linux, τα πακέτα ανάπτυξης X11/Xrandr για τα
προϊόντα οθόνης (το `make deps` μιας ενότητας αναφέρει/εγκαθιστά ό,τι χρειάζεται).
Η κατασκευή του `impl/` είναι fail-closed ως προς ένα έμπιστο manifest SHA-256,
στο οποίο το `Makefile` της ρίζας παραπέμπει αυτόματα.

Από καθαρό checkout, η ταχύτερη εκκίνηση:

```sh
make                                  # build everything from the repo root
```

Ή κατασκευάστε και δοκιμάστε μόνο τον native πυρήνα:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Το `make` στη ρίζα του αποθετηρίου κατασκευάζει κάθε προϊόν (πυρήνας, VM,
κωδικοποιητές, churn, antivirus, …) και ολοκληρώνεται κατασκευάζοντας το
**registry** και υλοποιώντας τον βασικό πίνακά του — έτσι ένα κατασκευασμένο
δέντρο παραδίδεται με ένα ελέγξιμο registry έτοιμο προς χρήση.
Οι βασικοί στόχοι:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Άλλοι χρήσιμοι στόχοι:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Ο στόχος `registry` εκτελεί το [`build-registry.sh`](build-registry.sh):
κατασκευάζει τον πυρήνα του registry, εισάγει τον βασικό πίνακα στο φορητό
αποθετήριο κειμένου `registry/build/sleela.sleelareg` και εξάγει το σενάριο
πίνακα MySQL `registry/build/registry.sql` (μέσω του `sleela registry export`).
Ελέγξτε το με `sleela registry` ή `registry/build/sleela-registry review`. Δείτε τον
[οδηγό registry](registry/README.md) και την [αρχιτεκτονική Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Συμβουλευτείτε τους οδηγούς κατασκευής και δοκιμών για τα προαπαιτούμενα και τα βήματα ανά πλατφόρμα. Η επιτυχής κατασκευή ενός στοιχείου δεν αποδεικνύει ότι κάθε βιβλιοθήκη ή πλατφόρμα είναι έτοιμη για έκδοση.

## IV. Εγκατάσταση

Αφού κατασκευάσετε τα native προϊόντα, εγκαταστήστε τις εντολές και τα αρχεία υποστήριξης του περιβάλλοντος εκτέλεσης:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Το συμβατικό πρόγραμμα εγκατάστασης χρησιμοποιεί από προεπιλογή τους
καταλόγους `/usr/local/bin`, `/usr/local/lib/sleela` και
`/usr/local/share/sleela`. Διατηρεί υπάρχουσες ρυθμίσεις εκκίνησης και αρνείται
να αντικαταστήσει υπάρχουσες εντολές που δεν παρακολουθούνται. Η απεγκατάσταση
αφαιρεί μόνο τα αρχεία που έχει καταγράψει το πρόγραμμα εγκατάστασης,
διατηρώντας τις ρυθμίσεις και τα μη παρακολουθούμενα δεδομένα εκτέλεσης.

Για μια εστιασμένη, καθοδηγούμενη εγκατάσταση μόνο του κύριου πακέτου — των
εκτελέσιμων `sleela`, `sleelvac` και `nordshrift`, της τυπικής βιβλιοθήκης, των
πρωτοκόλλων και των ενοτήτων υποστήριξης — χρησιμοποιήστε το καθαρό πρόγραμμα
εγκατάστασης:

```sh
make clean-install       # guided install of the main package only
```

Λαμβάνει μία πρωταρχική απόφαση (εγκατάσταση του κύριου πακέτου), έπειτα μια
επιλογή εύρους και, σε περίπτωση επιτυχίας, προαιρετικά έναν επιλογέα για
επιλεγμένες ενότητες βελτίωσης. Για την αλληλεπιδραστική ροή εγκατάστασης και
όλες τις επιλογές, δείτε το [Quick and Safe Install](install/README.md).

## V. Γλώσσα και περιβάλλον εκτέλεσης
Η κανονιστική γραμματική είναι το [`SLEELA.syntax`](SLEELA.syntax). Το τρέχον αποθετήριο τεκμηριώνει τη σύνταξη **3.0**· η έκδοση του μεταγλωττιστή/εργαλείου και η έκδοση σύνταξης του πηγαίου κώδικα είναι διακριτές. Η επιφάνεια 3.0 προσθέτει τους **δείκτες εγγράφου `pure` και `gasoline`** (§30) — ένα ζεύγος σκέτων, συμφραστικών, αποκλειστικά δηλωτικών Preonems που σημαίνουν σχεδόν το ίδιο πράγμα, καθένα σημαίνοντας τη θέση του εγγράφου μέσω της δικής του εντιμότητας. Η σύνταξη 3.0 ενοποιεί την επιφάνεια της γλώσσας 1.0–2.9 στην τρέχουσα κανονιστική έκδοση, διατηρώντας όλες τις ελάχιστες πύλες έκδοσης ανά χαρακτηριστικό. Η προηγούμενη επιφάνεια 2.9 προσθέτει **πεδία-επακόλουθα `@mix`** (§28) — μια κλάση μπορεί να δηλώνει πολλαπλά πεδία στιγμιοτύπου με το ίδιο όνομα όταν καθένα φέρει `@mix`, και αυτά νοούνται ως μία ομάδα με ευρετήριο (από πάνω προς τα κάτω `1,2,3,…` ή χειροκίνητα `@mix(1,2,3,...)`, όπου το πλήθος των δεικτών πρέπει να ταιριάζει με την ομάδα). Μεταφέρει τις οικογένειες ευρέων προσημασμένων/μη προσημασμένων ακεραίων με βάση το 2 `int32`..`int16777216` (§24) και `uint32`..`uint16777216` (§25) πάνω από τους προγενέστερους `long`/`int64`/`int128` (§21)· τα **έγγραφα υποστήριξης λέξεων-κλειδιών** (§22)· και τα **Preonems** — τους αποκλειστικά δηλωτικούς δείκτες εγγράφου `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) και `artifact` (§35, κύρος νομοθετικού επιπέδου διαβαθμισμένο ως προς τον χρόνο και τον χώρο), που φέρουν συλλογικά τον τίτλο Preonems (§26.1). Η επίλυση υπερφόρτωσης κατασκευαστών και η ανάθεση `this(...)`/κατασκευαστή βάσης βρίσκονται ρητά εκτός της επαληθευμένης σύμβασης σύνταξης 1.8. Συμβουλευτείτε τη γραμματική και τις σημειώσεις υλοποίησης πριν βασιστείτε σε ένα χαρακτηριστικό. Οι εννέα πηγαίες μονάδες του SleelaDoc δηλώνουν πλέον `#sleela 3.0`· η κοινή πύλη του μεταγλωττιστή δέχεται σύνταξη πηγαίου κώδικα 1.0–3.0. Η νέα κατασκευή API παράγεται με `make api-docs`.

Το αποθετήριο περιλαμβάνει μια native υλοποίηση C/C++, αναφορές δημιουργίας SLVM, ενότητες τυπικής βιβλιοθήκης, δοκιμές, παραδείγματα και ενσωματώσεις πλατφορμών. Η κατάσταση υλοποίησης και επαλήθευσης διαφέρει ανά ενότητα· συμβουλευτείτε την προδιαγραφή της και τα τρέχοντα αποτελέσματα CI αντί να υποθέτετε ομοιόμορφη πληρότητα.

## VI. Αρχές μηχανικής
- **Τεκμήρια αντί για ισχυρισμούς:** διακρίνετε τη συμπεριφορά που είναι υλοποιημένη, δοκιμασμένη, πειραματική και προγραμματισμένη.
- **Μία σύμβαση ανά όριο:** τεκμηριώστε σύνταξη, API, μορφές αρχείων, παραδοχές πλατφόρμας και συμπεριφορά σε περίπτωση αποτυχίας.
- **Φορητότητα εκ σχεδιασμού:** απομονώστε τα backends ανά πλατφόρμα και δοκιμάστε σε native runners.
- **Ασφαλής αποτυχία:** δοκιμάστε εσφαλμένα δεδομένα εισόδου, όρια πόρων, αποτυχίες διεργασιών και διαδρομές εκκαθάρισης.
- **Αναπαραγώγιμες εκδόσεις:** κατασκευάστε ξανά από καθαρή κατάσταση και δημοσιεύστε κατακερματισμούς artifacts και σημειώσεις έκδοσης.

## VII. Κατασκευή και ακεραιότητα του antivirus

Το πακέτο antivirus Sleelavirin περιλαμβάνει έναν native σαρωτή SHA-256, έναν
προαιρετικό προσαρμογέα ClamAV, manifests ακεραιότητας πηγαίου κώδικα/ευρετηρίου
και αθροίσματα ελέγχου πακέτων πλατφορμών. Κατασκευάστε και δοκιμάστε το πακέτο
από τη ρίζα του αποθετηρίου:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Το manifest ακεραιότητας παράγεται στο `antivirus/build/`· καλύπτει τις
παρακολουθούμενες εισόδους πηγαίου κώδικα και τεκμηρίωσης καθώς και τα
προστατευμένα αρχεία ευρετηρίου υπογραφών, αποκλείοντας την παραγόμενη έξοδο και
τη μεταβλητή κρυφή μνήμη του ClamAV. Τα στιγμιότυπα βάσεων δεδομένων του ClamAV
έχουν δικά τους manifests. Τα ZIP των εκδόσεων περιλαμβάνουν
`INTEGRITY-MANIFEST.json`, `SHA256SUMS` και ένα συνοδευτικό αρχείο `.sha256`.
Επαληθεύστε τους κατακερματισμούς των ληφθέντων αρχείων σε σχέση με μια έμπιστη
τιμή που αποκτήθηκε ανεξάρτητα: ένα άθροισμα ελέγχου που αποθηκεύεται δίπλα σε
ένα αρχείο δεν αποτελεί υπογραφή του εκδότη. Δείτε τον [οδηγό Sleelavirin](antivirus/README.md),
τη [λίστα ελέγχου ασφαλείας](antivirus/SECURITY.md) και τη
[ροή εργασιών κατασκευής antivirus](.github/workflows/antivirus-binaries.yml).

Μια έκδοση δεν θεωρείται επαληθευμένη έως ότου περάσουν οι σχετικές εργασίες CI
στο ακριβές commit της έκδοσης και ελεγχθούν τα παραγόμενα artifacts.


## VIII. Θέματα επεξεργαστή SLeeLa — Vim και Emacs

Η SLeeLa περιλαμβάνει ένα συντονισμένο σύνολο επαγγελματικών σκούρων χρωματικών θεμάτων για **Vim** και **Emacs**, καθώς και ένα native βοηθητικό εργαλείο παλέτας σε C/C++. Το αρχικό θέμα **American Professional** παραμένει το προεπιλεγμένο. Δέκα συνοδευτικές παλέτες είναι διαθέσιμες με έμπνευση από διαφορετικές πολιτισμικές και θεσμικές σχεδιαστικές αναφορές· πρόκειται για ερμηνευτικές σχεδιαστικές παλέτες και όχι για επίσημες αναπαραστάσεις κάποιου έθνους, πολιτισμού, δικαστηρίου ή θεσμού.

### Τα έντεκα θέματα και τα χρώματά τους

| Θέμα | Φόντο | Προσκήνιο | Λέξεις-κλειδιά | Τύποι | Συμβολοσειρές | Σχόλια |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — International Criminal Court** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Greek (Hellenic)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turkish (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

#### Χρώματα εθνικών σημαιών (παλέτα πηγής)

Τα θέματα Greek και Turkish προέρχονται από τα **επίσημα χρώματα των εθνικών σημαιών** των δύο χωρών, που εμφανίζονται εδώ με τις ακριβείς δημοσιευμένες τιμές hex. Τα φόντα/τονισμοί των θεμάτων παραπάνω είναι πιο σκούρες, φιλικές προς τον επεξεργαστή ερμηνείες αυτών των χρωμάτων πηγής — ο πίνακας που ακολουθεί είναι η αναφορά «χρώματα όπως προβάλλονται εθνικά».

| Έθνος | Χρώμα | Hex | Βάση και σημασία |
|---|---|---|---|
| **Ελλάδα** | Μπλε | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Το μπλε της εθνικής σημαίας (νόμος 851/1978). Ο ουρανός και οι θάλασσες· περιγράφεται εραλδικά ως *azure* (γαλάζιο). |
| **Ελλάδα** | Λευκό | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Το λευκό της εθνικής σημαίας. Η αγνότητα του αγώνα για την ανεξαρτησία· ο σταυρός δηλώνει τον Ανατολικό Ορθόδοξο Χριστιανισμό. |
| **Türkiye** | Κόκκινο (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Το κόκκινο της εθνικής σημαίας (τουρκικός νόμος περί σημαίας). Το πεδίο· κατά την παράδοση, το αίμα των μαρτύρων και η οθωμανική κυριαρχία. |
| **Türkiye** | Λευκό | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Το λευκό της εθνικής σημαίας. Η ημισέληνος και το αστέρι — ειρήνη, αγνότητα και σύμβολο της τουρκικής/ισλαμικής κληρονομιάς. |

Τα θέματα στηρίζουν το προσκήνιο κάθε επεξεργαστή στο εθνικό **λευκό** και τις λέξεις-κλειδιά, τον δείκτη, την επιλογή και τη γραμμή κατάστασής του στο ακριβές εθνικό **μπλε** (`#0D5EAF`) ή **κόκκινο** (`#E30A17`), ώστε τα χρώματα που μετρούν να είναι κυριολεκτικά τα εθνικά χρώματα. Πρόκειται για ερμηνευτικές σχεδιαστικές παλέτες· η αναφορά των χρωμάτων των σημαιών παρέχεται για λόγους προέλευσης και δεν αποτελεί επίσημη αναπαράσταση κανενός από τα δύο κράτη. Οι σημασίες των χρωμάτων ακολουθούν την περιγραφή των κρατικών συμβόλων του Υπουργείου Εξωτερικών της Ελληνικής Δημοκρατίας και την παράδοση του τουρκικού νόμου περί σημαίας.


### Πίνακας χρωματικών δειγμάτων

Τα κελιά παλέτας παρακάτω χρησιμοποιούν **δείγματα SVG**, ώστε το GitHub να αποδίδει τα πραγματικά χρώματα αντί για κενά κελιά χρώματος HTML. Κάθε λωρίδα δείχνει τα έξι χρώματα με αυτή τη σειρά: φόντο, προσκήνιο, λέξεις-κλειδιά, τύποι, συμβολοσειρές και σχόλια. Οι ακριβείς κωδικοί hex αναγράφονται δίπλα σε κάθε δείγμα, με την ίδια σειρά θεμάτων όπως στον πίνακα αναφοράς παραπάνω.

| Θέμα | Αποδιδόμενα χρώματα παλέτας | Κωδικοί hex (φόντο · προσκήνιο · λέξεις-κλειδιά · τύποι · συμβολοσειρές · σχόλια) |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Πρόσθετα χρώματα τονισμού χρησιμοποιούνται για συναρτήσεις, σταθερές, επιλογές, διαγνωστικά και οδηγίες προεπεξεργαστή. Η παλέτα Russian and Slavic χρησιμοποιεί σκόπιμα σχιστόλιθο, μπλε, φασκομηλιά, ρουμπίνι και αντικέ χρυσό μέσης έντασης για πιο ήρεμη επαγγελματική όψη.

### Εγκατάσταση των θεμάτων Vim

Από τη ρίζα του αποθετηρίου:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Προσθέστε τα παρακάτω στο `~/.vimrc` για να ενεργοποιήσετε την επισήμανση σύνταξης SLeeLa και να επιλέξετε θέμα:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Επιλέξτε ακριβώς ένα από τα παρακάτω ονόματα colorscheme:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Για παράδειγμα, χρησιμοποιήστε `colorscheme sleela-russian-slavic` για τη μέσης έντασης παλέτα Russian and Slavic, ή `colorscheme sleela-greek` / `colorscheme sleela-turkish` για τις παλέτες Hellenic και Al Bayrak. Επανεκκινήστε το Vim ή φορτώστε ξανά τις ρυθμίσεις σας με `:source $MYVIMRC`.

### Εγκατάσταση των θεμάτων Emacs

Δημιουργήστε έναν τοπικό κατάλογο θεμάτων και αντιγράψτε τα αρχεία mode και θεμάτων από τη ρίζα του αποθετηρίου:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Προσθέστε το παρακάτω στο `~/.emacs` ή στο `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Αντικαταστήστε το `sleela-american` με `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ή `sleela-sleela` για να επιλέξετε άλλο θέμα. Αν ο Emacs ρωτήσει αν εμπιστεύεστε το θέμα, ελέγξτε το τοπικό αρχείο θέματος πριν το αποδεχθείτε.

### Προεπισκόπηση των native παλετών C/C++

Το μικρό native βοηθητικό εργαλείο παλέτας μπορεί να απαριθμήσει τα θέματα, να εκτυπώσει τα χρώματα τερματικού τους και να αναζητήσει ένα μεμονωμένο χρώμα:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Για λόγους συμβατότητας προς τα πίσω, τα `--palette` και `--hex COLOR` χωρίς `--theme` επιλέγουν το American Professional. Τα αποτελέσματα κατασκευής και δοκιμών πρέπει να ελέγχονται στο σύστημα-στόχο και στο CI· η τεκμηρίωση μιας εντολής δεν αποτελεί από μόνη της απόδειξη ότι αυτή πέρασε.

Δείτε [τον οδηγό ενσωμάτωσης επεξεργαστών](linux-editors/README.md) για πλήρεις περιγραφές των παλετών και λεπτομέρειες του εργαλείου.

## IX. Συνεισφορά
1. Διαβάστε το ευρετήριο τεκμηρίωσης και την πολιτική δοκιμών.
2. Κάντε εστιασμένες αλλαγές με θετικές και αρνητικές δοκιμές.
3. Εκτελέστε στοχευμένες δοκιμές και έπειτα την ευρεία σουίτα.
4. Αναφέρετε λεπτομέρειες λειτουργικού συστήματος/αλυσίδας εργαλείων· μην περιγράφετε παραλειφθέντες ελέγχους ως επιτυχείς.

Η SLeeLa αναπτύσσεται από τη **MEARVK LLC**. Το ιστορικό υλικό της αρχικής σελίδας διατηρείται στο [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Αναμενόμενη εισφορά στα Round Public email

Για την πρωτοβουλία AE6E66, η δηλωμένη προσδοκία του έργου είναι **$300,000,000.00 USD ανά Round Public email**, ως προτεινόμενο ποσό φορολογικής εισφοράς που συνδέεται με κάθε πληρούν τις προϋποθέσεις Round Public email.

Η δήλωση αυτή καταγράφει αποκλειστικά μια προσδοκία του έργου. **Δεν** αποτελεί νόμιμο φορολογικό καταλογισμό, κυβερνητική εντολή, δικαστική απόφαση ή ανεξάρτητα τεκμηριωμένη φορολογική υποχρέωση. Κάθε πραγματική εισφορά θα απαιτούσε την αρμόδια νομική αρχή, δικαιοδοσία, ορισμούς, διαδικασία και αρμόδιο κυβερνητικό προσδιορισμό.


## XI. Αλφάβητο και περιβάλλον εκτέλεσης συνόλων χαρακτήρων

Η τεκμηρίωση αλφαβήτου και το native περιβάλλον εκτέλεσης χαρακτήρων της SLeeLa συντηρούνται στα alphabet/sleela/ και lib/alphabet/native/. Οι native δοκιμές C++17 καλύπτουν αυστηρή αποκωδικοποίηση/κωδικοποίηση βαθμωτών τιμών UTF-8, υπογραφές χωρητικότητας C<n> και φραγμένη συμμετοχή σε σύνολα χαρακτήρων:

    make -C lib/alphabet/native test

Ο πειραματικός κατάλογος UTF-4088 και ο προσαρμογέας αναζήτησής του για τη SLeeLa τεκμηριώνονται στο utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. Τα αναγνωριστικά UTF-4088 αποτελούν ξεχωριστό χώρο ονομάτων και όχι βαθμωτές τιμές Unicode. Η εργασία δέσμευσης για τον μεταγλωττιστή και το SLVM παρακολουθείται ρητά χωριστά από αυτά τα native θεμέλια.

## XII. AutoCAD Creator™

Το [AutoCAD Creator™](autocad/creator/README.md) είναι μια διαδραστική εφαρμογή
2D CAD γραμμένη εξ ολοκλήρου σε SLeeLa. Ενώ ο αδελφός AutoCAD Renderer
μετατρέπει ένα σχέδιο σε σχεδίαση με μία κίνηση, το Creator σας επιτρέπει να
**δημιουργείτε και να τροποποιείτε** ζωντανά ένα έγγραφο 2D AutoCAD —
προσθέτοντας τοίχους, δωμάτια, διαστάσεις και σημειώσεις, και έπειτα να
μετακινείτε, μετονομάζετε, αλλάζετε επίπεδο, διαγράφετε και αναιρείτε όσα
τοποθετήσατε — και να **αποθηκεύετε** το αποτέλεσμα ως πραγματικό σχέδιο
AutoCAD DXF (κείμενο R12, ανοίγει σε AutoCAD / LibreCAD).

Η γραφική του διεπαφή βασίζεται στο **SleelaUI™**, το δικό της cross-platform
εργαλειοσύνολο UI της SLeeLa (στο [`lib/user-interface`](lib/user-interface)) —
όχι JavaFX — που ανοίγει πραγματικό native παράθυρο σε X11 / Cocoa / Win32. Ο
σχεδιασμός ακολουθεί έναν κανόνα, **«το SleelaUI παρουσιάζει· η SLeeLa
αποφασίζει»**: το GUI δεν υπολογίζει ποτέ γεωμετρία και δεν γράφει DXF. Κάθε
εργαλείο εκδίδει πρόθεση δημιουργίας / τροποποίησης / διαγραφής / αναίρεσης /
αποθήκευσης προς τον κυρίαρχο [`SLCadCreator`](lib/autocad/SLCadCreator.sleela),
που κατέχει το επεξεργάσιμο έγγραφο και ένα αναστρέψιμο ιστορικό εντολών, ενώ ο
καμβάς απλώς προεπισκοπεί τη γεωμετρία που κατέχει ο Creator.

Η επαναχρησιμοποιήσιμη μηχανή επεξεργασίας βρίσκεται στο [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) και
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — και εκτελείται από άκρη σε άκρη
χωρίς οθόνη (headless). Το κέλυφος GUI και το demo βρίσκονται στο [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Σε σύστημα με οθόνη ανοίγει το ζωντανό παράθυρο επεξεργασίας· σε headless
host δημιουργεί το σχέδιο στη μνήμη, εκτυπώνει μια αναφορά μαζί με το DXF και
τερματίζει καθαρά.


## XIII. Σειρά χάρτη εισόδου UTF-4088

Ο εξειδικευμένος κατάλογος UTF-4088 ορίζει τη χωρική διάσχιση του χάρτη εισόδου και τις αφηρημένες επαναλήψεις πίεσης στο [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Η πολιτική ευρετηρίου θεμάτων National Lien βρίσκεται στο [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Οι συντεταγμένες χάρτη, τα επίπεδα πίεσης (1–50), τα μόνιμα αναγνωριστικά καταλόγου και οι αλφαβητικές θέσεις είναι ανεξάρτητα πεδία· κανένα δεν πρέπει να αντικαθίσταται σιωπηρά από άλλο. Η πίεση είναι αλγοριθμικό επίπεδο επανάληψης και όχι φυσικό μέγεθος.


## XIV. Πρόσφατο και διευρυμένο ευρετήριο πακέτων

Αυτό το προσαρτημένο ευρετήριο επισημαίνει πρόσφατα διευρυμένα πακέτα, βιβλιοθήκες
τομέα και συλλογές πηγαίου κώδικα. Οι εγγραφές ομαδοποιούνται αλφαβητικά κατά
όνομα πακέτου ή θέμα. Οι περιγραφές προσδιορίζουν τη θέση στο αποθετήριο· η
κατάσταση υλοποίησης και επαλήθευσης πρέπει να ελέγχεται στην τεκμηρίωση και τις
δοκιμές κάθε πακέτου.

### A–C

- **Advantage** — `lib/advantage/` και `advantage/`: αξιοποίηση ενός μοναδικού
  μερίσματος σε σχέση με ένα μοντέλο ως πολυεπίπεδο κρεμμύδι Sequitur, με πέντε
  υποφακέλους θεμάτων — `math/` (από κοινού μεγιστοποίηση κέρδους, διαπραγμάτευση
  Nash, διμερές newsvendor), `physics/` (μετρική/γεωδαισιακή/επαφή/SDE πεδίου
  εμπιστοσύνης, επισημασμένη αναλογία μοντελοποίησης), `integration/` (διαμερίσματα
  κάθετης ολοκλήρωσης και βαθμολογία πολυπλοκότητας), `grading/` (βαθμολογητής
  κυκλώματος πολυπλοκότητας A–D για ενότητες και σύνολα) και `output/` (εκπομπή
  σε τερματικό / CommonRails / socket). Ένας native καταναλωτής C/C++
  (`advantage/`) συνδέει το μοντέλο με επεξεργασμένα παραδείγματα συναλλαγών.
  Δείτε το [`lib/advantage/README.md`](lib/advantage/README.md)
  και το [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet και UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` και
  `utf-4088/`: κατάλογοι αλφαβήτων, native υποστήριξη συνόλων χαρακτήρων, σειρά
  χάρτη εισόδου και τεκμηρίωση ενσωμάτωσης UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` και `lib/autocad/`: διαδραστικές ροές
  εργασίας 2D σχεδίασης και επαναχρησιμοποιήσιμα στοιχεία εγγράφων/επεξεργασίας CAD.
- **Ballum** — έλεγχος οθόνης σε επίπεδο pixel: set/get ανά pixel, blit περιοχής,
  φωτεινότητα/θερμότητα/gamma ολόκληρης της οθόνης, είσοδος δείκτη/πληκτρολογίου
  και μια **επιφάνεια framebuffer πλήρους οθόνης** για πλήρη, ιδιόκτητη, γρήγορη
  ανά pixel διαχείριση (παράθυρο override-redirect + μία παρουσίαση
  `XPutImage`/MIT-SHM σε Linux/X11). Native πυρήνας στο `ballum/` με την όψη
  SLeeLa στο `lib/ballum/` και γέφυρα VM στο `impl/`· δείτε το
  [`ballum/README.md`](ballum/README.md) και τα εκτελέσιμα
  [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` και `lib/cdma/`: τεχνικό υλικό σχετικό με CDMA και εργασία
  βιβλιοθήκης πηγαίου κώδικα SLeeLa.
- **Character** — `lib/character/`: προφίλ χαρακτήρων και επαναχρησιμοποιήσιμα
  στατιστικά χαρακτήρων, συμπεριλαμβανομένων παραδειγμάτων βιογραφικών προφίλ.
- **Coorenagraph** — `lib/coorenagraph/`: ηθική συλλογιστική, ηθικό λεξιλόγιο,
  διαβούλευση και συναφείς έννοιες· δείτε το README του πακέτου και τα αρχεία πηγαίου κώδικα.

### D–G

- **Governance source models** — το [lib/governance](lib/governance/README.md) ορίζει δηλωτικά σχήματα πηγαίου κώδικα SLeeLa για προτάσεις, ανώνυμες χειροκίνητες ψηφοφορίες και συγκεντρωτικές καταμετρήσεις. Η προτεινόμενη σημείωση @vote και η χειροκίνητη διαδικασία τεκμηριώνονται στο [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md)· οι τρέχοντες μεταγλωττιστές τη μεταχειρίζονται ως αδρανή μεταδεδομένα σχολίου.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` έως τους διαθέσιμους καταλόγους πακέτων HTTP
  ανά έκδοση: προδιαγραφές πρωτοκόλλου και υλοποιήσεις ανά έκδοση. Συμβουλευτείτε
  την τεκμηρίωση κάθε έκδοσης για το ακριβές πεδίο και την κατάσταση επαλήθευσής της.
- **International Criminal Court** — `lib/international-criminal-court/` και
  `INTERNATIONAL.CRIMINAL.COURTS.md`: συναφής πηγαίος κώδικας και τεκμηρίωση
  συγκεκριμένου τομέα.

### O–S

- **OFDMA** — `ofdma/`: τεχνικό υλικό για πολλαπλή πρόσβαση ορθογώνιας διαίρεσης
  συχνότητας και πόροι ανάπτυξης C/C++.
- **Philosophy και η ενότητα Conversation Mask** — το [`philosophy/`](philosophy/README.md)
  περιέχει τα native θεμέλια C/C++ και το [`philosophy/module/`](philosophy/module/README.md)
  οργανώνει το επόμενο επίπεδο ενότητας σε τρεις ρητές περιοχές:
  [`data/`](philosophy/module/data/README.md) για τυποποιημένες, χρονολογημένες
  παρατηρήσεις, λήξεις γεγονότων, ρυθμίσεις ημιζωής, προέλευση και συναίνεση·
  [`helper/`](philosophy/module/helper/README.md) για ντετερμινιστική επικύρωση,
  φθορά συνάφειας, συμβατές συγκρίσεις και παρουσίαση φιλτραρισμένη βάσει
  συναίνεσης· και [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  για μια υποστηρικτική, μη κλινική διεπαφή συνομιλίας. Οι λαβές προς τη SLeeLa
  και το συντεθειμένο Tree βρίσκονται στο [`lib/philosophy/`](lib/philosophy/README.md),
  δίπλα στο ευρύτερο πακέτο philosophy. Η ενότητα βασίζεται σε ρητή είσοδο και
  προτάσσει τη συναίνεση: δεν παρακολουθεί παθητικά ανθρώπους ή μέσα, δεν
  διαγιγνώσκει, δεν συμπεραίνει ιδιωτικές προθέσεις, δεν βαθμολογεί την ανθρώπινη
  αξία και δεν προτείνει αλλαγές φαρμακευτικής αγωγής. Οι σύνδεσμοι γεγονότων
  διατηρούν την προέλευση και την πλοήγηση, όχι ανεξάρτητη επιβεβαίωση.
- **Synchro** — ενσωμάτωση native runtime/μεταγλωττιστή και λειτουργικότητα
  αποστολής πακέτων και μετρήσεων· δείτε τον σχετικό πηγαίο κώδικα στο `impl/` και
  την υλοποίηση του μεταγλωττιστή.
- **SleelaUI / terminal UI** — `lib/user-interface/` και το έργο
  `sleela-terminal`: ενσωμάτωση UI της SLeeLa και στοιχεία αλληλεπίδρασης
  προσανατολισμένα στο τερματικό.

### U–W

- **United Nations** — `united-nations/`: υλικό αποθετηρίου συγκεκριμένου τομέα.
- **Wi-Fi 4, Wi-Fi 5 και Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` και συναφείς κατάλογοι πηγαίου κώδικα `lib/wifi/`: υλικό και
  πόροι ρύθμισης ασύρματου δικτύου ανά γενιά.

### Συντήρηση του ευρετηρίου πακέτων

Όταν ένα πακέτο προστίθεται ή διευρύνεται ουσιωδώς, προσθέστε την εγγραφή του
εδώ στην κατάλληλη αλφαβητική ομάδα, συνδέστε τον κανονικό κατάλογο και το README
του και περιγράψτε μόνο δυνατότητες που υποστηρίζονται από τον πηγαίο κώδικα που
έχει καταχωρηθεί. Διατηρήστε αυτό το ευρετήριο προσθετικό: μην αφαιρείτε και μην
ξαναγράφετε τις υπάρχουσες ενότητες του README στο πλαίσιο ενημερώσεων του
ευρετηρίου πακέτων.




---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Εθνική εικονογραφία επωνυμίας Science

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Πειραματικός κατάλογος UTF-4088 ενός εκατομμυρίου εγγραφών

Η [γεννήτρια καταλόγου UTF-4088](utf-4088/tools/generate_catalogue.py) κατασκευάζει ντετερμινιστικά έναν πειραματικό κατάλογο συμβόλων 1.048.576 εγγραφών. Η τεκμηριωμένη συμπαγής μορφή περιγράφεται στο [utf-4088/FORMAT.md](utf-4088/FORMAT.md)· η σύμβαση δημιουργίας και επικύρωσης βρίσκεται στο [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Εκτελέστε τη γεννήτρια με Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Το παραγόμενο δυαδικό αρχείο έχει μέγεθος 16.777.244 bytes και σχεδιάζεται ώστε να παραμένει κάτω από 20.000.000 bytes. Δοκιμές μονάδας και μια ροή εργασιών GitHub Actions επαληθεύουν το πλήθος εγγραφών, τη μοναδικότητα των γλυφών, το μέγεθος και το SHA-256· οι επιτυχείς εκτελέσεις δημοσιεύουν το δυαδικό αρχείο και τη σύνοψη ως artifact `utf4088-million-catalogue` της ροής εργασιών. Το παραγόμενο δυαδικό αρχείο διατηρείται σκόπιμα εκτός του ιστορικού του Git. Το UTF-4088 παραμένει πειραματικό και δεν είναι συμβατό με το Unicode.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Πρόσφατες προσθήκες — σταθερά Pi, εκκίνηση Modium/Registry, μαθηματικά sleelactl και `@native`

Η ενότητα αυτή καταγράφει χαρακτηριστικά που προστέθηκαν κατά την εργασία του 2026-11, με τις νεότερες ομάδες πρώτες. Τα συνδεδεμένα αρχεία πηγαίου κώδικα και η κατάσταση επαλήθευσής τους είναι κανονιστικά· αλλαγές υπό αξιολόγηση δεν περιγράφονται ως συγχωνευμένες έως ότου φτάσουν στο `master`.

### Η σταθερά καταλόγου Pi με 100.000.000 ψηφία

Το αποθετήριο περιλαμβάνει μια επαληθευμένη τιμή του π **εκατό εκατομμυρίων ψηφίων** στο [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (σωστά διαμορφωμένος δεκαδικός, `3.14159…`), μαζί με μια τιμή 57.344 ψηφίων στο [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Και οι δύο έχουν επαληθευτεί ανεξάρτητα ως γνήσιο π — δείτε το [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), που τεκμηριώνει τους ελέγχους (το πρόθεμα των εγχειριδίων, το σημείο Feynman στη δεκαδική θέση 762, το γνωστό ένα-εκατομμυριοστό ψηφίο, η συμφωνία με το `lib/math/SLPiReference.sleela` και μια ανάλυση συχνότητας ψηφίων ολόκληρου του αρχείου) και ένα σενάριο αναπαραγωγής με μία κίνηση.

### Τα Modium™ / Registry™ φορτώνουν τη σταθερά Pi κατά την εκκίνηση του υποσυστήματος

Στα αποκλειστικά σημεία εισόδου των υποσυστημάτων — `sleela registry` και `sleela modium` (και στην πρώτη εκτέλεση του προγράμματος εγκατάστασης), **όχι** σε κάθε `sleela run` — τα υποσυστήματα Modium και Registry φορτώνουν τη μεγάλη σταθερά καταλόγου π. Σε διαδραστικό τερματικό εκτυπώνουν μια παροδική κόκκινη γραμμή `Loading Modium™` / `Loading Registry™`, η οποία σβήνεται επιτόπου πριν ξεκινήσει η κανονική έξοδος· η επίδραση περιορίζεται αυστηρά σε TTY, ώστε η έξοδος μέσω σωλήνωσης και δοκιμών να παραμένει αμετάβλητη byte προς byte. Ο εντοπιστής (όχι τα bytes) εισάγεται ως προεπιλογές του κλειδιού `Math` του registry, `piReferenceLargePath` και `piReferenceLargeDigits`, τόσο στο μοντέλο SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) όσο και στον native seeder ([`registry/sleela_registry.c`](registry/sleela_registry.c))· το `lib/math/SLPiReference.sleela` εκθέτει τα `largePath()` / `largeDigits()`. Η σταθερά π εγκαθίσταται στη θέση SLeeLa του λειτουργικού συστήματος (`<data>/pi/`) **και** από το τυπικό πρόγραμμα εγκατάστασης `make` **και** από το καθοδηγούμενο «όμορφο» πρόγραμμα εγκατάστασης. Λεπτομέρειες: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — το «όμορφο» πρόγραμμα εγκατάστασης και ένα θέμα `math` με native υποστήριξη

- Το καθοδηγούμενο («όμορφο») πρόγραμμα εγκατάστασης εγκαθιστά πλέον το `sleelactl` (το επίπεδο ελέγχου της SLeeLa) και το μητρώο ενοτήτων του, όπως και το πρόγραμμα εγκατάστασης `make`. Δείτε το [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- Το `sleelactl` απέκτησε μια νέα φύση **`subject`** για τις native βιβλιοθήκες θεμάτων C/C++ που έχουν μεταγλωττιστεί στο περιβάλλον εκτέλεσης, και καταχωρεί την πρώτη: το **`math`**. Το `sleelactl run math` εκτελεί από άκρη σε άκρη το native backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) μέσω του demo αναφοράς του θέματος· το `sleelactl info math` εξηγεί πώς προσεγγίζεται (`import math;`). Η φύση subject προσφέρει μόνο `run info` — σκόπιμα δεν υπάρχει ρήμα `check`, επειδή το `sleela check` δεν φορτώνει τα native θέματα που είναι καταχωρημένα στο SHEET.

### `@native`, Native Drive και εκτέλεση σε πολλές γλώσσες

Μια νέα σημείωση μεθόδου **`@native`** ζητά μεταγλώττιση εκ των προτέρων (ahead-of-time) σε native κώδικα για μια «θερμή» μέθοδο. Η νέα εντολή `sleela aot` εντοπίζει τις μεθόδους `@native`, επαληθεύει ότι καθεμία βρίσκεται εντός ενός υποστηριζόμενου *υποσυνόλου που μπορεί να εκφορτωθεί* και εκπέμπει C/C++ — **αποτυγχάνοντας με ασφαλή τρόπο (fail closed)**, δηλαδή αρνούμενη, με ακριβή αιτιολογία, κάθε δομή που δεν μπορεί ακόμη να μεταφράσει, ώστε να μην παράγεται ποτέ μισομεταφρασμένη μέθοδος.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

Το `@native` παραμένει αίτημα για επεξεργασία AOT, με τις μη υποστηριζόμενες δομές να απορρίπτονται πριν από την εκπομπή. Το [Native Drive](impl/native-drive/README.md) υποστηρίζει κατασκευές εκτελέσιμων και κοινόχρηστων βιβλιοθηκών C/C++ από πολλές μονάδες μέσω μεταγλωττιστών του host (συμπεριλαμβανομένου του MSVC), υποστηριζόμενη έξοδο AOT της SLeeLa, επιθεώρηση συμβόλων σε σχέση με το ABI του native loader, εκτέλεση διεργασιών χωρίς shell, χειρισμό stdin/stdout/stderr, σωληνώσεις πολλαπλών σταδίων και ανάθεση XClass στον loader παραγωγής. Το `sleelactl` εκθέτει τις εντολές κατασκευής/εκτέλεσής του και δρομολογεί την επιθεώρηση XClass και την ρητή απόδοση/εγκατάσταση μονάδων systemd μέσω του `inspect native-drive`. Η VM κατέχει έναν φραγμένο πίνακα λαβών native ενοτήτων με υποχρεωτική εξουσιοδότηση του host και λαβές με έλεγχο γενιάς. Η σύνταξη 3.0 προσθέτει πλέον τα `nativeOpen`, `nativeCall` και `nativeClose`: η μόνο προσθετική αποστολή bytecode διασχίζει ένα ξεχωριστά εκδιδόμενο ABI null/bool/int64/double/UTF-8, ενώ πίνακες, δομές και άλλες σύνθετες τιμές που ανήκουν στη VM παραμένουν εντός του διερμηνέα και μπορεί να χρησιμοποιούν εφεδρική λύση bytecode με το ίδιο όνομα. Τα plugins μπορούν προαιρετικά να λάβουν έναν εκδιδόμενο πίνακα callback του host για να καλούν ακριβείς συναρτήσεις SLeeLa μέσω εμφωλευμένης εκτέλεσης VM, με ρίζωση GC της αναστελλόμενης στοίβας, ρητή λίστα επιτρεπόμενων ονομάτων callback του host και απόρριψη αναδρομικών κλήσεων του native API. Ο native κώδικας παραμένει έμπιστος κώδικας εντός της διεργασίας και όχι sandbox. Οι σημειώσεις πηγαίου κώδικα πολιτικής διεργασιών έχουν εκδιδόμενο σχήμα, αλλά δεν αναλύονται/επιβάλλονται ακόμη. Λεπτομέρειες, όρια ασφαλείας και το καθολικό των επτά ροών εργασίας: [Native Drive](impl/native-drive/README.md) και [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Διεθνείς εκδόσεις README

Η SLeeLa παρέχει σημεία εισόδου README ανά χώρα για τις 50 αγορές της διεθνούς έκδοσής της. Κάθε χώρα έχει ένα αρχείο με όνομα `README.<CC>.md`· όπου σχετίζονται περισσότερες γλώσσες, ομαδοποιούνται στο αρχείο της συγκεκριμένης χώρας.

- [README.AE](README.AE.md)
- [README.AT](README.AT.md)
- [README.AU](README.AU.md)
- [README.BE](README.BE.md)
- [README.BR](README.BR.md)
- [README.CA](README.CA.md)
- [README.CH](README.CH.md)
- [README.CL](README.CL.md)
- [README.CN](README.CN.md)
- [README.CO](README.CO.md)
- [README.CZ](README.CZ.md)
- [README.DE](README.DE.md)
- [README.DK](README.DK.md)
- [README.EE](README.EE.md)
- [README.ES](README.ES.md)
- [README.FI](README.FI.md)
- [README.FR](README.FR.md)
- [README.GB](README.GB.md)
- [README.GR](README.GR.md)
- [README.HK](README.HK.md)
- [README.HU](README.HU.md)
- [README.ID](README.ID.md)
- [README.IE](README.IE.md)
- [README.IL](README.IL.md)
- [README.IN](README.IN.md)
- [README.IS](README.IS.md)
- [README.IT](README.IT.md)
- [README.JP](README.JP.md)
- [README.KR](README.KR.md)
- [README.LU](README.LU.md)
- [README.MX](README.MX.md)
- [README.MY](README.MY.md)
- [README.NL](README.NL.md)
- [README.NO](README.NO.md)
- [README.NZ](README.NZ.md)
- [README.PH](README.PH.md)
- [README.PL](README.PL.md)
- [README.PT](README.PT.md)
- [README.QA](README.QA.md)
- [README.RU](README.RU.md)
- [README.SA](README.SA.md)
- [README.SE](README.SE.md)
- [README.SG](README.SG.md)
- [README.SI](README.SI.md)
- [README.SK](README.SK.md)
- [README.TH](README.TH.md)
- [README.TR](README.TR.md)
- [README.TW](README.TW.md)
- [README.US](README.US.md)
- [README.ZA](README.ZA.md)

**Πολιτική μετάφρασης:** Τα αρχεία αυτά είναι αρχικά τοπικοποιημένα σημεία εισόδου ανά χώρα. Οι εισαγωγές και οι επικεφαλίδες γλωσσών τους είναι τοπικοποιημένες, ενώ η πλήρης τεχνική αναφορά παραμένει στο κανονιστικό αγγλικό [README.md](README.md). Ένα αρχείο χώρας δεν πρέπει να περιγράφεται ως πλήρης μετάφραση έως ότου μεταφραστεί κάθε ενότητα, ελεγχθούν τα τεχνικά αναγνωριστικά και οι σύνδεσμοι και η έκδοση λάβει γλωσσικό έλεγχο. Η γλωσσική κάλυψη σε αυτή την πρώτη φάση είναι αντιπροσωπευτική και δεν συνιστά ισχυρισμό ότι κάθε συνταγματικά αναγνωρισμένη, περιφερειακή, αυτόχθονη ή νοηματική γλώσσα έχει μεταφραστεί πλήρως.

## Παράρτημα — διατηρούμενη περιφερειακή καθοδήγηση

Το παράρτημα αυτό διατηρεί πρακτική καθοδήγηση από τον προηγούμενο σύντομο ελληνικό οδηγό, η οποία δεν υπάρχει ως αυτοτελείς ενότητες στο κανονιστικό README. Σε περίπτωση οποιασδήποτε ασυμφωνίας υπερισχύει το κανονιστικό αγγλικό [README.md](README.md).

### Στόχοι, προφίλ και όρια εκτέλεσης

Η SLeeLa διαχωρίζει τον πηγαίο κώδικα από τους στόχους κατασκευής και εκτέλεσης. Η τεκμηριωμένη οικογένεια στόχων περιλαμβάνει τα `SLVM`, `SLJVM` και `SLIR`· το `NATIVE` είναι περιορισμένος στόχος υπό προϋποθέσεις και δεν πρέπει να θεωρείται διαθέσιμος σε κάθε πλατφόρμα. Τα προφίλ `BASIC_COMPLETE` και `ADVANCED_TOTAL` δηλώνουν διαφορετικό εύρος. Επαληθεύετε τον πραγματικό στόχο, το προφίλ και την υποστήριξη πλατφόρμας κάθε κατασκευής.

Μια αναφορά προσδιορίζει αρχείο, ενότητα ή πόρο· δεν παρέχει από μόνη της εξουσιοδότηση εκτέλεσης. Όταν ένα πρόγραμμα χρησιμοποιεί αρχεία, εξωτερικές βιβλιοθήκες, native bindings ή πόρους συστήματος, πρέπει να ορίζονται ρητά τα όρια εξουσιοδότησης και η ευθύνη εκτέλεσης. Μια περιγραφή στην τεκμηρίωση δεν αποδεικνύει από μόνη της εγγύηση ασφάλειας του runtime· εξετάστε την υλοποίηση και τις σχετικές δοκιμές.

### Κύκλος ζωής πόρων και αποσφαλμάτωση

Καταγράψτε την ιδιοκτησία, τη διάρκεια ζωής και τις ευθύνες συγχρονισμού των πόρων που μοιράζονται μεταξύ συναρτήσεων ή νημάτων. Διακρίνετε local variables, shared object state, VM handles και serializable data. Ένα VM-local handle δεν είναι σταθερή μορφή αρχείου ή μήνυμα δικτύου. Χρησιμοποιήστε το serialization interface της ενότητας και δοκιμάστε invalid, missing και boundary values.

Κατά την αποσφαλμάτωση καταγράψτε commit, OS, architecture και toolchain. Κατασκευάστε πρώτα τον μικρότερο σχετικό στόχο, διατηρήστε ολόκληρο το error output, διαχωρίστε compiler errors από runtime errors και δημιουργήστε ελάχιστο αναπαραγώγιμο παράδειγμα. Η επιτυχής κατασκευή δεν αποδεικνύει από μόνη της σωστή συμπεριφορά κατά την εκτέλεση.

Η αναφορά επικύρωσης πρέπει να περιλαμβάνει exact commit, OS/architecture, compiler versions, πλήρεις build/test commands, αριθμούς pass/fail/skip και γνωστούς περιορισμούς. Οι δοκιμές που παραλείφθηκαν ή δεν εκτελέστηκαν δεν μετρούν ως passed.

### Συμβόλαια API ενοτήτων, συμβατότητα και στρατηγική δοκιμών

Κατά την προσθήκη νέου API, τεκμηριώστε τις εισόδους, τις εξόδους, τις συνθήκες σφάλματος, την ιδιοκτησία δεδομένων και τυχόν side effects. Τα παραδείγματα πρέπει να παρουσιάζουν τόσο τη συνηθισμένη χρήση όσο και τον χειρισμό invalid ή missing input. Μην παρουσιάζετε μια συμπεριφορά ως εγγυημένη αν δεν επιβεβαιώνεται από την implementation και τις σχετικές tests.

Οργανώστε τις δοκιμές ανά στάδιο του compiler pipeline: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution και native integration. Για κάθε regression case διατηρήστε την ελάχιστη αναπαραγώγιμη είσοδο, το αναμενόμενο αποτέλεσμα και το πραγματικό αποτέλεσμα. Οι αλλαγές API πρέπει να εξηγούν την backward compatibility, τα migration steps και την επίδραση στα υπάρχοντα examples.

### Τοπικοποίηση, κωδικοποίηση και αναγνωσιμότητα

Διατηρείτε αμετάβλητα τα technical identifiers, commands, paths, syntax και code blocks σε όλες τις γλωσσικές εκδόσεις. Μεταφράζετε το επεξηγηματικό κείμενο, όχι τα tokens ή τα ονόματα που χρησιμοποιούνται στο source code. Δηλώνετε με σαφήνεια τη γλώσσα και το περιφερειακό πλαίσιο.

Μετά από κάθε ενημέρωση ελέγχετε τα Markdown links, την ιεραρχία επικεφαλίδων, τα code fences και τα παραδείγματα. Όταν αλλάζουν τα canonical English documents, συγκρίνετε τις αντίστοιχες ενότητες και ενημερώνετε ολόκληρη τη μετάφραση, όχι μόνο την εισαγωγή. Το περιεχόμενο που δεν έχει ελεγχθεί γλωσσικά ή τεχνικά πρέπει να επισημαίνεται.

**Κατάσταση μετάφρασης:** Οι ενότητες I–XIX έχουν μεταφραστεί στα ελληνικά. Τα τεχνικά αναγνωριστικά, οι εντολές, τα δείγματα κώδικα, τα ονόματα αρχείων, οι σύνδεσμοι και οι διευθύνσεις διατηρούνται αμετάβλητα. Συνιστάται τελικός γλωσσικός έλεγχος από φυσικούς ομιλητές.
