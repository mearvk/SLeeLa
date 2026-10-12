import argparse
from pathlib import Path
import re
import sys

SHA256 = re.compile(r'^[0-9a-fA-F]{64}$')


def main():
    parser = argparse.ArgumentParser(description='Import compatible extracted ClamAV SHA-256 .hsb records.')
    parser.add_argument('--source', type=Path)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--check', type=Path)
    args = parser.parse_args()
    if args.check:
        try:
            lines = args.check.read_text(encoding='utf-8').splitlines()
        except OSError as exc:
            print(f'ERROR: {exc}', file=sys.stderr)
            return 2
        bad = 0
        for number, line in enumerate(lines, 1):
            line = line.strip()
            if line and not line.startswith('#') and not SHA256.fullmatch(line.split(None, 1)[0]):
                print(f'INVALID {args.check}:{number}: expected 64 hex characters', file=sys.stderr)
                bad += 1
        print('VALID' if not bad else 'INVALID', args.check)
        return 0 if not bad else 2
    if not args.source or not args.output:
        parser.error('provide --source and --output, or use --check')
    if not args.source.is_dir():
        parser.error('source must be a directory of extracted text signature files')
    entries, skipped = {}, 0
    for path in sorted(args.source.rglob('*.hsb')):
        try:
            lines = path.read_text(encoding='utf-8', errors='replace').splitlines()
        except OSError:
            skipped += 1
            continue
        for line in lines:
            fields = line.strip().split(':')
            if not line.strip() or line.lstrip().startswith('#'):
                continue
            if len(fields) < 3 or not SHA256.fullmatch(fields[0].strip()):
                skipped += 1
                continue
            try:
                int(fields[1])
            except ValueError:
                skipped += 1
                continue
            entries.setdefault(fields[0].lower(), set()).add(':'.join(fields[2:]).strip() or 'clamav-import')
    if not entries:
        print('ERROR: no compatible SHA-256 .hsb records; output left unchanged', file=sys.stderr)
        return 2
    args.output.parent.mkdir(parents=True, exist_ok=True)
    temp = args.output.with_suffix(args.output.suffix + '.tmp')
    with temp.open('w', encoding='utf-8', newline='\n') as out:
        out.write('# Imported compatible whole-file SHA-256 records from extracted ClamAV .hsb files.\n')
        out.write('# This subset does not represent all ClamAV detections.\n')
        for digest in sorted(entries):
            out.write(f"{digest} {', '.join(sorted(entries[digest]))}\n")
    temp.replace(args.output)
    print(f'IMPORTED {len(entries)} unique SHA-256 signatures to {args.output}; skipped {skipped} records')
    return 0

if __name__ == '__main__':
    raise SystemExit(main())
