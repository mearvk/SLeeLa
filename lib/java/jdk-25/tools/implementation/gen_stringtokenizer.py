#!/usr/bin/env python3
"""Emit the body of java.util.StringTokenizer, written from the JDK 28 API
specification (no Java source text is copied).

The tokenizer keeps only the string, the delimiter set, the scan position and
the returnDelims flag; every query rescans from the position, so there is no
cached state to invalidate when nextToken(delim) changes the delimiters.
Delimiters are UTF-16 code units (BMP characters)."""
from pathlib import Path
import tempfile
import implement

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))

BODY = '''  < ---- java.util.StringTokenizer members (procedural implementation in SLeeLa) ---- >
  << Splits a string into tokens at delimiter characters. With returnDelims
     false a run of delimiters separates tokens and is skipped; with true each
     delimiter is itself a one-character token. nextToken(delim) replaces the
     delimiter set for this and every later call. >>
  String text;
  String delimiters;
  int currentPosition;
  int maxPosition;
  boolean retDelims;

  public StringTokenizer(java.lang.String str, java.lang.String delim, boolean returnDelims) {
    if (str == null) { throw new NullPointerException(); }
    this.text = str;
    this.delimiters = delim;
    this.currentPosition = 0;
    this.maxPosition = str.length();
    this.retDelims = returnDelims;
  }
  public StringTokenizer(java.lang.String str, java.lang.String delim) { this(str, delim, false); }
  public StringTokenizer(java.lang.String str) { this(str, " \\t\\n\\r\\f", false); }

  boolean isDelimiter(int c) {
    if (this.delimiters == null) { throw new NullPointerException(); }
    return this.delimiters.indexOf(c) >= 0;
  }
  int skipDelimiters(int start) {
    int position = start;
    if (this.retDelims) { return position; }
    while (position < this.maxPosition && isDelimiter(this.text.charAt(position))) { position = position + 1; }
    return position;
  }
  int scanToken(int start) {
    int position = start;
    while (position < this.maxPosition && !isDelimiter(this.text.charAt(position))) { position = position + 1; }
    if (this.retDelims && start == position && position < this.maxPosition) {
      position = position + 1;
    }
    return position;
  }

  public boolean hasMoreTokens() { return skipDelimiters(this.currentPosition) < this.maxPosition; }
  public java.lang.String nextToken() {
    int start = skipDelimiters(this.currentPosition);
    if (start >= this.maxPosition) { throw new NoSuchElementException(); }
    this.currentPosition = scanToken(start);
    return this.text.substring(start, this.currentPosition);
  }
  public java.lang.String nextToken(java.lang.String delim) {
    this.delimiters = delim;
    return nextToken();
  }
  public boolean hasMoreElements() { return hasMoreTokens(); }
  public java.lang.Object nextElement() { return nextToken(); }
  public int countTokens() {
    int count = 0;
    int position = this.currentPosition;
    while (position < this.maxPosition) {
      position = skipDelimiters(position);
      if (position >= this.maxPosition) { return count; }
      position = scanToken(position);
      count = count + 1;
    }
    return count;
  }
'''
(OUT / "java.util.StringTokenizer.body").write_text(BODY)
implement.apply_dir(OUT)
