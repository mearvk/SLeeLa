# Exemplar source: American Airlines

- URL: https://www.aa.com/
- Status: NOT captured in this repo.
- Reason: the site returns HTTP 403 (WAF/bot protection) to automated clients,
  and the build sandbox blocks outbound egress (INTEGRATIONS_ONLY). See
  ../README.md for how to capture and drop the real HTML/CSS/JS/JSON here.
