# HISTORICAL DESIGN NOTE — ALTER TABLE ADD COLUMN IS NOW IMPLEMENTED.
#
# Current executable examples:
#   09-alter-table-add-column.sql
#   10-alter-table-default.sql
#   11-alter-table-errors.sql
#
# Supported classic-SQL syntax:
#   ALTER TABLE contacts ADD COLUMN phone
#   ALTER TABLE contacts ADD COLUMN status DEFAULT 'active'
#
# Existing rows receive the declared DEFAULT value, or an empty string when
# DEFAULT is omitted. Duplicate column names are rejected. This file remains
# only as the original design note; use the numbered executable examples.
