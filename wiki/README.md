<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# wiki/

Source for the SLeeLa GitHub wiki. These pages are kept in-repo so they are
versioned and reviewable alongside the code; the GitHub wiki itself lives in a
separate `SLeeLa.wiki.git` repository.

## Pages

| File | Wiki page |
|---|---|
| `Home.md` | **Home** (landing page) |
| `Language-Reference.md` | Language Reference |
| `Nordshrift-Sheet-Reference.md` | Nordshrift Sheet Reference |
| `Writing-a-Subject.md` | Writing a Subject |

Inter-page links use GitHub wiki style (`[Text](Page-Name)`), so they resolve
directly once copied into the wiki.

## Publishing to the GitHub wiki

The wiki is a separate git repo. To publish:

```sh
# 1. Enable the wiki once via the repo Settings, and create any first page in
#    the GitHub UI so the wiki repo exists.
# 2. Clone it and copy these files in:
git clone https://github.com/mearvk/SLeeLa.wiki.git
cp wiki/*.md SLeeLa.wiki/
cd SLeeLa.wiki
git add . && git commit -m "Sync wiki from repo" && git push
```

The repository is authoritative; treat the wiki as a rendered copy.