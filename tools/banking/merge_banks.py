import re
from pathlib import Path

banks = Path('BANKS.md').read_text(encoding='utf-8')
banks2 = Path('BANKS2.md').read_text(encoding='utf-8')

currency = {}
for m in re.finditer(r'^\|\s*(\d{3})\s*\|\s*([^|]+?)\s*\|\s*([^|]+?)\s*\|\s*$', banks2, re.M):
    currency[m.group(1)] = m.group(3).strip()

# Only modify the generated national economic table.
pat = re.compile(r'(\| ID \| Country/Jurisdiction \| ISO \| Currency \| GDP \| GDP/Capita \| Inflation \| Trade/GDP \| World Bank Income \| Banking \| Status \|\n\|---\|---\|---\|---\|---:.*?\n)(?=\n|$)', re.S)
m = pat.search(banks)
if not m:
    raise SystemExit('Could not locate national economic table')

table = m.group(1)
lines = table.splitlines()
out = []
for line in lines:
    mm = re.match(r'^\|\s*(\d{3})\s*\|', line)
    if mm and mm.group(1) in currency:
        cols = line.split('|')
        # ['', ID, Country, ISO, Currency, GDP, ...]
        if len(cols) >= 6:
            cols[4] = f' {currency[mm.group(1)]} '
            line = '|'.join(cols)
    out.append(line)

newtable = '\n'.join(out) + '\n'
Path('BANKS.md').write_text(banks[:m.start(1)] + newtable + banks[m.end(1):], encoding='utf-8')
print(f'Merged {len(currency)} currency records into BANKS.md')
