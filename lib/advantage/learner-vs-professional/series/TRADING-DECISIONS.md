# Trading Decisions (Lessons 7 to 32)

Twenty-six trading decisions that keep the shape of lesson 6: the Purchase
Master runs the Sequitur onion, the student totals registered entries by hand,
the Instructor names every differing entry, and the student re-observes until
the two agree. Ties go to the earlier listed option (the tie norm).

All quotes are illustrative inputs in integer cents per lot, not market data.
Every lesson runs on the shared [`SLDecisionDesk`](./SLDecisionDesk.sleela)
(up to eight options). Lessons 7 to 22 have two to four options; lessons 23 to
32 have five to eight.

| # | folder | goods | options | student slip | master pick (total, cents) | tie at best | first pass |
|---|--------|-------|:------:|--------------|----------------------------|:-----------:|------------|
| 7 | [`07-trade-lumber`](./07-trade-lumber/DiTradeLumber.sleela) | Lumber | 2 | missed discount at Mill A | Mill A (83500) | no | picks differ |
| 8 | [`08-trade-cement`](./08-trade-cement/DiTradeCement.sleela) | Cement | 2 | fee counted twice at Plant B | Plant B (52500) | no | picks differ |
| 9 | [`09-trade-copper`](./09-trade-copper/DiTradeCopper.sleela) | Copper | 2 | discount sign flipped at Smelter A | Smelter A (307000) | no | picks differ |
| 10 | [`10-trade-steel`](./10-trade-steel/DiTradeSteel.sleela) | Steel | 3 | option not observed at Mill A | Mill C (118500) | no | same pick, entry differs |
| 11 | [`11-trade-coffee`](./11-trade-coffee/DiTradeCoffee.sleela) | Coffee | 2 | quote in dollars, not cents at Importer A | Importer B (27400) | no | picks differ |
| 12 | [`12-trade-cotton`](./12-trade-cotton/DiTradeCotton.sleela) | Cotton | 3 | no slip (clean) | Gin C (45400) | no | AGREE |
| 13 | [`13-trade-solar`](./13-trade-solar/DiTradeSolar.sleela) | Solar panels | 2 | missed discount at Supplier A | Supplier A (99000) | yes | picks differ |
| 14 | [`14-trade-lithium`](./14-trade-lithium/DiTradeLithium.sleela) | Lithium | 3 | fee counted twice at Refiner A | Refiner A (632000) | no | picks differ |
| 15 | [`15-trade-semiconductors`](./15-trade-semiconductors/DiTradeSemiconductors.sleela) | Semiconductors | 2 | discount sign flipped at Foundry B | Foundry B (207000) | no | picks differ |
| 16 | [`16-trade-server-racks`](./16-trade-server-racks/DiTradeServerRacks.sleela) | Server racks | 4 | option not observed at Builder C | Builder C (857000) | no | picks differ |
| 17 | [`17-trade-smartphones`](./17-trade-smartphones/DiTradeSmartphones.sleela) | Smartphones | 3 | quote in dollars, not cents at Distributor B | Distributor A (61500) | no | picks differ |
| 18 | [`18-trade-televisions`](./18-trade-televisions/DiTradeTelevisions.sleela) | Televisions | 4 | missed discount at Wholesaler C | Wholesaler C (32400) | no | picks differ |
| 19 | [`19-trade-wheat`](./19-trade-wheat/DiTradeWheat.sleela) | Wheat | 3 | no slip (clean) | Elevator A (21400) | yes | AGREE |
| 20 | [`20-trade-fuel`](./20-trade-fuel/DiTradeFuel.sleela) | Fuel | 3 | fee counted twice at Depot A | Depot A (75000) | no | picks differ |
| 21 | [`21-trade-freight`](./21-trade-freight/DiTradeFreight.sleela) | Freight | 3 | discount sign flipped at Carrier C | Carrier C (14900) | no | picks differ |
| 22 | [`22-trade-tie-capstone`](./22-trade-tie-capstone/DiTradeTieCapstone.sleela) | Tie capstone | 4 | option not observed at Vendor A | Vendor A (495000) | yes | picks differ |
| 23 | [`23-trade-rice`](./23-trade-rice/DiTradeRice.sleela) | Rice | 5 | discount filed under the next option at Miller B | Miller B (52100) | no | picks differ |
| 24 | [`24-trade-sugar`](./24-trade-sugar/DiTradeSugar.sleela) | Sugar | 5 | missed discount at Refinery D | Refinery D (41600) | no | picks differ |
| 25 | [`25-trade-aluminum`](./25-trade-aluminum/DiTradeAluminum.sleela) | Aluminum | 6 | fee counted twice at Smelter A | Smelter A (54800) | no | picks differ |
| 26 | [`26-trade-zinc`](./26-trade-zinc/DiTradeZinc.sleela) | Zinc | 6 | discount filed under the next option at Miner E | Miner B (45000) | no | picks differ |
| 27 | [`27-trade-nickel`](./27-trade-nickel/DiTradeNickel.sleela) | Nickel | 5 | discount sign flipped at Refiner C | Refiner C (50200) | no | picks differ |
| 28 | [`28-trade-rubber`](./28-trade-rubber/DiTradeRubber.sleela) | Rubber | 7 | option not observed at Planter F | Planter A (52400) | no | same pick, entry differs |
| 29 | [`29-trade-glass`](./29-trade-glass/DiTradeGlass.sleela) | Glass | 6 | quote in dollars, not cents at Maker B | Maker E (48700) | no | picks differ |
| 30 | [`30-trade-paper`](./30-trade-paper/DiTradePaper.sleela) | Paper | 8 | no slip (clean) | Mill D (47000) | yes | AGREE |
| 31 | [`31-trade-resin`](./31-trade-resin/DiTradeResin.sleela) | Plastic resin | 8 | discount filed under the next option at Compounder H | Compounder H (50600) | no | picks differ |
| 32 | [`32-trade-eight-way-capstone`](./32-trade-eight-way-capstone/DiTradeEightWayCapstone.sleela) | Eight-way capstone | 8 | missed discount at Vendor A | Vendor A (54100) | yes | picks differ |

## Slip kinds

| kind | slip | what the Instructor reports |
|------|------|-----------------------------|
| 1 | missed discount | "student did not register it, master has -N" |
| 2 | fee counted twice | "fee again: student only, master has no such entry" |
| 3 | discount sign flipped | "student +N, master -N -> re-observe" |
| 4 | option not observed | "master only, student did not observe it" |
| 5 | quote in dollars, not cents | "student N, master N00 -> re-observe" |
| 6 | discount filed under the next option | "student filed it under X, master files it under Y" |

## Notes per lesson

- Lesson 10 (Steel): Right pick, wrong observation: the Instructor still refuses the work.
- Lesson 12 (Cotton): A clean decision: the first pass already agrees.
- Lesson 13 (Solar panels): The true totals tie, so the tie norm picks Supplier A; the slip flips the student.
- Lesson 16 (Server racks): Four options; the cheapest one is the one the student missed.
- Lesson 19 (Wheat): A clean decision with a tie between A and B: the tie norm picks Elevator A.
- Lesson 22 (Tie capstone): A and C tie at the best total. The tie norm picks Vendor A; the student missed A and lands on C, which has the same total but is the wrong pick.
- Lesson 23 (Rice): The discount is real but filed under the next supplier: the totals of two options move.
- Lesson 26 (Zinc): The discount is real but filed under the next miner: the totals of two options move.
- Lesson 28 (Rubber): Right pick, wrong observation: the Instructor still refuses the work.
- Lesson 30 (Paper): A clean decision over eight options with a tie at the best total: the tie norm picks the earliest tied vendor.
- Lesson 31 (Plastic resin): The discount of the last option is filed under the first option (the list wraps round).
- Lesson 32 (Eight-way capstone): Eight options, a tie at the best total, and a missed discount on the earliest tied option: the student lands on the later tied option.

## What to look for

- A wrong entry can flip the pick (most lessons), or leave the pick right with
  the observation wrong (lessons 10 and 28). The Instructor refuses both.
- A misfiled entry (kind 6) keeps the right value but the wrong carrier. Two
  option totals move at once, which is why one-place-only matters.
- A tie is decided by the norm, never by luck (lessons 13, 19, 22, 30, 32).
- The clean decisions (lessons 12, 19 and 30) show that agreement on the first
  pass needs no second pass.
