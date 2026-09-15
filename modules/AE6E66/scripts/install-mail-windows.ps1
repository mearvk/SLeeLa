# AE6E66 Windows mail integration preflight.
# This script never installs software, creates services, changes ACLs, or writes mail credentials.
$ErrorActionPreference = "Stop"

Write-Host "AE6E66 Windows mail preflight"

$required = @("powershell")
foreach ($name in $required) {
    if (-not (Get-Command $name -ErrorAction SilentlyContinue)) {
        throw "Required command not available: $name"
    }
}

Write-Host "No hMailServer/stunnel installation is performed by AE6E66."
Write-Host "Configure an administrator-managed local submission service separately."
Write-Host "Requirements: loopback-only submission, authenticated/authorized sender identity,"
Write-Host "TLS at the appropriate submission boundary, recipient validation, rate limiting,"
Write-Host "and explicit operator approval before any bulk delivery."
Write-Host "AE6E66 Windows mail preflight: PASS"
