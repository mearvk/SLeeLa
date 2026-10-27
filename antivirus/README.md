
## SLeeLa-aware service control

The repository also includes a small systemd-aware controller for the Sleelavirin components. The controller is intentionally conservative: it exposes service lifecycle operations when `systemctl` is available, otherwise it reports the systemd state and policy information without pretending the service exists.

```sh
# See the default service and policy status
python3 antivirus/tools/sleela-antivirus-ctl.py status --json

# Run a native scan directly through the SLeeLa coordinator
python3 antivirus/tools/sleela-antivirus-ctl.py scan \
  --engine native \
  --report-json antivirus/build/sleela-antivirus-report.json \
  .

# Install the packaged systemd unit template on a Linux host with systemd
sudo python3 antivirus/tools/sleela-antivirus-ctl.py install-unit --target-dir /etc/systemd/system
sudo systemctl daemon-reload
sudo systemctl start sleela-antivirus.service
sudo systemctl status sleela-antivirus.service
```

This controller reads the repository's SLeeLa policy example and can act as a thin compatibility layer between the antivirus package and a host-level `systemctl` environment. The actual service target is still deliberately modelled as a controller for the scan pipeline, not a claim that all polices or hosts can safely execute a privileged on-access scanner by default.

The packaged unit template is kept in `antivirus/systemd/sleela-antivirus.service` and is intended to be adapted for the host's installation root and scan policy. The controller is a compatibility and orchestration layer; it does not silently elevate privileges or override the package-level fail-closed policy.

---

# SLeeLa
< p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

