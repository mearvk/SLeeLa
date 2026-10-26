# CSS source files — signatures &amp; provenance

Every CSS specification source archived under [`css/`](.) is listed here with its **file
signature** (byte size + SHA-256) and its **provenance** (the exact GitHub repo and commit it was
retrieved from). This matches the integrity convention used for the HTML sources in
[`../specifications/PROVENANCE.txt`](../specifications/PROVENANCE.txt).

> **On "file signature":** `.css`/spec files have no binary magic number — they are plain text,
> identified by the `text/css` MIME type (for stylesheets) and here by a cryptographic
> **SHA-256** digest, which uniquely fingerprints the exact bytes archived. Verify any file with
> `sha256sum <file>` and compare against the hash below.

Retrieved 2026-10-25 via the GitHub API (the sandbox is `INTEGRATIONS_ONLY`; `w3.org` is
unreachable directly, so sources come from GitHub mirrors of the W3C trees).

## Sources

| File | Size (bytes) | SHA-256 |
|------|-------------:|---------|
| `css1-1996/REC-CSS1-19961217.html` | 186630 | `0342a4d82cda9d161ea6572d0d91af4aa50e23d250f38111c7574597b814815f` |
| `css2.1-2011/CSS21-Overview.bs` | 644776 | `60dc5b8df0d1b97c82998bd05016ac4ec3146a160d79017071c18df41309186e` |
| `snapshot/CSS-Snapshot-2023.bs` | 43550 | `0db20e614c1fe11a11ebefc296bfddf2ab3c966b9b723281b18adde2e70bae02` |
| `css3-modules/selectors-3.src.html` | 108217 | `5260fb58130a37a9d5cb9894b2f1467439556098f6b7f6c1bb6196340fbd460b` |
| `css3-modules/css-color-3.html` | 170820 | `4cfdd10ea66d4c826b376cfd78f35096b7f4a27c8687bf306f8c1aae5872ec7a` |
| `css3-modules/css-backgrounds-3.bs` | 166998 | `5119e57464695b40659e52bb19dbab978f3440c1665b63e952d158d0474ffb08` |
| `css3-modules/css-fonts-3.html` | 240516 | `b4f219d0592dd1b1a5973c15184206824cd357e6cc83ec79bbe46b8202d8fe9f` |
| `css3-modules/css-text-3.bs` | 317103 | `87014a5f761a435f769754e6ab62be43c80628c6fc7cee59671c810122a9de28` |
| `css3-modules/css-values-3.bs` | 102031 | `d62721722605033ddcf3b035fd75dc0e61262f1e0bbfc28aba3069c67cb31e29` |
| `css3-modules/mediaqueries-4.bs` | 102106 | `131735536a9fc2e0b3bb6d156e70e90c5d93023cb4a2517725425c3333e968a0` |
| `css3-modules/css-cascade-4.bs` | 67047 | `e5932127261f9cd5e0e900c9dcad978024cb44fedb77d27f53da7b17953da86f` |
| `css3-modules/css-flexbox-1.bs` | 302576 | `9045cf12ebac5c027cadaacab8b4e90fcc66657520eeaab140d50e51bd300ffa` |
| `css3-modules/css-grid-1.bs` | 328096 | `853e873678a599394d6c5b7f68a7bf6735d33c8e6b4d468e48bbe61670a17a3b` |
| `css3-modules/css-transitions-1.bs` | 68772 | `cd15f3d2d3d47e5696b135781c8c632803978db8388033edff1bc3374b6c2da3` |
| `css3-modules/css-animations-1.bs` | 54937 | `2727c211033d4568621965aa5b2b9f3873a49ab8ae85223e8272803f143a3ccf` |

## Provenance (repo + commit)

| File(s) | Repo | Commit | Upstream path |
|---------|------|--------|---------------|
| `css1-1996/REC-CSS1-19961217.html` | `slebok/zoo` | `e184d20e0ba77dd7ce7b9de3b5eabb196bdf3703` | `zoo/css/v1.0/19961217/fetched/REC-CSS1-961217.html` (mirror of `w3.org/TR/REC-CSS1`) |
| `css2.1-2011/CSS21-Overview.bs` | `w3c/csswg-drafts` | `8b82486fbc4ebb8ac59585ceb4b67306391915cd` | `css2/Overview.bs` |
| `snapshot/CSS-Snapshot-2023.bs` | `w3c/csswg-drafts` | `6bcffdcf4a12ae5d692ca9e371e34981112679b0` | `css-2023/Overview.bs` |
| all `css3-modules/*` | `w3c/csswg-drafts` | HEAD `e1bd39185acd650512f82e1eeb7b94da21b686c7` | `<module>/Overview.bs` or `Overview.src.html` / `Overview.html` |

`.bs` files are the W3C **Bikeshed** preprocessor source (the authoritative editable source for
each module); `.html` files are the generated single-page spec where no Bikeshed source is kept
in-repo (`css-color-3`, `css-fonts-3`), and `selectors-3.src.html` is that module's HTML source.
Nothing was edited; files are byte-for-byte as mirrored. Canonical upstream specs live under
`https://www.w3.org/TR/` (blocked from this environment).
