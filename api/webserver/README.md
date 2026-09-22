# SLeeLa Web Server Monitor

The SLeeLa Web Server Monitor is a deployment and operations module for web-aware applications that use Apache HTTP Server and/or Apache Tomcat.

It provides one operational vocabulary across Linux, Windows 10+, and macOS:
- install or verify Apache/Tomcat
- inspect service/process state
- perform HTTP health checks
- change configured listening ports
- install application modules/artifacts from an explicit local source
- upgrade through the host platform's package manager where supported
- produce machine-readable status and evidence
- avoid silent privilege escalation

## Design

SLeeLa application -> Web Server Monitor -> Apache/Tomcat adapter -> OS adapter -> health/config/deployment evidence.

The module does not replace Apache or Tomcat. It manages their deployment boundary and gives applications a consistent SLeeLa-facing contract.

## Operations

The platform adapters expose: install, status, health, upgrade, port, module, and validate.

On Linux and macOS, module deployment supports an explicitly supplied local WAR for Tomcat. Apache configuration/module deployment remains distribution-specific and fails closed rather than guessing an installation layout. Windows follows the same explicit approach.

## Safety boundary

Installation, service restart, package installation, and configuration changes can require administrator privileges. The scripts never silently assume root/Administrator access.

Before a restart, configuration is checked where the installed server exposes a supported validation command. A failed validation prevents the restart.

Module paths and package operations are explicit inputs. The monitor does not execute arbitrary XML, shell text, or application code as an administrative command.

## Application deployment

A web-aware SLeeLa application can treat this module as a deployment dependency:

1. verify the target server
2. install or upgrade the selected server
3. install the application's module/artifact
4. configure its port
5. validate
6. restart
7. perform an HTTP health check
8. emit deployment evidence

The health endpoint is configurable; the default is /.

## Configuration

SLEELA_WEB_SERVER: apache or tomcat
SLEELA_WEB_PORT: desired native listener port
SLEELA_WEB_HEALTH_URL: HTTP health URL
SLEELA_WEB_MODULE: local application artifact
SLEELA_TOMCAT_SERVICE: service name override
SLEELA_APACHE_SERVICE: service name override

Never put passwords or service credentials in these variables.

## Platform support

| Platform | Adapter | Intended operations |
|---|---|---|
| Linux | linux/webctl.sh | install, status, health, upgrade, port, module, validate |
| macOS | macos/webctl.sh | install, status, health, upgrade, port, module, validate |
| Windows 10+ | windows/WebCtl.ps1 | install, status, health, upgrade, port, module, validate |

Package-manager availability varies by machine. The adapters report unsupported operations instead of silently substituting unrelated software.

## API integration

The C API in sleela_webserver.h provides bounded configuration validation and server-selection vocabulary. It is suitable for BODI/XML or other SLeeLa orchestration layers.

The OS adapters remain responsible for native package managers, services, paths, and process controls.
