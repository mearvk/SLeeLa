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
        if args.user in {"root", "0"}:
            raise ValueError("system native services must run as an unprivileged account, not root")
        if not re.fullmatch(r"[a-z_][a-z0-9_-]{0,31}", args.user):
            raise ValueError("invalid system service user name")
        lines.append(f"User={args.user}")
    lines.extend([
        f"WorkingDirectory={systemd_quote(str(work))}",
        f"ExecStart={systemd_quote(str(exe))}",
        "Restart=on-failure",
        "RestartSec=3s",
        "NoNewPrivileges=yes",
        "CapabilityBoundingSet=",
        "AmbientCapabilities=",
        "PrivateTmp=yes",
        "PrivateDevices=yes",
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

def _unit_paths(args: argparse.Namespace):
    if not NAME.fullmatch(args.name):
        raise ValueError("service name must contain only letters, digits, dot, underscore, @, or hyphen")
    if args.system:
        # Reading system-unit status does not require root on a normally
        # configured host. Mutating operations (restart/stop/disable/rollback)
        # remain explicit administrator actions; never invoke sudo implicitly.
        mutating = not args.status
        if mutating and os.name != "nt" and os.geteuid() != 0:
            raise RuntimeError("mutating a system service requires root; no implicit sudo is invoked")
        directory = pathlib.Path("/etc/systemd/system")
    else:
        directory = pathlib.Path.home() / ".config/systemd/user"
    target = directory / f"{args.name}.service"
    return directory, target, target.with_name(target.name + ".sleela-backup")

def _systemctl(args: argparse.Namespace, *command: str) -> int:
    if sys.platform != "linux" or not shutil.which("systemctl"):
        raise RuntimeError("systemd lifecycle is supported only on Linux hosts with systemctl")
    scope = "--system" if args.system else "--user"
    return subprocess.run(["systemctl", scope, *command], check=False).returncode

def lifecycle(args: argparse.Namespace) -> int:
    directory, target, backup = _unit_paths(args)
    if args.status:
        return _systemctl(args, "--no-pager", "status", f"{args.name}.service")
    if args.restart:
        return _systemctl(args, "restart", f"{args.name}.service")
    if args.stop:
        return _systemctl(args, "stop", f"{args.name}.service")
    if args.disable:
        return _systemctl(args, "disable", f"{args.name}.service")
    if args.rollback:
        if not backup.is_file():
            raise RuntimeError(f"no rollback backup exists for {target}")
        if not target.parent.is_dir():
            raise RuntimeError(f"service unit directory does not exist: {target.parent}")
        fd, temporary = tempfile.mkstemp(prefix=f".{args.name}.rollback.", dir=target.parent)
        try:
            with os.fdopen(fd, "wb") as stream, backup.open("rb") as source:
                shutil.copyfileobj(source, stream)
                stream.flush(); os.fsync(stream.fileno())
            os.replace(temporary, target)
        finally:
            if os.path.exists(temporary): os.unlink(temporary)
        rc = _systemctl(args, "daemon-reload")
        if rc == 0:
            backup.unlink()
            print(f"restored previous unit {target}", file=sys.stderr)
        else:
            print(f"sleela-native-service: daemon-reload failed; rollback backup retained at {backup}", file=sys.stderr)
        return rc
    raise RuntimeError("no lifecycle action selected")

def main() -> int:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--name", required=True, help="unit name without .service")
    p.add_argument("--executable", help="absolute or resolvable executable path")
    p.add_argument("--working-directory", default=".")
    p.add_argument("--description", default="SLeeLa native workload")
    p.add_argument("--user", help="unprivileged account for a system unit")
    p.add_argument("--writable-path", action="append", default=[], help="explicit writable directory; repeatable")
    p.add_argument("--output", help="write rendered unit to this file; otherwise stdout")
    p.add_argument("--install", action="store_true", help="install a user or system unit")
    p.add_argument("--replace", action="store_true", help="replace an existing unit file; otherwise installation fails if it exists")
    p.add_argument("--system", action="store_true", help="install into /etc/systemd/system (requires root and --user)")
    p.add_argument("--enable", action="store_true", help="daemon-reload and enable after installation")
    p.add_argument("--start", action="store_true", help="start after installation (requires --enable)")
    actions = p.add_mutually_exclusive_group()
    actions.add_argument("--status", action="store_true", help="query the unit status")
    actions.add_argument("--restart", action="store_true", help="restart an installed unit")
    actions.add_argument("--stop", action="store_true", help="stop an installed unit")
    actions.add_argument("--disable", action="store_true", help="disable an installed unit")
    actions.add_argument("--rollback", action="store_true", help="restore the previous unit saved by --replace")
    args = p.parse_args()
    if any((args.status, args.restart, args.stop, args.disable, args.rollback)):
        if any((args.install, args.replace, args.enable, args.start, args.output, args.executable, args.writable_path)):
            p.error("lifecycle actions cannot be combined with unit rendering or installation options")
        try:
            return lifecycle(args)
        except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as exc:
            print(f"sleela-native-service: {exc}", file=sys.stderr)
            return 1
    if not args.executable:
        p.error("--executable is required when rendering or installing a unit")
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
        backup = target.with_name(target.name + ".sleela-backup")
        existed = target.exists()
        if existed and not args.replace:
            raise RuntimeError(f"unit already exists: {target}; pass --replace to replace it explicitly")
        if existed and backup.exists():
            raise RuntimeError(f"rollback backup already exists: {backup}; run --rollback before replacing again")

        # Preserve the previous unit before changing the live file. The backup
        # is written atomically and is retained after success for explicit
        # rollback. A failed install attempts to restore the previous state.
        if existed:
            backup_fd, backup_tmp = tempfile.mkstemp(prefix=f".{args.name}.backup.", dir=target_dir)
            try:
                with os.fdopen(backup_fd, "wb") as stream, target.open("rb") as source:
                    shutil.copyfileobj(source, stream)
                    stream.flush()
                    os.fsync(stream.fileno())
                shutil.copystat(target, backup_tmp)
                os.replace(backup_tmp, backup)
            finally:
                if os.path.exists(backup_tmp):
                    os.unlink(backup_tmp)

        fd, temp_name = tempfile.mkstemp(prefix=f".{args.name}.", dir=target_dir)
        installed = False
        enabled = False
        started = False
        try:
            with os.fdopen(fd, "w", encoding="utf-8") as stream:
                stream.write(unit)
                stream.flush()
                os.fsync(stream.fileno())
            os.replace(temp_name, target)
            installed = True

            scope = ["--system"] if args.system else ["--user"]
            subprocess.run(["systemctl", *scope, "daemon-reload"], check=True)
            if args.enable:
                subprocess.run(["systemctl", *scope, "enable", f"{args.name}.service"], check=True)
                enabled = True
            if args.start:
                subprocess.run(["systemctl", *scope, "start", f"{args.name}.service"], check=True)
                started = True
        except (OSError, subprocess.CalledProcessError) as exc:
            # Undo side effects in reverse order. Keep the backup if any
            # recovery step fails so an administrator can recover manually.
            recovery_errors = []
            if started or args.start:
                try:
                    subprocess.run(["systemctl", *scope, "stop", f"{args.name}.service"], check=True)
                except (OSError, subprocess.CalledProcessError) as recovery_exc:
                    recovery_errors.append(f"stop: {recovery_exc}")
            if enabled or args.enable:
                try:
                    subprocess.run(["systemctl", *scope, "disable", f"{args.name}.service"], check=True)
                except (OSError, subprocess.CalledProcessError) as recovery_exc:
                    recovery_errors.append(f"disable: {recovery_exc}")
            if installed:
                try:
                    if existed:
                        restore_fd, restore_tmp = tempfile.mkstemp(prefix=f".{args.name}.restore.", dir=target_dir)
                        try:
                            with os.fdopen(restore_fd, "wb") as stream, backup.open("rb") as source:
                                shutil.copyfileobj(source, stream)
                                stream.flush()
                                os.fsync(stream.fileno())
                            shutil.copystat(backup, restore_tmp)
                            os.replace(restore_tmp, target)
                        finally:
                            if os.path.exists(restore_tmp):
                                os.unlink(restore_tmp)
                    else:
                        target.unlink(missing_ok=True)
                    subprocess.run(["systemctl", *scope, "daemon-reload"], check=True)
                    if existed and not recovery_errors:
                        backup.unlink(missing_ok=True)
                except (OSError, subprocess.CalledProcessError) as recovery_exc:
                    recovery_errors.append(f"restore/reload: {recovery_exc}")
            if recovery_errors:
                raise RuntimeError(
                    f"installation failed ({exc}); automatic recovery was incomplete: "
                    + "; ".join(recovery_errors)
                    + (f"; preserve/review backup at {backup}" if backup.exists() else "")
                ) from exc
            raise RuntimeError(f"installation failed and previous state was restored: {exc}") from exc
        finally:
            if os.path.exists(temp_name):
                os.unlink(temp_name)
        print(f"installed {target}", file=sys.stderr)
        return 0
    except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as exc:
        print(f"sleela-native-service: {exc}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
