# DESIGN REFERENCE ONLY — NOT EXECUTABLE BY THE CURRENT ENGINE.
#
# The current SLeeLa SQL native engine does not implement ALTER TABLE or
# ADD COLUMN. This file records the intended shape of a future feature.
# Do not feed these statements to the current CLI and expect success.
#
# Proposed syntax:
# ALTER TABLE contacts ADD COLUMN phone
# ALTER TABLE contacts ADD COLUMN created_at
#
# Migration behavior should be specified before implementation:
# - preserve existing rows;
# - define the value for the new column in old rows (e.g. empty string);
# - reject duplicate column names;
# - write to a temporary file and replace the table file safely;
# - leave the original data intact if migration fails.
