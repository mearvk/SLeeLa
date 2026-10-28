#!/usr/bin/env python3
"""SLeeLa-aware antivirus systemctl wrapper.

This controller focuses on the repository's Sleelavirin components and gives a
small systemd-friendly interface around them. It is intentionally conservative:
all actions that can affect system state require explicit command execution and
systemd availability checks.
"""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Optional


DEFAULT_SERVICE_NAME = "sleela-antivirus.service"
DEFAULT_POLICY_NAME = "antivirus-policy.example.json"
DEFAULT_POLICY_DIR = Path("lib/antivirus")
DEFAULT_ENV_EXAMPLE = "antivirus/systemd/sleela-antivirus.env.example"
DEFAULT_ENV_PATH = Path("/etc/sleela-antivirus/sleela-antivirus.env")
REQUIRED_ENV_VARS = [
    "SLEELA_ANTIVIRUS_ENGINE",
    "SLEELA_ANTIVIRUS_ROOT",
    "SLEELA_ANTIVIRUS_REPORT",
    "SLEELA_ANTIVIRUS_QUARANTINE",
    "SLEELA_ANTIVIRUS_DB",
    "SLEELA_ANTIVIRUS_NATIVE",
]


def repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


def default_policy_path() -> Path:
    return repo_root() / DEFAULT_POLICY_DIR / DEFAULT_POLICY_NAME


def default_env_example_path() -> Path:
    return repo_root() / DEFAULT_ENV_EXAMPLE


def print_json(data: object) -> None:
    print(json.dumps(data, indent=2, sort_keys=True))


def run_cmd(cmd: Iterable[str], check: bool = False) -> subprocess.CompletedProcess:
    result = subprocess.run(list(cmd), text=True, capture_output=True)
    if check and result.returncode != 0:
        raise subprocess.CalledProcessError(result.returncode, cmd, output=result.stdout, stderr=result.stderr)
    return result


def systemctl_available() -> bool:
    return shutil.which("systemctl") is not None


def systemctl_cmd(service: str, action: str, extra_args: Optional[List[str]] = None) -> List[str]:
    cmd = ["systemctl", action, service]
    if extra_args:
        cmd.extend(extra_args)
    return cmd


def read_policy(path: Path) -> dict:
    if not path.exists():
        return {
            "schema": "sleela-antivirus-policy-1",
            "scope": "LOCAL",
            "engine": "clamav",
            "preferDaemon": True,
            "allowOneShotFallback": True,
            "systemScanEnabled": False,
            "quarantineEnabled": False,
            "followSymbolicLinks": False,
            "maxFileSizeBytes": 268435456,
            "timeoutSeconds": 120,
            "maxDepth": 64,
            "onAccess": {"enabled": False, "mode": "notify-only", "platformSpecific": True},
            "logging": {"enabled": True, "includeTargetPath": True, "redactUserHomeInExport": True},
        }
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def read_env_file(path: Path) -> Dict[str, str]:
    result: Dict[str, str] = {}
    if not path.exists():
        return result
    for raw_line in path.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        if "=" not in line:
            continue
        key, value = line.split("=", 1)
        result[key.strip()] = value.strip()
    return result


def install_file(source: Path, target: Path, overwrite: bool = False) -> bool:
    if target.exists() and not overwrite:
        return False
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(source.read_text(encoding="utf-8"), encoding="utf-8")
    return True


def path_is_inside_repo(path_value: str) -> bool:
    try:
        return Path(path_value).resolve().is_relative_to(repo_root().resolve())
    except Exception:
        return False


def validate_host(service_name: str, env_path: Path, policy_path: Path) -> Dict[str, object]:
    report: Dict[str, object] = {
        "service": service_name,
        "systemd_available": systemctl_available(),
        "policy_path": str(policy_path),
        "policy_exists": policy_path.exists(),
        "env_path": str(env_path),
        "env_exists": env_path.exists(),
        "errors": [],
        "warnings": [],
        "missing_env_vars": [],
        "path_checks": {},
        "env_values": {},
    }

    env_values = read_env_file(env_path)
    report["env_values"] = env_values

    for name in REQUIRED_ENV_VARS:
        if name not in env_values or not str(env_values[name]).strip():
            report["missing_env_vars"].append(name)
            report["errors"].append(f"missing required env var: {name}")
        else:
            report["path_checks"][name] = env_values[name]

    for name, value in env_values.items():
        if name in {"SLEELA_ANTIVIRUS_ROOT", "SLEELA_ANTIVIRUS_REPORT", "SLEELA_ANTIVIRUS_QUARANTINE", "SLEELA_ANTIVIRUS_DB", "SLEELA_ANTIVIRUS_NATIVE"}:
            p = Path(value)
            if not p.is_absolute():
                report["warnings"].append(f"{name} is not an absolute path: {value}")
            if path_is_inside_repo(value):
                report["warnings"].append(f"{name} resolves inside the repository: {value}")
            if name in {"SLEELA_ANTIVIRUS_REPORT", "SLEELA_ANTIVIRUS_QUARANTINE"}:
                parent = p.parent
                if parent.exists() and not parent.is_dir():
                    report["errors"].append(f"{name} parent is not a directory: {parent}")

    if not report["policy_exists"]:
        report["warnings"].append("policy file does not exist; using the repository example fallback only")

    if not report["systemd_available"]:
        report["warnings"].append("systemctl is not available; service lifecycle operations are unavailable on this host")

    if not env_path.exists():
        report["warnings"].append("host environment file is absent; install it before enabling the service")

    return report


def status_payload(service: str, policy_path: Path, env_path: Optional[Path] = None) -> dict:
    payload = {
        "service": service,
        "systemd": systemctl_available(),
        "policy_path": str(policy_path),
        "policy": read_policy(policy_path),
        "env_file_path": str(env_path or DEFAULT_ENV_PATH),
        "env_file_exists": bool((env_path or DEFAULT_ENV_PATH).exists()),
        "env_file_values": read_env_file(env_path or DEFAULT_ENV_PATH),
    }
    if not systemctl_available():
        payload["state"] = "systemd-unavailable"
        return payload

    for action in ("is-enabled", "is-active"):
        result = run_cmd(systemctl_cmd(service, action), check=False)
        payload[f"{action.replace('-', '_')}"] = result.stdout.strip() if result.stdout else "unknown"
        payload[f"{action.replace('-', '_')}_rc"] = result.returncode

    result = run_cmd(["systemctl", "show", service, "--property=ActiveState,UnitFileState,FragmentPath,Description"], check=False)
    payload["systemctl_show"] = result.stdout.strip() if result.stdout else ""
    payload["state"] = payload.get("is_active", "unknown").strip() or "unknown"
    return payload


def handle_status(args: argparse.Namespace) -> int:
    service = args.service_name
    policy_path = args.policy
    payload = status_payload(service, policy_path, args.env_file)
    if args.json:
        print_json(payload)
        return 0
    print(f"Service: {service}")
    print(f"systemd available: {payload['systemd']}")
    print(f"Policy: {payload['policy_path']}")
    print(f"Environment file: {payload['env_file_path']}")
    print(f"state: {payload['state']}")
    if payload['systemd']:
        print(f"enabled: {payload.get('is_enabled', 'unknown')}")
        print(f"active: {payload.get('is_active', 'unknown')}")
    return 0


def handle_start(args: argparse.Namespace) -> int:
    service = args.service_name
    if not systemctl_available():
        print(f"systemctl is not available; cannot start {service}.", file=sys.stderr)
        return 2
    cmd = ["systemctl", "start", service]
    result = run_cmd(cmd, check=False)
    if result.returncode != 0:
        sys.stderr.write(result.stderr or result.stdout or f"Failed to start {service}\n")
        return result.returncode
    print(f"Started {service}")
    return 0


def handle_stop(args: argparse.Namespace) -> int:
    service = args.service_name
    if not systemctl_available():
        print(f"systemctl is not available; cannot stop {service}.", file=sys.stderr)
        return 2
    result = run_cmd(["systemctl", "stop", service], check=False)
    if result.returncode != 0:
        sys.stderr.write(result.stderr or result.stdout or f"Failed to stop {service}\n")
        return result.returncode
    print(f"Stopped {service}")
    return 0


def handle_restart(args: argparse.Namespace) -> int:
    service = args.service_name
    if not systemctl_available():
        print(f"systemctl is not available; cannot restart {service}.", file=sys.stderr)
        return 2
    result = run_cmd(["systemctl", "restart", service], check=False)
    if result.returncode != 0:
        sys.stderr.write(result.stderr or result.stdout or f"Failed to restart {service}\n")
        return result.returncode
    print(f"Restarted {service}")
    return 0


def handle_enable(args: argparse.Namespace) -> int:
    service = args.service_name
    if not systemctl_available():
        print(f"systemctl is not available; cannot enable {service}.", file=sys.stderr)
        return 2
    cmd = ["systemctl", "enable", service]
    if args.now:
        cmd.append("--now")
    result = run_cmd(cmd, check=False)
    if result.returncode != 0:
        sys.stderr.write(result.stderr or result.stdout or f"Failed to enable {service}\n")
        return result.returncode
    print(f"Enabled {service}")
    return 0


def handle_disable(args: argparse.Namespace) -> int:
    service = args.service_name
    if not systemctl_available():
        print(f"systemctl is not available; cannot disable {service}.", file=sys.stderr)
        return 2
    result = run_cmd(["systemctl", "disable", service], check=False)
    if result.returncode != 0:
        sys.stderr.write(result.stderr or result.stdout or f"Failed to disable {service}\n")
        return result.returncode
    print(f"Disabled {service}")
    return 0


def handle_policy(args: argparse.Namespace) -> int:
    policy = read_policy(args.policy)
    if args.json:
        print_json(policy)
        return 0
    print_json(policy)
    return 0


def handle_scan(args: argparse.Namespace) -> int:
    repo = repo_root()
    scan = repo / "antivirus" / "tools" / "sleelavirin_scan.py"
    if not scan.exists():
        print(f"Missing scanner script: {scan}", file=sys.stderr)
        return 2

    cmd = [sys.executable, str(scan), "--engine", args.engine]
    if args.database:
        cmd.extend(["--database", str(args.database)])
    if args.report_json:
        cmd.extend(["--report-json", str(args.report_json)])
    if args.quarantine_dir:
        cmd.extend(["--quarantine-dir", str(args.quarantine_dir)])
    if args.max_depth is not None:
        cmd.extend(["--max-depth", str(args.max_depth)])
    if args.jobs is not None:
        cmd.extend(["--jobs", str(args.jobs)])
    if args.timeout is not None:
        cmd.extend(["--scan-timeout", str(args.timeout)])
    if args.max_file_size is not None:
        cmd.extend(["--max-file-size", str(args.max_file_size)])
    if args.exclude:
        for value in args.exclude:
            cmd.extend(["--exclude", value])
    if args.extension:
        for value in args.extension:
            cmd.extend(["--extension", value])
    if args.follow_symlinks:
        cmd.append("--follow-symlinks")
    cmd.extend(args.paths)

    result = run_cmd(cmd, check=False)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    return result.returncode


def handle_install_unit(args: argparse.Namespace) -> int:
    source = Path(__file__).resolve().parents[1] / "systemd" / "sleela-antivirus.service"
    if not source.exists():
        print(f"Missing service template: {source}", file=sys.stderr)
        return 2
    target = Path(args.target_dir) / args.service_name
    if not install_file(source, target, overwrite=args.force):
        print(f"Unit file already exists at {target}; use --force to overwrite.", file=sys.stderr)
        return 1
    print(f"Installed service template at {target}")
    if systemctl_available():
        run_cmd(["systemctl", "daemon-reload"], check=False)
        print("systemctl daemon-reload completed")
    return 0


def handle_install_env(args: argparse.Namespace) -> int:
    example = default_env_example_path()
    if not example.exists():
        print(f"Missing environment example: {example}", file=sys.stderr)
        return 2
    target = Path(args.target_dir) / args.env_name
    if not install_file(example, target, overwrite=args.force):
        print(f"Environment file already exists at {target}; use --force to overwrite.", file=sys.stderr)
        return 1
    print(f"Installed environment file at {target}")
    return 0


def handle_install_host(args: argparse.Namespace) -> int:
    unit_args = argparse.Namespace(target_dir=args.target_unit_dir, service_name=args.service_name, force=args.force)
    unit_rc = handle_install_unit(unit_args)
    if unit_rc != 0:
        return unit_rc
    env_args = argparse.Namespace(target_dir=args.target_env_dir, env_name=args.env_name, force=args.force)
    return handle_install_env(env_args)


def handle_validate_host(args: argparse.Namespace) -> int:
    report = validate_host(args.service_name, args.env_file, args.policy)
    if args.json:
        print_json(report)
        return 0 if not report["errors"] else 2
    print(json.dumps(report, indent=2, sort_keys=True))
    return 0 if not report["errors"] else 2


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="SLeeLa-aware antivirus service controller.")
    parser.add_argument("--service-name", default=DEFAULT_SERVICE_NAME, help="systemd service name to control")
    parser.add_argument("--policy", type=Path, default=default_policy_path(), help="path to a SLeeLa antivirus policy JSON file")
    parser.add_argument("--env-file", type=Path, default=DEFAULT_ENV_PATH, help="host environment file for the service")

    subparsers = parser.add_subparsers(dest="command", required=True)

    status = subparsers.add_parser("status", help="show service and policy status")
    status.add_argument("--json", action="store_true")
    status.set_defaults(func=handle_status)

    validate_host_cmd = subparsers.add_parser("validate-host", help="validate host runtime values before enabling the service")
    validate_host_cmd.add_argument("--json", action="store_true")
    validate_host_cmd.set_defaults(func=handle_validate_host)

    start = subparsers.add_parser("start", help="start the SLeeLa antivirus service")
    start.set_defaults(func=handle_start)

    stop = subparsers.add_parser("stop", help="stop the SLeeLa antivirus service")
    stop.set_defaults(func=handle_stop)

    restart = subparsers.add_parser("restart", help="restart the SLeeLa antivirus service")
    restart.set_defaults(func=handle_restart)

    enable = subparsers.add_parser("enable", help="enable the SLeeLa antivirus service")
    enable.add_argument("--now", action="store_true")
    enable.set_defaults(func=handle_enable)

    disable = subparsers.add_parser("disable", help="disable the SLeeLa antivirus service")
    disable.set_defaults(func=handle_disable)

    policy = subparsers.add_parser("policy", help="print the active SLeeLa antivirus policy")
    policy.add_argument("--json", action="store_true")
    policy.set_defaults(func=handle_policy)

    scan = subparsers.add_parser("scan", help="run a local Sleelavirin scan against one or more paths")
    scan.add_argument("--engine", choices=("native", "clamav", "both"), default="native")
    scan.add_argument("--database", type=Path, help="native signature database to use")
    scan.add_argument("--report-json", type=Path, help="write a JSON scan report to this path")
    scan.add_argument("--quarantine-dir", type=Path, help="quarantine destination for detections")
    scan.add_argument("--max-depth", type=int, help="directory depth limit (0 means unlimited)")
    scan.add_argument("--jobs", type=int, help="native worker count")
    scan.add_argument("--timeout", type=int, dest="timeout", help="per-file engine timeout in seconds")
    scan.add_argument("--max-file-size", help="max file size, e.g. 100M")
    scan.add_argument("--exclude", action="append", default=[], help="repeatable glob to exclude")
    scan.add_argument("--extension", action="append", default=[], help="repeatable extension filter")
    scan.add_argument("--follow-symlinks", action="store_true")
    scan.add_argument("paths", nargs="+", help="paths to scan")
    scan.set_defaults(func=handle_scan)

    install = subparsers.add_parser("install-unit", help="install the unit file template to the target systemd directory")
    install.add_argument("--target-dir", default="/etc/systemd/system", help="target directory for the service unit")
    install.add_argument("--service-name", default=DEFAULT_SERVICE_NAME, help="service name to install")
    install.add_argument("--force", action="store_true", help="overwrite an existing unit file")
    install.set_defaults(func=handle_install_unit)

    install_env = subparsers.add_parser("install-env", help="install the example environment file to the target host directory")
    install_env.add_argument("--target-dir", default="/etc/sleela-antivirus", help="target directory for the service environment file")
    install_env.add_argument("--env-name", default="sleela-antivirus.env", help="environment file name")
    install_env.add_argument("--force", action="store_true", help="overwrite an existing environment file")
    install_env.set_defaults(func=handle_install_env)

    install_host = subparsers.add_parser("install-host", help="install both the service unit and the host environment file")
    install_host.add_argument("--target-unit-dir", default="/etc/systemd/system", help="target directory for the service unit")
    install_host.add_argument("--target-env-dir", default="/etc/sleela-antivirus", help="target directory for the service environment file")
    install_host.add_argument("--env-name", default="sleela-antivirus.env", help="environment file name")
    install_host.add_argument("--service-name", default=DEFAULT_SERVICE_NAME, help="service name to install")
    install_host.add_argument("--force", action="store_true", help="overwrite existing files")
    install_host.set_defaults(func=handle_install_host)

    return parser


def main(argv: Optional[List[str]] = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
