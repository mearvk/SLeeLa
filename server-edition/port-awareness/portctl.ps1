param(
  [Parameter(Mandatory=$true)][ValidateSet("open","close","status")][string]$Action,
  [Parameter(Mandatory=$true)][ValidateSet("Discord-1","Discord-2","Discord-3","basic-probe","basic-scan")][string]$Edition,
  [Parameter(Mandatory=$true)][ValidateRange(1,65535)][int]$Port,
  [ValidateSet("tcp","udp")][string]$Protocol = "tcp"
)

$ErrorActionPreference = "Stop"
$RuleName = "SLeeLa-$Edition-$Protocol-$Port"
$os = Get-CimInstance Win32_OperatingSystem
$version = $os.Version

function Remove-SleeLaRule {
  Get-NetFirewallRule -DisplayName $RuleName -ErrorAction SilentlyContinue |
    Remove-NetFirewallRule -ErrorAction SilentlyContinue
}

switch ($Action) {
  "open" {
    Remove-SleeLaRule
    New-NetFirewallRule -DisplayName $RuleName -Direction Inbound -Action Allow -Protocol $Protocol -LocalPort $Port -Profile Any -Enabled True | Out-Null
  }
  "close" {
    Remove-SleeLaRule
  }
  "status" {
    Get-NetFirewallRule -DisplayName $RuleName -ErrorAction SilentlyContinue |
      Get-NetFirewallPortFilter | Select-Object Protocol,LocalPort
  }
}

Write-Output "SLeeLa portctl: action=$Action edition=$Edition port=$Port/$Protocol os=Windows version=$version firewall=Windows Defender Firewall rule=$RuleName"
