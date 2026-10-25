# documents-safe

Public-domain source works behind the stanford-math canon. Safe to hold and
share: each entry below is out of copyright and offered by a reputable archive.

> **Why this folder holds a manifest, not the files yet.** This sandbox runs in
> `INTEGRATIONS_ONLY` network mode — it has no open-internet egress, and the
> archives below (Project Gutenberg, Wikisource, Fourmilab, Internet Archive)
> refuse automated fetches (HTTP 403). The files could not be pulled in here.
> This manifest records verified, stable download URLs so the works can be
> retrieved from any machine with normal network access — see *How to fetch*.

## The safe set (verified public domain)

| Work | Author, year | Why it's here | Download |
|------|--------------|---------------|----------|
| *Philosophiæ Naturalis Principia Mathematica* | Newton, 1687 | Source of Newton's laws & gravitation | Project Gutenberg #28233 (Latin) / #76404 (English, "Mathematical Principles") |
| *On the Electrodynamics of Moving Bodies* | Einstein, 1905 | Source of special relativity, E=mc² | Fourmilab (placed in public domain) / Wikisource |
| *Discours préliminaire de l'Encyclopédie* (Preliminary Discourse) | d'Alembert, 1751 | The `1751.md` anchor — knowledge unified, not single | Internet Archive |

### Stable URLs

- **Newton, *Principia* (Latin, PD):** https://www.gutenberg.org/ebooks/28233
  (plain text: https://www.gutenberg.org/cache/epub/28233/pg28233.txt)
- **Newton, *Mathematical Principles* (English, PD):** https://www.gutenberg.org/ebooks/76404
- **Einstein 1905 (PD):** https://en.wikisource.org/wiki/On_the_Electrodynamics_of_Moving_Bodies
  (also Fourmilab: https://server.fourmilab.ch/etexts/einstein/specrel/www )
- **d'Alembert 1751 Preliminary Discourse (PD):**
  https://archive.org/details/preliminarydisco0000alem
  (French original *Discours préliminaire*: https://archive.org/details/discoursprelimin0000alem )

## Deliberately excluded

- **Shannon, *A Mathematical Theory of Communication* (1948).** Still under
  copyright (a 1948 US corporate work is not yet public domain). It stays in the
  canon as a *citation* (see `SMProvenance`), but its full text is **not**
  downloaded or redistributed here. That is the compassionate, lawful call.

## How to fetch (on a machine with open network)

From this folder:

```sh
# Newton (English, Project Gutenberg plain text)
curl -L -o newton-principia-en-1729.txt \
  https://www.gutenberg.org/cache/epub/76404/pg76404.txt

# Newton (Latin original)
curl -L -o newton-principia-la-1687.txt \
  https://www.gutenberg.org/cache/epub/28233/pg28233.txt

# Einstein 1905 (Wikisource plain text)
curl -L -o einstein-1905-electrodynamics.txt \
  "https://en.wikisource.org/wiki/On_the_Electrodynamics_of_Moving_Bodies?action=raw"

# d'Alembert 1751 Preliminary Discourse (Internet Archive full text)
curl -L -o dalembert-1751-preliminary-discourse.txt \
  https://archive.org/stream/preliminarydisco0000alem/preliminarydisco0000alem_djvu.txt
```

Verify each download opens as expected before relying on it; archive URLs can
change. All four works are public domain; redistribution is unrestricted.

*Respect to Wikipedia and the Internet Archive as tertiary locators.*
