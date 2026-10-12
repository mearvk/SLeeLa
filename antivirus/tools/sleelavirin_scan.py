import argparse
import fnmatch
import os
from pathlib import Path
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed


def size_arg(value):
    value = value.strip().lower()
    scale = 1
    if value.endswith('k'):
        scale, value = 1024, value[:-1]
    elif value.endswith('m'):
        scale, value = 1024**2, value[:-1]
    elif value.endswith('g'):
        scale, value = 1024**3, value[:-1]
    try:
        number = int(value)
        if number < 0: raise ValueError()
        return number * scale
    except ValueError as exc:
        raise argparse.ArgumentTypeError('use a non-negative byte count or K/M/G suffix') from exc


def enumerate_files(inputs, depth_limit, follow, excludes, extensions, size_limit):
    output, seen = [], set()
    for item in inputs:
        root = Path(item).expanduser()
        if not root.exists():
            print(f'ERROR path does not exist: {root}', file=sys.stderr)
            continue
        if root.is_file(): candidates = [root]
        elif root.is_dir():
            candidates = []
            root_depth = len(root.resolve().parts)
            for current, dirs, names in os.walk(root, followlinks=follow):
                here = Path(current)
                depth = len(here.resolve().parts) - root_depth
                dirs[:] = sorted(d for d in dirs if (follow or not (here / d).is_symlink()) and not any(fnmatch.fnmatch(str(here / d), p) for p in excludes))
                if depth_limit and depth >= depth_limit: dirs[:] = []
                candidates.extend(here / name for name in sorted(names))
        else: continue
        for path in candidates:
            try:
                if not path.is_file() or (path.is_symlink() and not follow): continue
                key = str(path.resolve())
                if key in seen or any(fnmatch.fnmatch(str(path), p) or fnmatch.fnmatch(path.name, p) for p in excludes): continue
                if extensions and path.suffix.lower() not in extensions: continue
                if path.stat().st_size > size_limit:
                    print(f'SKIPPED_TOO_LARGE {path}')
                    continue
                seen.add(key); output.append(path)
            except OSError as exc: print(f'ERROR {path}: {exc}', file=sys.stderr)
    return output


def native_scan(executable, database, path):
    try:
        result = subprocess.run([str(executable), '--database', str(database), str(path)], capture_output=True, text=True)
        output = (result.stdout + result.stderr).strip()
        if result.returncode == 0: return 'NO_MATCH', path, output or f'NO_MATCH {path}'
        if result.returncode == 1 and 'DETECTED ' in output: return 'DETECTED', path, output
        return 'ERROR', path, output or f'ERROR {path} exit={result.returncode}'
    except OSError as exc: return 'ERROR', path, f'ERROR {path}: {exc}'


def main():
    package = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser(description='Native-first Sleelavirin scanner with optional ClamAV secondary engine.')
    parser.add_argument('--engine', choices=('native', 'clamav', 'both'), default='native')
    parser.add_argument('--database', default=str(package / 'signatures' / 'native-sha256.txt'))
    parser.add_argument('--clamscan', default='clamscan')
    parser.add_argument('--max-depth', type=int, default=8, help='directory depth; 0 means unlimited')
    parser.add_argument('--jobs', type=int, default=1, help='concurrent native file scans')
    parser.add_argument('--max-file-size', type=size_arg, default=100*1024*1024, help='bytes or K/M/G; default 100M')
    parser.add_argument('--exclude', action='append', default=[], metavar='GLOB')
    parser.add_argument('--extension', action='append', default=[], metavar='.EXT')
    parser.add_argument('--follow-symlinks', action='store_true')
    parser.add_argument('--action', choices=('report', 'quarantine', 'stop'), default='report')
    parser.add_argument('--quarantine-dir', default=str(package / 'quarantine'))
    parser.add_argument('paths', nargs='+')
    args = parser.parse_args()
    if args.max_depth < 0 or args.jobs < 1: parser.error('--max-depth must be >= 0 and --jobs >= 1')
    extensions = {x.lower() if x.startswith('.') else '.'+x.lower() for x in args.extension}
    paths = enumerate_files(args.paths, args.max_depth, args.follow_symlinks, args.exclude, extensions, args.max_file_size)
    if not paths:
        print('ERROR: no eligible files found', file=sys.stderr); return 2
    exe = package / 'build' / ('sleelavirin-scan.exe' if os.name == 'nt' else 'sleelavirin-scan')
    found, errors = [], 0
    if args.engine in ('native', 'both'):
        database = Path(args.database).expanduser()
        if not exe.is_file() or not database.is_file():
            print('ERROR: native scanner or database missing; build with make -C antivirus and check --database', file=sys.stderr); return 2
        results = []
        with ThreadPoolExecutor(max_workers=args.jobs) as pool:
            futures = [pool.submit(native_scan, exe, database, path) for path in paths]
            for future in as_completed(futures):
                row = future.result(); results.append(row)
                if args.action == 'stop' and row[0] == 'DETECTED':
                    for other in futures: other.cancel()
                    break
        for verdict, path, message in sorted(results, key=lambda row: str(row[1])):
            print(message)
            if verdict == 'DETECTED': found.append(path)
            elif verdict == 'ERROR': errors += 1
    if args.engine in ('clamav', 'both') and not (args.action == 'stop' and found):
        clam = shutil.which(args.clamscan)
        if not clam: print(f'ERROR: ClamAV not found: {args.clamscan}', file=sys.stderr); return 2
        command = [clam, '--infected', '--no-summary', '--max-filesize', str(args.max_file_size), *[str(p) for p in paths]]
        try:
            result = subprocess.run(command, capture_output=True, text=True)
            if result.stdout: print(result.stdout, end='' if result.stdout.endswith('\n') else '\n')
            if result.stderr: print(result.stderr, file=sys.stderr)
            if result.returncode == 1:
                print('CLAMAV_DETECTION')
                found.extend(Path(line.split(': ', 1)[0]) for line in result.stdout.splitlines() if ' FOUND' in line)
            elif result.returncode == 0: print('CLAMAV_NO_DETECTION (not a guarantee that files are safe)')
            else: print(f'CLAMAV_ERROR exit={result.returncode}', file=sys.stderr); errors += 1
        except OSError as exc: print(f'ERROR unable to start ClamAV: {exc}', file=sys.stderr); errors += 1
    unique = list(dict.fromkeys(found))
    if args.action == 'quarantine' and unique:
        destination = Path(args.quarantine_dir).expanduser()
        for path in unique:
            try:
                dest_real = destination.resolve()
                source_real = path.resolve()
                if source_real == dest_real or dest_real in source_real.parents:
                    print(f'ERROR unsafe quarantine destination for {path}', file=sys.stderr); errors += 1; continue
                destination.mkdir(parents=True, exist_ok=True)
                target = destination / path.name
                i = 1
                while target.exists(): target = destination / f'{path.stem}.{i}{path.suffix}'; i += 1
                shutil.move(str(path), str(target))
                print(f'QUARANTINED {path} -> {target}')
            except OSError as exc: print(f'ERROR quarantine failed for {path}: {exc}', file=sys.stderr); errors += 1
    return 1 if unique else (2 if errors else 0)

if __name__ == '__main__':
    raise SystemExit(main())
