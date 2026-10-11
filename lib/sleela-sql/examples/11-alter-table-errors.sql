# The first statement succeeds if contacts has no status column.
# The second statement should fail with "column already exists".
ALTER TABLE contacts ADD COLUMN status DEFAULT 'active'
ALTER TABLE contacts ADD COLUMN status
