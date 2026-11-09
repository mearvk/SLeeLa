#!/usr/bin/env python3
"""Generate/install a conservative systemd unit for a trusted native executable."""
from __future__ import annotations
import argparse, os, pathlib, re, shutil, subprocess, sys, tempfile

NAME = re.compile(r"^[a-zA-Z0-9][a-zA-Z0-9_.@-]{0,127}$")

def systemd_quote(value: str) -> str:
    if "\n" in value or "\r" in value or "\0" in value:
        raise ValueError("systemd values cannot contain newline or NUL")
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"').replace("%", "%%") + '"'

def render(args: argparse.Namespace) -> str:
    if not NAME.fullmatch(args.name):
        raise ValueError("service name must contain only letters, digits, dot, underscore, @, or hyphen")
    exe = pathlib.Path(args.executable).expanduser().resolve(strict=True)
    if not exe.is_file() or not os.access(exe, os.X_OK):
        raise ValueError("executable must be an existing executable file")
    work = pathlib.Path(args.working_directory).expanduser().resolve(strict=True)
    if not work.is_dir():
        raise ValueError("working directory must be an existing directory")
    lines = [
        "[Unit]",
        f"Description={systemd_quote(args.description)}",
        "After=network.target",
        "",
        "[Service]",
        "Type=simple",
    ]
    if args.system:
        if not args.user:
            raise ValueError("--system requires an explicit --user account")
        if not re.fullmatch(r"[a-z_][a-z0-9_-]{0,31}", args.user):
            raise ValueError("invalid system service user name")
        lines.append(f"User={args.user}")
    lines.extend([
        f"WorkingDirectory={systemd_quote(str(work))}",
        f"ExecStart={systemd_quote(str(exe))}",
        "Restart=on-failure",
        "RestartSec=3s",
        "NoNewPrivileges=yes",
        "PrivateTmp=yes",
        "ProtectSystem=strict",
        "ProtectHome=read-only",
        "ProtectKernelTunables=yes",
        "ProtectKernelModules=yes",
        "ProtectControlGroups=yes",
        "LockPersonality=yes",
        "RestrictSUIDSGID=yes",
        "SystemCallArchitectures=native",
    ])
    if args.writable_path:
        for item in args.writable_path:
            p = pathlib.Path(item).expanduser().resolve(strict=True)
            if not p.is_dir():
                raise ValueError(f"writable path is not a directory: {p}")
            lines.append(f"ReadWritePaths={systemd_quote(str(p))}")
    lines.extend(["", "[Install]", "WantedBy=default.target" if not args.system else "WantedBy=multi-user.target", ""])
    return "\n".join(lines)

def main() -> int:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--name", required=True, help="unit name without .service")
    p.add_argument("--executable", required=True, help="absolute or resolvable executable path")
    p.add_argument("--working-directory", default=".")
    p.add_argument("--description", default="SLeeLa native workload")
    p.add_argument("--user", help="unprivileged account for a system unit")
    p.add_argument("--writable-path", action="append", default=[], help="explicit writable directory; repeatable")
    p.add_argument("--output", help="write rendered unit to this file; otherwise stdout")
    p.add_argument("--install", action="store_true", help="install a user or system unit")
    p.add_argument("--system", action="store_true", help="install into /etc/systemd/system (requires root and --user)")
    p.add_argument("--enable", action="store_true", help="daemon-reload and enable after installation")
    p.add_argument("--start", action="store_true", help="start after installation (requires --enable)")
    args = p.parse_args()
    if args.system and not args.install:
        p.error("--system requires --install")
    if args.enable and not args.install:
        p.error("--enable requires --install")
    if args.start and not args.enable:
        p.error("--start requires --enable")
    try:
        unit = render(args)
        if not args.install:
            if args.output:
                target = pathlib.Path(args.output).expanduser().resolve()
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_text(unit, encoding="utf-8")
            else:
                sys.stdout.write(unit)
            return 0
        if sys.platform != "linux" or not shutil.which("systemctl"):
            raise RuntimeError("systemd installation is supported only on Linux hosts with systemctl")
        if args.system:
            if os.geteuid() != 0:
                raise RuntimeError("--system installation requires root; no implicit sudo is invoked")
            target_dir = pathlib.Path("/etc/systemd/system")
        else:
            target_dir = pathlib.Path.home() / ".config/systemd/user"
        target_dir.mkdir(parents=True, exist_ok=True)
        target = target_dir / f"{args.name}.service"
        fd, temp_name = tempfile.mkstemp(prefix=f".{args.name}.", dir=target_dir)
        try:
            with os.fdopen(fd, "w", encoding="utf-8") as stream:
                stream.write(unit)
                stream.flush()
                os.fsync(stream.fileno())
            os.replace(temp_name, target)
        finally:
            if os.path.exists(temp_name):
                os.unlink(temp_name)
        scope = ["--system"] if args.system else ["--user"]
        subprocess.run(["systemctl", *scope, "daemon-reload"], check=True)
        if args.enable:
            subprocess.run(["systemctl", *scope, "enable", f"{args.name}.service"], check=True)
        if args.start:
            subprocess.run(["systemctl", *scope, "start", f"{args.name}.service"], check=True)
        print(f"installed {target}", file=sys.stderr)
        return 0
    except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as exc:
        print(f"sleela-native-service: {exc}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
