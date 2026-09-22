# SLeeLa XML Examples

This directory contains clear, runnable examples for the SLeeLa BODI XML project system.

## Example families

### 01 — Mathematics
../api/bodi/bodi-xml-runner 01-math.xml

### 02 — Physics
../api/bodi/bodi-xml-runner 02-physics.xml

### 03 — Chemistry
../api/bodi/bodi-xml-runner 03-chemistry.xml

### 04 — Mixed science
../api/bodi/bodi-xml-runner 04-mixed-science.xml

### 05 — SMTP email
../api/bodi/bodi-xml-runner 05-email-smtp.xml --send-email

### 06 — SMTP dry run
../api/bodi/bodi-xml-runner 06-email-dry-run.xml

### 07 — HTTP-style post
../api/bodi/bodi-xml-runner 07-post.xml

### 08 — Listener declaration
../api/bodi/bodi-xml-runner 08-listener.xml

### 09 — Router declaration
../api/bodi/bodi-xml-runner 09-router.xml

For SMTP projects, credentials are referenced through environment variables rather than stored in XML. Actual transmission requires --send-email.

## XML model

Every project starts with <bodi version="1"> and a project declaration. Science projects contain discipline and operation elements. The dispatcher is allow-listed; XML cannot invoke arbitrary shell commands or arbitrary native functions.
