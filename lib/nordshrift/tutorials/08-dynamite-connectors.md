# Nordshrift Tutorial 08 — Implicit System Loading

Nordshrift carries Dynamite metadata from primary SLeeLa source into the semantic and loader pipeline.

Example:

~~~text
// @dynamite class=MySQLConnector
// @dynamite.config=database-defaults
// @dynamite.property port=3306
~~~

The class can become an implicit-load candidate after symbol and configuration resolution. Installation, secrets, and provider credentials remain runtime configuration concerns.
