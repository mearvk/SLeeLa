# Social-Structure Canon

> 34 equations, sourced from `SMSocialCanon.sleela` -- the single
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

## The 34 equations

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
| 19 | Eigenvector centrality | `x = (1/lambda) A x` | network science | Phillip Bonacich | 1972 | Importance proportional to the importance of neighbors; a foundational network measure. |
| 20 | PageRank | `PR = (1-d)/N + d sum PR(j)/outdeg(j)` | network science | Page; Brin | 1998 | Ranks nodes by a random surfer's visit frequency; the algorithm behind web search. |
| 21 | Katz centrality | `x = (I - alpha A)^(-1) . 1` | network science | Leo Katz | 1953 | Credits all paths with distance-decay weighting; a classic influence measure. |
| 22 | Closeness centrality | `C(v) = (n - 1) / sum_u d(v,u)` | network science | Gert Sabidussi | 1966 | Importance by proximity to all others; measures efficient reach in a network. |
| 23 | Bonacich power centrality | `c = alpha (I - beta A)^(-1) A . 1` | network science | Phillip Bonacich | 1987 | Power depends on connections to the less-connected; captures bargaining advantage. |
| 24 | Freeman degree centralization | `C = sum (c_max - c_i) / max possible` | network science | Linton Freeman | 1979 | How unequal a network's degree distribution is; a measure of centralization. |
| 25 | Shannon diversity index | `H = -sum p_i ln p_i` | social measurement | Claude Shannon | 1948 | Information-entropy measure of variety and evenness; widely used across the social and life sciences. |
| 26 | Simpson diversity index | `D = 1 - sum p_i^2` | social measurement | Edward H. Simpson | 1949 | The probability two random members differ; a standard diversity measure. |
| 27 | Theil index (inequality) | `T = (1/N) sum (x_i/x-bar) ln(x_i/x-bar)` | social measurement | Henri Theil | 1967 | An entropy-based, decomposable inequality measure; separates within- and between-group inequality. |
| 28 | Atkinson inequality index | `A = 1 - (1/x-bar)(mean of x^(1-e))^(1/(1-e))` | social measurement | Anthony Atkinson | 1970 | Inequality with an explicit inequality-aversion parameter; grounded in social welfare. |
| 29 | Zipf's law (city sizes) | `size ~ 1 / rank` | urban economics | George Kingsley Zipf | 1949 | City size falls inversely with rank; a striking regularity of human settlement. |
| 30 | Gravity model of migration | `F_ij = G P_i P_j / d_ij^2` | spatial social science | Ernst Georg Ravenstein | 1885 | Flows grow with sizes and fall with distance; the base model of migration and trade. |
| 31 | Benford's law (leading digit) | `P(d) = log10(1 + 1/d)` | social statistics | Benford; Newcomb | 1938 | Leading digits follow a logarithmic distribution; used to detect fabricated data. |
| 32 | Sex ratio at birth (Fisher principle) | `parental investment drives a 1:1 ratio` | sociobiology | Ronald A. Fisher | 1930 | Equal parental investment drives a balanced sex ratio; a classic of evolutionary reasoning. |
| 33 | Metcalfe's law (network value) | `value ~ n^2` | network economics | Robert Metcalfe | 1980 | A network's value scales with the square of its users; a heuristic of network economics. |
| 34 | Granovetter threshold model | `collective action occurs above an activation threshold` | collective behavior | Mark Granovetter | 1978 | Collective action ignites once enough others act; explains riots, fads, and adoption. |

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

## Definitions & relevance (second-layer additions)

The entries below are the second-layer additions to this canon, organized by
type. Each gives a short **definition** (what the measure is and what its symbols
mean) and its **relevance** (why it is foundational to the study of social
structure). As everywhere in this archive, these describe **structure and
distribution — not persons**; they rank positions and quantify inequality, never
individuals.

### Network centrality

**Eigenvector centrality** — `x = (1/lambda) A x`. A node's importance is set
proportional to the summed importance of the nodes it connects to, so influence
flows recursively through the network rather than being counted one hop at a
time. It is foundational because it captures the intuition that being connected
to well-connected others matters more than raw tie count; it is the mathematical
ancestor of PageRank and a standard tool for finding structurally influential
positions.

**PageRank** — `PR = (1-d)/N + d sum PR(j)/outdeg(j)`. The stationary
distribution of a random surfer who follows links with probability `d` and
jumps to a random node otherwise; a node's score is the long-run fraction of
time the surfer spends there. It is relevant far beyond the web: as a damped,
teleporting eigenvector centrality it ranks importance in any directed network,
from citation graphs to transport systems.

**Katz centrality** — `x = (I - alpha A)^(-1) . 1`. Credits a node for every
path that reaches it, discounting longer paths by a factor `alpha` per step, so
distant connections still contribute but with diminishing weight. It matters
because it bridges local degree and global eigenvector centrality, and it stays
well-defined on directed, acyclic structures where plain eigenvector centrality
can fail.

**Closeness centrality** — `C(v) = (n - 1) / sum_u d(v,u)`. The inverse of a
node's average shortest-path distance to all others, so a high score means the
node can reach the rest of the network in few steps. It is the natural measure
of reach and efficiency — how quickly information or influence originating at a
position can spread to everyone else.

**Bonacich power centrality** — `c = alpha (I - beta A)^(-1) A . 1`. Generalizes
eigenvector centrality with a parameter `beta` whose sign decides whether being
tied to well-connected others is an advantage (positive) or, in bargaining, a
disadvantage (negative). Its relevance is that it formalizes the counterintuitive
idea that power can come from connecting to the *weakly* connected — central to
exchange and negotiation theory.

**Freeman degree centralization** — `C = sum (c_max - c_i) / (max possible)`. A
graph-level measure of how unequally centrality is distributed, comparing the
most central node to all the rest against the most star-like possible network.
It matters because it moves the analysis from individual positions to the whole
structure, quantifying whether a group is organized around a hub or spread
evenly.

### Diversity & inequality measurement

**Shannon diversity index** — `H = -sum p_i ln p_i`. Borrows information entropy
to score a population's variety: it is maximized when categories are many and
evenly represented, and zero when one category dominates. It is a foundational,
field-crossing measure of heterogeneity, used from ecology to the diversity of
occupations or opinions in a social group.

**Simpson diversity index** — `D = 1 - sum p_i^2`. The probability that two
members drawn at random fall in different categories, so higher means more mixed.
Its relevance is as an intuitive, bounded companion to Shannon entropy that
weights common categories more heavily — a standard concentration/diversity
gauge.

**Theil index (inequality)** — `T = (1/N) sum (x_i/x-bar) ln(x_i/x-bar)`. An entropy-based
measure of inequality whose signature property is **decomposability**: total
inequality splits exactly into a within-group part and a between-group part. That
makes it foundational for attributing inequality to structure — how much of it
lies between regions, sectors, or classes versus within them.

**Atkinson inequality index** — `A = 1 - (1/x-bar)(mean of x^(1-e))^(1/(1-e))`.
Measures inequality with an explicit **inequality-aversion** parameter `e`, so
the analyst states how much weight to place on the bottom of the distribution.
It matters because it grounds an inequality number in social-welfare theory
rather than in a purely descriptive statistic.

### Distributional & spatial laws

**Zipf's law (city sizes)** — `size ~ 1 / rank`. The population of the k-th
largest city is roughly the largest city's size divided by k, giving a strikingly
regular rank–size curve. It is relevant as one of the most robust empirical
regularities in the social sciences, constraining theories of how settlements
and firms grow.

**Gravity model of migration** — `F_ij = G P_i P_j / d_ij^2`. Flows between two
places rise with the product of their sizes and fall with the distance between
them, by analogy with Newtonian gravity. It is the workhorse model of migration
and trade, still the baseline against which spatial-interaction theories are
tested.

**Benford's law (leading digit)** — `P(d) = log10(1 + 1/d)`. In many
naturally-occurring datasets the leading digit is 1 far more often than 9,
following a logarithmic law. Its relevance is both theoretical (scale invariance)
and practical: deviations from it are a standard red flag in fraud and
data-integrity auditing.

### Sociobiology & network economics

**Sex ratio at birth (Fisher principle)** — equal parental investment drives the
population toward a 1:1 ratio. Fisher's argument is that whenever one sex is
rarer, parents producing it gain a reproductive advantage until balance returns.
It matters as an early, elegant equilibrium argument that seeded evolutionary
game theory and the economics of investment under frequency dependence.

**Metcalfe's law (network value)** — `value ~ n^2`. The usefulness of a
communication network grows roughly with the square of its users, because the
number of possible pairwise connections does. It is relevant as a heuristic for
network effects and critical mass, widely invoked (and debated) in the economics
of platforms and standards.

### Collective behavior

**Granovetter threshold model** — collective action ignites once the fraction of
others already acting crosses an individual's activation threshold. Because
thresholds vary across people, a population's *distribution* of thresholds — not
any single person — determines whether a riot, fad, or adoption cascade takes
off. It is foundational to understanding tipping points and why outwardly similar
crowds behave very differently.
