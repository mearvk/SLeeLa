# Social-Structure Canon

> 18 equations, sourced from `SMSocialCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

How social ties and social structure are actually measured in the social sciences -- network connection (density, clustering, centrality), the strength and overlap of ties (Granovetter), social capital (bonding vs bridging), structural holes, the human scale of stable relationships (Dunbar), the shape of real networks (small-world, scale-free, community structure), and the distribution of wealth (Gini, Lorenz, Pareto). These measures describe STRUCTURE -- ties and distributions -- and rank no person. Each row credits the scholar and year as the record gives them, with a one-line note on why it matters.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
A standing boundary for this canon: these are measures of **social structure** --
network ties and statistical distributions -- **not** scores or rankings of individual
people. Symbolic forms use plain-ASCII notation so they stay greppable and diff-friendly.
The **Why it matters** column states each result's significance in one faithful line.

## The 18 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Network density | `D = 2 L / (N (N - 1))` | social network analysis | classical graph theory | unknown | The fraction of possible ties that actually exist; the simplest measure of how connected a group is. |
| 2 | Local clustering coefficient | `C_i = 2 e_i / (k_i (k_i - 1))` | social network analysis | Watts & Strogatz | 1998 | How tightly a person's contacts know each other; captures close-knit, 'everyone-knows-everyone' structure. |
| 3 | Degree centrality | `C_D(v) = deg(v) / (N - 1)` | social network analysis | Linton Freeman | 1979 | How many direct ties a node has; the most basic measure of how connected an individual position is. |
| 4 | Betweenness centrality | `C_B(v) = sum_{s != v != t} sigma_st(v) / sigma_st` | social network analysis | Linton Freeman | 1977 | How often a node sits on the shortest path between others; identifies brokers and bridges. |
| 5 | Tie strength (strength of weak ties) | `strength ~ time + intimacy + reciprocity; weak ties bridge groups` | economic sociology | Mark Granovetter | 1973 | Granovetter's insight that weak ties (acquaintances) reach new information strong ties cannot -- key to how jobs and ideas spread. |
| 6 | Dyadic embeddedness (tie overlap) | `overlap(a,b) = \|N(a) cap N(b)\| / \|N(a) cup N(b)\|` | social network analysis | Granovetter (overlap principle) | 1973 | The more two people's networks overlap, the stronger their tie; the structural basis of the weak-ties argument. |
| 7 | Homophily index | `H = (same-group ties - expected) / (total - expected)` | sociology | McPherson; Smith-Lovin; Cook | 2001 | Measures the tendency for similar people to connect ('birds of a feather'); shapes the structure of every social network. |
| 8 | Social capital (bridging vs bonding) | `returns ~ f(bonding within-group, bridging across-group)` | sociology | Putnam; Coleman; Bourdieu | 2000 | The value in social ties: bonding ties hold a group together, bridging ties link groups -- both drive collective action. |
| 9 | Structural holes (constraint) | `C_i = sum_j (p_ij + sum_q p_iq p_qj)^2` | economic sociology | Ronald Burt | 1992 | Burt's measure of the advantage of bridging gaps between otherwise-disconnected groups; a source of opportunity and influence. |
| 10 | Dunbar's number | `N_stable ~ 150 (cognitive limit on stable ties)` | evolutionary anthropology | Robin Dunbar | 1992 | A cognitive ceiling of roughly 150 stable relationships; why communities and units tend to a human scale. |
| 11 | Preferential attachment (scale-free degree) | `P(k) ~ k^(-gamma)` | network science | Barabasi & Albert | 1999 | 'The rich get richer' in ties -- explains why a few hubs dominate real social and web networks. |
| 12 | Small-world path length | `L ~ ln(N) / ln(k)` | network science | Watts & Strogatz | 1998 | Why any two people are linked by only a few steps ('six degrees'); short paths despite local clustering. |
| 13 | Gini coefficient (wealth inequality) | `G = (sum_i sum_j \|x_i - x_j\|) / (2 N^2 mu)` | economics of distribution | Corrado Gini | 1912 | The standard single-number measure of how unequally wealth or income is distributed (0 = equal, 1 = all held by one). |
| 14 | Lorenz curve | `L(p) = (cumulative wealth of poorest p fraction)` | economics of distribution | Max O. Lorenz | 1905 | Plots cumulative share of wealth against share of population; the visual from which the Gini coefficient is computed. |
| 15 | Pareto (wealth) distribution | `P(X > x) ~ (x_m / x)^alpha` | economics of distribution | Vilfredo Pareto | 1896 | The heavy-tailed law behind the '80-20 rule' of wealth; the original model of concentration at the top. |
| 16 | Reciprocity (directed ties) | `r = (mutual dyads) / (total ties)` | social network analysis | standard SNA measure | unknown | How often ties are mutual rather than one-way; a basic indicator of balance and cohesion in a network. |
| 17 | Modularity (community structure) | `Q = sum_c (e_cc - a_c^2)` | network science | Newman & Girvan | 2004 | Measures how cleanly a network splits into communities; the basis of community-detection methods. |
| 18 | Assortativity coefficient | `r = Pearson correlation of degrees across edges` | network science | M. E. J. Newman | 2002 | Whether well-connected nodes link to other well-connected nodes; distinguishes social networks from technological ones. |

## Validate

These equations are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints every entry and the running total across
all canons):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Names, forms, discoverers, and years follow the standard social-science / network-science
record; the **Why it matters** notes summarize each result's established significance. Key
anchors: Granovetter (1973), Freeman (1977-79), Burt (1992), McPherson-Smith-Lovin-Cook
(2001), Watts-Strogatz (1998), Barabasi-Albert (1999), Newman-Girvan (2004), Gini (1912),
Lorenz (1905), Pareto (1896). See `SMProvenance.sleela` and the repo `README.md`. This
archive records and attributes published results; it does not originate, own, or certify
them.
