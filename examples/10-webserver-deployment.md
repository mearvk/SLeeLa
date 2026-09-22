# SLeeLa Web Server Deployment Example

This profile is an application-facing deployment contract.

Linux:
  SLEELA_WEB_SERVER=tomcat SLEELA_WEB_PORT=8080 SLEELA_WEB_HEALTH_URL=/health SLEELA_WEB_MODULE=/path/app.war bash ../api/webserver/linux/webctl.sh module

Then:
  SLEELA_WEB_SERVER=tomcat SLEELA_WEB_PORT=8080 SLEELA_WEB_HEALTH_URL=/health bash ../api/webserver/linux/webctl.sh health

For macOS, replace the adapter with ../api/webserver/macos/webctl.sh.

For Windows 10+, run PowerShell as Administrator and use:
  $env:SLEELA_WEB_SERVER="tomcat"
  $env:SLEELA_WEB_PORT="8080"
  $env:SLEELA_WEB_HEALTH_URL="/health"
  $env:SLEELA_WEB_MODULE="C:\path\app.war"
  .\api\webserver\windows\WebCtl.ps1 module

The example deliberately uses an explicit local WAR and an explicit health path. Production deployments should use a real application health endpoint and a controlled release artifact.
