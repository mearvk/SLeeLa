[CmdletBinding()]param([ValidateSet("install","upgrade","status")][string]$Action="status")
$K=if($env:SLEELA_DB_KIND){$env:SLEELA_DB_KIND}else{"postgresql"}
$ids=@{postgresql="PostgreSQL.PostgreSQL";mysql="Oracle.MySQL";mariadb="MariaDB.Server";sqlite="SQLite.SQLite";sqlserver="Microsoft.SQLServer";oracle="Oracle.OracleDatabase"}
if(!$ids.ContainsKey($K)){Write-Error "unsupported database";exit 1}
if($Action -in @("install","upgrade") -and !(Get-Command winget -ErrorAction SilentlyContinue)){Write-Error "winget is required";exit 1}
switch($Action){install{winget install --id $ids[$K] --accept-package-agreements --accept-source-agreements};upgrade{winget upgrade --id $ids[$K] --accept-package-agreements --accept-source-agreements};status{Write-Output "SLeeLa dbctl: configured family is $K; native driver discovery is application-specific."}}
