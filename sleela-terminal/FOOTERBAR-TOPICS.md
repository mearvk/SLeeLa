# SleelaTerminal Footerbar Topic Catalog

The footerbar topic selector accepts exactly these 100 normalized terms. Topic terms are single tokens so they can be entered directly at the shell prompt.

## Usage

- `footerbar chemistry` — enable a chemistry RSS news search.
- `footerbar pharmacology` — replace the selection with pharmacology.
- `footerbar chemistry pharmacology` — combine multiple topics in one feed.
- `footerbar ai-research climate-science` — combine any supported terms.
- `footerbar status` — show the current selection.
- `footerbar list` — print the supported catalog.
- `footerbar clear` — disable the feed.

Selections are saved to `$XDG_CONFIG_HOME/sleela/footerbar-topics.conf`, or `~/.config/sleela/footerbar-topics.conf` when XDG_CONFIG_HOME is unset. The GUI checks for selection changes every 30 seconds and refreshes Google News RSS results every 15 minutes. It requires network access and the `curl` executable. If a feed is unavailable, the footer shows a concise diagnostic instead of blocking the terminal.

## Supported topics (100)

1. `academia`
2. `agriculture`
3. `ai-research`
4. `anatomy`
5. `anthropology`
6. `archaeology`
7. `astronomy`
8. `astrophysics`
9. `artificial-intelligence`
10. `behavioral-science`
11. `biochemistry`
12. `biodiversity`
13. `bioinformatics`
14. `biology`
15. `biomedical-engineering`
16. `biophysics`
17. `biotechnology`
18. `botany`
19. `business`
20. `cancer-research`
21. `chemistry`
22. `climate-science`
23. `cognitive-science`
24. `computational-biology`
25. `computer-science`
26. `conservation`
27. `data-science`
28. `dentistry`
29. `developmental-biology`
30. `drug-discovery`
31. `ecology`
32. `economics`
33. `education`
34. `electrical-engineering`
35. `energy`
36. `engineering`
37. `environmental-science`
38. `epidemiology`
39. `ethics`
40. `evolution`
41. `finance`
42. `food-science`
43. `forensic-science`
44. `genetics`
45. `geochemistry`
46. `geography`
47. `geology`
48. `global-health`
49. `health-policy`
50. `history`
51. `immunology`
52. `infectious-disease`
53. `information-security`
54. `materials-science`
55. `mathematics`
56. `mechanical-engineering`
57. `medicine`
58. `microbiology`
59. `molecular-biology`
60. `nanotechnology`
61. `neuroscience`
62. `nuclear-physics`
63. `nutrition`
64. `oceanography`
65. `oncology`
66. `optics`
67. `paleontology`
68. `pharmacology`
69. `pharmacy`
70. `philosophy`
71. `physics`
72. `physiology`
73. `plant-science`
74. `political-science`
75. `psychology`
76. `public-health`
77. `quantum-computing`
78. `quantum-physics`
79. `robotics`
80. `science-policy`
81. `software-engineering`
82. `statistics`
83. `sustainability`
84. `systems-biology`
85. `toxicology`
86. `veterinary-medicine`
87. `virology`
88. `volcanology`
89. `water-resources`
90. `wildlife`
91. `anxiety-research`
92. `clinical-trials`
93. `digital-health`
94. `medical-devices`
95. `mental-health`
96. `pharmacogenomics`
97. `regenerative-medicine`
98. `stem-cells`
99. `surgery`
100. `vaccinology`
