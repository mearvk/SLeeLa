<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Language Packs

Localize SLeeLa's outputs and prompts into a language other than English.

A **language pack** is a small, self-contained, dependency-free text file that
maps a stable **message key** to its translation in one language. SLeeLa
programs look up keys through the `lib/languages` library
(`SLLanguage` / `SLLanguageCatalog`); tooling and front ends can read the same
files directly. English (`en`) is the reference pack — every other pack
translates the same key set.

## Why keys, not sentences

Programs reference a stable key (e.g. `prompt.continue`) and the active pack
supplies the display text. Changing language is then a one-line choice
(`SLLanguageCatalog.use("fr")`) with no change to program logic — the SLeeLa
rule *decide in code, present in the user's language*.

## Pack file format

- UTF-8 text, one entry per line: `key = value`.
- Blank lines and lines beginning with `#` are comments.
- The leading `#!locale` / `#!name` / `#!direction` header lines carry metadata
  (BCP-47 code, endonym, and `ltr`/`rtl`). `direction=rtl` marks
  right-to-left scripts (Arabic, Hebrew).
- A value may contain `{0}`, `{1}`, … positional placeholders that the caller
  fills at runtime (e.g. a count or a file name).
- Keys absent from a pack fall back to the English (`en`) value.

```
#!locale = fr
#!name = Français
#!direction = ltr
greeting.hello = Bonjour
prompt.continue = Continuer ?
```

## Message key namespaces

| Namespace    | Purpose                                        |
|--------------|------------------------------------------------|
| `greeting.*` | Common greetings / salutations.                |
| `prompt.*`   | Interactive prompts (yes/no, continue, retry). |
| `word.*`     | Single-word UI labels (yes, no, ok, cancel).   |
| `status.*`   | Runtime status lines (ready, done, failed).    |
| `error.*`    | Error message leads (not found, denied).       |
| `unit.*`     | Measurement/あtime words (seconds, bytes).      |

The canonical key set and its English reference are in
[`en.lang`](en.lang). The complete key list is in [`KEYS.md`](KEYS.md).

## Included packs

Each pack below is an original, concise translation of the reference key set,
authored for this repository and released under the repository's terms — no
third-party pack content is bundled, so there are no external licenses to track.

| Code | Language    | File        | Script direction |
|------|-------------|-------------|------------------|
| `en` | English     | `en.lang`   | ltr (reference)  |
| `es` | Español     | `es.lang`   | ltr              |
| `fr` | Français    | `fr.lang`   | ltr              |
| `de` | Deutsch     | `de.lang`   | ltr              |
| `pt` | Português   | `pt.lang`   | ltr              |
| `it` | Italiano    | `it.lang`   | ltr              |
| `ja` | 日本語       | `ja.lang`   | ltr              |
| `zh` | 中文 (简体)  | `zh.lang`   | ltr              |
| `hi` | हिन्दी        | `hi.lang`   | ltr              |
| `ko` | 한국어       | `ko.lang`   | ltr              |
| `th` | ไทย         | `th.lang`   | ltr              |
| `ar` | العربية      | `ar.lang`   | rtl              |

## Using a pack from SLeeLa

```
import languages;

class Hello {
    void main() {
        SLLanguageCatalog cat = new SLLanguageCatalog();
        cat.use("fr");
        print(cat.text("greeting.hello"));   // Bonjour
        print(cat.text("prompt.continue"));  // Continuer ?
    }
}
```

A runnable demonstration is `lib/languages/languages.sleela`.

## Settings

[`settings.conf`](settings.conf) is the single place to record a localization
preference, so a chosen language persists and becomes the default. It is an
INI-style file matching the repository's `config/` convention:

```ini
[language]
active   = "en"   # BCP-47 code present here: en es fr de pt it ja zh hi ko th ar
fallback = "en"   # used when a key is missing or the active code is unknown
mode     = "folder"
relative_path = "."

[format]
respect_direction   = true   # honor a pack's #!direction (ltr/rtl)
enable_placeholders = true   # fill {0}, {1}, ... in messages
missing_key         = "key"  # "key" echoes the key; "empty" yields ""

[override]
env_var = "SLEELA_LANG"      # when set, overrides [language].active for a run
```

Switch the language of all localized output by changing one line
(`active = "th"`), or per-run via the `SLEELA_LANG` environment variable. The
`lib/languages` library reads these settings through `SLLanguageSettings`
(active/fallback locale, direction, placeholder, and missing-key policy); its
`resolve(override)` applies the override-then-active precedence.

**Max Rupplin — MEARVK LLC — 2026**

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>


## World language research directory — top 80 by total speakers

The subdirectories in this section are **research profiles**, separate from the existing localization language packs. The initial 80 entries follow the 2025 total-speaker ranking (native plus second-language speakers) and retain the source's language/variety distinctions. Speaker estimates are approximate and should be refreshed against a current primary dataset before formal analysis.

The directory records educational attainment, age structure, subjective well-being, fertility, and savings as **geographic socioeconomic indicators**. These measures are not intrinsic properties of a language or its speakers. “Maturity” is represented only through neutral demographic measures; “loveliness” is not objectively measurable and is represented, if desired, by sourced life-satisfaction/well-being measures. Fertility is recorded as total fertility rate (births per woman), and savings measures must specify household or national savings.

| Rank | Language | Estimated total speakers (millions) | Directory |
|---:|---|---:|---|
| 1 | English | 1500 | [english/](english/README.md) |
| 2 | Mandarin Chinese | 1200 | [mandarin-chinese/](mandarin-chinese/README.md) |
| 3 | Hindi | 609.1 | [hindi/](hindi/README.md) |
| 4 | Spanish | 558.5 | [spanish/](spanish/README.md) |
| 5 | Standard Arabic | 334.8 | [standard-arabic/](standard-arabic/README.md) |
| 6 | French | 311.9 | [french/](french/README.md) |
| 7 | Bengali | 284.3 | [bengali/](bengali/README.md) |
| 8 | Portuguese | 266.6 | [portuguese/](portuguese/README.md) |
| 9 | Russian | 253.4 | [russian/](russian/README.md) |
| 10 | Indonesian | 252.4 | [indonesian/](indonesian/README.md) |
| 11 | Urdu | 246 | [urdu/](urdu/README.md) |
| 12 | Standard German | 134 | [standard-german/](standard-german/README.md) |
| 13 | Japanese | 125.6 | [japanese/](japanese/README.md) |
| 14 | Nigerian Pidgin | 120.7 | [nigerian-pidgin/](nigerian-pidgin/README.md) |
| 15 | Egyptian Arabic | 118.6 | [egyptian-arabic/](egyptian-arabic/README.md) |
| 16 | Marathi | 99.3 | [marathi/](marathi/README.md) |
| 17 | Vietnamese | 97 | [vietnamese/](vietnamese/README.md) |
| 18 | Telugu | 95.8 | [telugu/](telugu/README.md) |
| 19 | Hausa | 94.4 | [hausa/](hausa/README.md) |
| 20 | Turkish | 91.3 | [turkish/](turkish/README.md) |
| 21 | Western Punjabi | 90.3 | [western-punjabi/](western-punjabi/README.md) |
| 22 | Swahili | 87.2 | [swahili/](swahili/README.md) |
| 23 | Tagalog | 86.9 | [tagalog/](tagalog/README.md) |
| 24 | Tamil | 86.3 | [tamil/](tamil/README.md) |
| 25 | Yue Chinese | 85.7 | [yue-chinese/](yue-chinese/README.md) |
| 26 | Wu Chinese | 83.4 | [wu-chinese/](wu-chinese/README.md) |
| 27 | Iranian Persian | 82.6 | [iranian-persian/](iranian-persian/README.md) |
| 28 | Korean | 81.6 | [korean/](korean/README.md) |
| 29 | Thai | 71.4 | [thai/](thai/README.md) |
| 30 | Javanese | 69.2 | [javanese/](javanese/README.md) |
| 31 | Italian | 66.2 | [italian/](italian/README.md) |
| 32 | Gujarati | 62.5 | [gujarati/](gujarati/README.md) |
| 33 | Levantine Arabic | 60.4 | [levantine-arabic/](levantine-arabic/README.md) |
| 34 | Amharic | 60 | [amharic/](amharic/README.md) |
| 35 | Kannada | 59.7 | [kannada/](kannada/README.md) |
| 36 | Bhojpuri | 52.7 | [bhojpuri/](bhojpuri/README.md) |
| 37 | Sudanese Arabic | 52.3 | [sudanese-arabic/](sudanese-arabic/README.md) |
| 38 | Yoruba | 49.9 | [yoruba/](yoruba/README.md) |
| 39 | Jinyu Chinese | 48 | [jinyu-chinese/](jinyu-chinese/README.md) |
| 40 | Min Nan Chinese | 45.8 | [min-nan-chinese/](min-nan-chinese/README.md) |
| 41 | Polish | 45.3 | [polish/](polish/README.md) |
| 42 | Burmese | 44.4 | [burmese/](burmese/README.md) |
| 43 | Algerian Arabic | 41.6 | [algerian-arabic/](algerian-arabic/README.md) |
| 44 | Hakka Chinese | 40.7 | [hakka-chinese/](hakka-chinese/README.md) |
| 45 | Lingala | 40.6 | [lingala/](lingala/README.md) |
| 46 | Moroccan Arabic | 40.2 | [moroccan-arabic/](moroccan-arabic/README.md) |
| 47 | Odia | 39.5 | [odia/](odia/README.md) |
| 48 | Ukrainian | 39 | [ukrainian/](ukrainian/README.md) |
| 49 | Xiang Chinese | 38.2 | [xiang-chinese/](xiang-chinese/README.md) |
| 50 | Malayalam | 38.2 | [malayalam/](malayalam/README.md) |
| 51 | Sindhi | 36.9 | [sindhi/](sindhi/README.md) |
| 52 | Eastern Punjabi | 36.5 | [eastern-punjabi/](eastern-punjabi/README.md) |
| 53 | Sunda | 35.5 | [sunda/](sunda/README.md) |
| 54 | Igbo | 33.7 | [igbo/](igbo/README.md) |
| 55 | Dari | 33.4 | [dari/](dari/README.md) |
| 56 | Nepali | 33.1 | [nepali/](nepali/README.md) |
| 57 | Malay | 29.5 | [malay/](malay/README.md) |
| 58 | Northern Uzbek | 29.4 | [northern-uzbek/](northern-uzbek/README.md) |
| 59 | Zulu | 27.8 | [zulu/](zulu/README.md) |
| 60 | Sa'idi Arabic | 27.3 | [saidi-arabic/](saidi-arabic/README.md) |
| 61 | Northern Pashto | 27.2 | [northern-pashto/](northern-pashto/README.md) |
| 62 | West Central Oromo | 27 | [west-central-oromo/](west-central-oromo/README.md) |
| 63 | Saraiki | 25.6 | [saraiki/](saraiki/README.md) |
| 64 | Dutch | 25.4 | [dutch/](dutch/README.md) |
| 65 | Somali | 24.7 | [somali/](somali/README.md) |
| 66 | Romanian | 23.7 | [romanian/](romanian/README.md) |
| 67 | Assamese | 23.6 | [assamese/](assamese/README.md) |
| 68 | Gan Chinese | 22.6 | [gan-chinese/](gan-chinese/README.md) |
| 69 | Southern Pashto | 21.7 | [southern-pashto/](southern-pashto/README.md) |
| 70 | Cebuano | 21.4 | [cebuano/](cebuano/README.md) |
| 71 | Kazakh | 21.4 | [kazakh/](kazakh/README.md) |
| 72 | Magahi | 21 | [magahi/](magahi/README.md) |
| 73 | Najdi Arabic | 20.9 | [najdi-arabic/](najdi-arabic/README.md) |
| 74 | Sinhala | 20.2 | [sinhala/](sinhala/README.md) |
| 75 | Mesopotamian Arabic | 20.1 | [mesopotamian-arabic/](mesopotamian-arabic/README.md) |
| 76 | Xhosa | 19.2 | [xhosa/](xhosa/README.md) |
| 77 | Khmer | 19 | [khmer/](khmer/README.md) |
| 78 | Afrikaans | 18.1 | [afrikaans/](afrikaans/README.md) |
| 79 | Nigerian Fulfulde | 18 | [nigerian-fulfulde/](nigerian-fulfulde/README.md) |
| 80 | Maithili | 17.6 | [maithili/](maithili/README.md) |

**Ranking source:** [Top 200 Languages (Ethnologue 2025)](https://www.jetpunk.com/user-quizzes/2026529/top-200-languages-ethnologue-2025), updated 10 September 2025. Estimates are index values, not exact current populations. Language-vs-dialect classification and second-language proficiency thresholds vary by source.
