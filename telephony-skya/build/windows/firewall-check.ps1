[CmdletBinding()]
param()
$ErrorActionPreference = "Continue"
if ($env:SKYA_FIREWALL_CHECK -eq "0") { Write-Host "Skya firewall check disabled (SKYA_FIREWALL_CHECK=0)."; exit 0 }
Write-Host "Skya firewall preflight (Windows 10+)."
try {
  $profiles = Get-NetFirewallProfile -ErrorAction Stop
  foreach ($p in $profiles) { Write-Host ("  {0}: Enabled={1} DefaultInbound={2} DefaultOutbound={3}" -f $p.Name,$p.Enabled,$p.DefaultInboundAction,$p.DefaultOutboundAction) }
  Write-Host "  Note: outbound client connections normally need no inbound firewall rule; private-group/server hosting may require an inbound rule for the configured port."
} catch {
  Write-Warning ("Unable to query Windows Firewall: {0}" -f $_.Exception.Message)
}
