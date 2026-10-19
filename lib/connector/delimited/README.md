<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# CSV/TSV connector

Dependency-free record-level CSV/TSV parser. Supports quoted fields, delimiters inside quoted fields, and doubled quote escapes; rejects unterminated quotes and characters after a closing quote. Pass one record at a time; newline handling, file encoding, headers, and schema inference remain caller responsibilities. Delimiter must be comma or tab. Build with `make -C lib/connector/delimited`.