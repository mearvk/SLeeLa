#!/usr/bin/env python3
"""Validate SLeeLa configuration policy and examples without changing them."""
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]


def parse(path, sections=False, allow_empty_values_for=()):
    result = {}
    section = ""
    for number, raw in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith(("#", ";")):
            continue
        if sections and line.startswith("[") and line.endswith("]"):
            section = line[1:-1].strip()
            if not section:
                raise ValueError(f"{path.relative_to(ROOT)}:{number}: empty section")
            continue
        if "=" not in line:
            raise ValueError(f"{path.relative_to(ROOT)}:{number}: expected key=value")
        key, value = (item.strip() for item in line.split("=", 1))
        if not key or (not value and key not in allow_empty_values_for):
            raise ValueError(f"{path.relative_to(ROOT)}:{number}: empty key/value")
        key = f"{section}.{key}" if section else key
        if key in result:
            raise ValueError(f"{path.relative_to(ROOT)}:{number}: duplicate key {key}")
        result[key] = value
    return result


def unquote(value):
    if len(value) >= 2 and value[0] == value[-1] and value[0] in "'\"":
        return value[1:-1]
    return value


def main():
    policy = parse(ROOT / "config/config-location.conf")
    runtime = parse(ROOT / "config/sleela.conf", sections=True)
    # An empty JavaFX module path is intentional: it lets Java use its default
    # module search path when no explicit JavaFX installation path is configured.
    vm = parse(ROOT / "config/vm.conf", allow_empty_values_for={"javafx.module_path"})
    expected = {
        "config.project.name": "SLeeLa",
        "config.location.relative": ".sleela/config",
        "config.location.install_relative": "Config",
        "config.location.absolute": "/etc/sleela",
    }
    for key, value in expected.items():
        if policy.get(key) != value:
            raise ValueError(f"config/config-location.conf: {key} must equal {value!r}")
    mirrors = {
        "project.name": "config.project.name",
        "config.relative_path": "config.location.relative",
        "config.absolute_path": "config.location.absolute",
        "install.relative_path": "config.location.install_relative",
    }
    for runtime_key, policy_key in mirrors.items():
        if unquote(runtime.get(runtime_key, "")) != policy[policy_key]:
            raise ValueError(f"config/sleela.conf: {runtime_key} must match {policy_key}")
    try:
        version = int(unquote(vm.get("vm.version", "")))
    except ValueError as exc:
        raise ValueError("config/vm.conf: vm.version must be an integer") from exc
    if not 1 <= version <= 11:
        raise ValueError("config/vm.conf: vm.version must be in range 1..11")
    try:
        runtime_vm_version = int(unquote(runtime.get("vm.version", "")))
    except ValueError as exc:
        raise ValueError("config/sleela.conf: vm.version must be an integer") from exc
    if runtime_vm_version != version:
        raise ValueError("config/sleela.conf: vm.version must match config/vm.conf")
    if vm.get("vm.strict", "").lower() not in {"true", "false", "1", "0"}:
        raise ValueError("config/vm.conf: invalid vm.strict value")
    if vm.get("vm.profile", "") not in {"user", "administrator", "developer"}:
        raise ValueError("config/vm.conf: unsupported vm.profile")
    examples = sorted((ROOT / "config").glob("*.properties.example"))
    if not examples:
        raise ValueError("config/: no properties example templates found")
    for path in examples:
        parse(path)
    print(f"Configuration validation: PASS ({len(examples)} properties examples)")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, ValueError) as exc:
        print(f"Configuration validation: FAIL: {exc}", file=sys.stderr)
        sys.exit(1)
