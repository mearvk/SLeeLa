#!/usr/bin/env python3
"""Emit bodies for the java.time core: LocalDate, LocalTime, LocalDateTime,
Instant, Duration, Period, Year, YearMonth, MonthDay, DayOfWeek, Month.

The algorithms are the ISO-8601 proleptic Gregorian calculations of the
java.time specification (epoch-day conversion over 400-year cycles, month
arithmetic through the proleptic month, nano-of-day time). The VM has no
static dispatch across classes, so the Java static factories (of, parse,
ofEpochDay, between, ...) are instance methods; any value works as a handle,
e.g. new LocalDate(1970, 1, 1).ofEpochDay(19000).
"""
import json
import sys
import tempfile
from pathlib import Path

import implement

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import jdk_api_declare as dec  # noqa: E402

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))
API = {json.loads(l)["name"]: json.loads(l) for l in dec.API.read_text().splitlines() if l.strip()}

COMMON = r'''
  < shared java.time helpers >
  long floorDiv(long x, long y) {
    long q = x / y;
    if ((x % y != 0) && ((x < 0) != (y < 0))) { q = q - 1; }
    return q;
  }
  long floorMod(long x, long y) { return x - floorDiv(x, y) * y; }
  int wrap32(long v) {
    long m = v % 4294967296;
    if (m < 0) { m = m + 4294967296; }
    if (m > 2147483647) { m = m - 4294967296; }
    return (int) m;
  }
  << Bitwise xor of two 32-bit two-complement ints. >>
  int xorInt(int a, int b) {
    long x = a;
    long y = b;
    if (x < 0) { x = x + 4294967296; }
    if (y < 0) { y = y + 4294967296; }
    long r = 0;
    long bit = 1;
    int k = 0;
    while (k < 32) {
      if ((x % 2) != (y % 2)) { r = r + bit; }
      x = x / 2; y = y / 2; bit = bit * 2; k = k + 1;
    }
    return wrap32(r);
  }
  String pad(long v, int width) {
    String s = "" + v;
    int n = javaStringLength(javaStringFromUtf8(s));
    while (n < width) { s = "0" + s; n = n + 1; }
    return s;
  }
  boolean leap(long y) { return (y % 4 == 0) && ((y % 100 != 0) || (y % 400 == 0)); }
  int monthLength(int m, boolean isLeap) {
    if (m == 2) { if (isLeap) { return 29; } return 28; }
    if (m == 4 || m == 6 || m == 9 || m == 11) { return 30; }
    return 31;
  }
  void checkField(String field, long value, long lo, long hi) {
    if (value < lo || value > hi) {
      throw new DateTimeException("Invalid value for " + field + " (valid values " + lo + " - " + hi + "): " + value);
    }
  }
  String monthName(int m) {
    if (m == 1) { return "JANUARY"; } if (m == 2) { return "FEBRUARY"; } if (m == 3) { return "MARCH"; }
    if (m == 4) { return "APRIL"; } if (m == 5) { return "MAY"; } if (m == 6) { return "JUNE"; }
    if (m == 7) { return "JULY"; } if (m == 8) { return "AUGUST"; } if (m == 9) { return "SEPTEMBER"; }
    if (m == 10) { return "OCTOBER"; } if (m == 11) { return "NOVEMBER"; }
    return "DECEMBER";
  }
  << Year rendering of ISO-8601 local dates: four digits zero padded, a sign
     for negative years, and a leading + beyond 9999. >>
  String yearText(long y) {
    long a = y;
    if (a < 0) { a = 0 - a; }
    if (a < 1000) {
      if (y < 0) { return "-" + pad(a, 4); }
      return pad(a, 4);
    }
    if (y > 9999) { return "+" + y; }
    return "" + y;
  }
  << Nano-of-second suffix in the Java style: 3, 6 or 9 digits, as needed. >>
  String nanoText(long nano) {
    if (nano % 1000000 == 0) { return "." + pad(nano / 1000000, 3); }
    if (nano % 1000 == 0) { return "." + pad(nano / 1000, 6); }
    return "." + pad(nano, 9);
  }
  < parse helpers over a Java UTF-16 copy of the text >
  String parseText;
  String parseUnits;
  int parsePos;
  int parseLen;
  void parseBegin(String text) {
    if (text == null) { throw new NullPointerException("text"); }
    this.parseText = text;
    this.parseUnits = javaStringFromUtf8(text);
    this.parsePos = 0;
    this.parseLen = javaStringLength(this.parseUnits);
  }
  DateTimeParseException parseError() {
    return new DateTimeParseException("Text '" + this.parseText + "' could not be parsed at index " + this.parsePos, this.parseText, this.parsePos);
  }
  int peekUnit() {
    if (this.parsePos >= this.parseLen) { return -1; }
    return javaStringCharAt(this.parseUnits, this.parsePos);
  }
  void expectUnit(int c) {
    if (peekUnit() != c) { throw parseError(); }
    this.parsePos = this.parsePos + 1;
  }
  long digits(int minCount, int maxCount) {
    long v = 0;
    int n = 0;
    while (n < maxCount && peekUnit() >= 48 && peekUnit() <= 57) {
      v = v * 10 + (peekUnit() - 48);
      this.parsePos = this.parsePos + 1;
      n = n + 1;
    }
    if (n < minCount) { throw parseError(); }
    return v;
  }
  void parseEnd() { if (this.parsePos != this.parseLen) { throw parseError(); } }
  long parseYear() {
    int c = peekUnit();
    if (c == 43 || c == 45) {
      this.parsePos = this.parsePos + 1;
      long y = digits(4, 10);
      if (c == 45) { return 0 - y; }
      return y;
    }
    return digits(4, 4);
  }
  << time part HH:mm[:ss[.fraction]] as nano-of-day >>
  long parseTimeNanos() {
    long h = digits(2, 2);
    expectUnit(58);
    long mi = digits(2, 2);
    long s = 0;
    long nano = 0;
    if (peekUnit() == 58) {
      this.parsePos = this.parsePos + 1;
      s = digits(2, 2);
      if (peekUnit() == 46) {
        this.parsePos = this.parsePos + 1;
        int start = this.parsePos;
        long f = digits(1, 9);
        int n = this.parsePos - start;
        while (n < 9) { f = f * 10; n = n + 1; }
        nano = f;
      }
    }
    checkField("HourOfDay", h, 0, 23);
    checkField("MinuteOfHour", mi, 0, 59);
    checkField("SecondOfMinute", s, 0, 59);
    return ((h * 60 + mi) * 60 + s) * 1000000000 + nano;
  }
'''


def enum_core(name):
    return "\n".join(dec.enum_members(API[f"java.time.{name}"], name, set())) + "\n"


DAYOFWEEK = enum_core("DayOfWeek") + r'''
  < java.time.DayOfWeek behavior (ISO: Monday = 1 .. Sunday = 7) >
  int getValue() { return this.enumOrdinal + 1; }
  public static DayOfWeek of(int dayOfWeek) {
    if (dayOfWeek < 1 || dayOfWeek > 7) { throw new DateTimeException("Invalid value for DayOfWeek: " + dayOfWeek); }
    DayOfWeek[] v = values();
    return v[dayOfWeek - 1];
  }
  DayOfWeek plus(long days) {
    long amount = days % 7;
    int idx = (int) ((amount + 7 + this.enumOrdinal) % 7);
    DayOfWeek[] v = values();
    return v[idx];
  }
  DayOfWeek minus(long days) { return plus(0 - (days % 7)); }
'''

MONTH = enum_core("Month") + r'''
  < java.time.Month behavior >
  int getValue() { return this.enumOrdinal + 1; }
  public static Month of(int month) {
    if (month < 1 || month > 12) { throw new DateTimeException("Invalid value for MonthOfYear: " + month); }
    Month[] v = values();
    return v[month - 1];
  }
  Month plus(long months) {
    long amount = months % 12;
    int idx = (int) ((amount + 12 + this.enumOrdinal) % 12);
    Month[] v = values();
    return v[idx];
  }
  Month minus(long months) { return plus(0 - (months % 12)); }
  int length(boolean leapYear) {
    int m = this.enumOrdinal + 1;
    if (m == 2) { if (leapYear) { return 29; } return 28; }
    if (m == 4 || m == 6 || m == 9 || m == 11) { return 30; }
    return 31;
  }
  int minLength() { return length(false); }
  int maxLength() { return length(true); }
  int firstDayOfYear(boolean leapYear) {
    int lp = 0;
    if (leapYear) { lp = 1; }
    int m = this.enumOrdinal + 1;
    if (m == 1) { return 1; }
    if (m == 2) { return 32; }
    if (m == 3) { return 60 + lp; }
    if (m == 4) { return 91 + lp; }
    if (m == 5) { return 121 + lp; }
    if (m == 6) { return 152 + lp; }
    if (m == 7) { return 182 + lp; }
    if (m == 8) { return 213 + lp; }
    if (m == 9) { return 244 + lp; }
    if (m == 10) { return 274 + lp; }
    if (m == 11) { return 305 + lp; }
    return 335 + lp;
  }
  Month firstMonthOfQuarter() {
    Month[] v = values();
    return v[(this.enumOrdinal / 3) * 3];
  }
'''

LOCALDATE = r'''  < ---- java.time.LocalDate members (procedural implementation in SLeeLa) ---- >
  << An ISO-8601 date without time zone. The constructor is the validating
     LocalDate.of(year, month, dayOfMonth); the static factories are
     instance methods. Epoch-day conversion is the java.time 400-year cycle
     algorithm, so every result matches Java for the whole supported range
     (-999999999 .. 999999999). >>
  int year;
  int month;
  int day;
  LocalDate(int year, int month, int dayOfMonth) {
    checkField("Year", year, -999999999, 999999999);
    checkField("MonthOfYear", month, 1, 12);
    checkField("DayOfMonth", dayOfMonth, 1, 31);
    if (dayOfMonth > 28 && dayOfMonth > monthLength(month, leap(year))) {
      if (dayOfMonth == 29) {
        throw new DateTimeException("Invalid date 'February 29' as '" + year + "' is not a leap year");
      }
      throw new DateTimeException("Invalid date '" + monthName(month) + " " + dayOfMonth + "'");
    }
    this.year = year;
    this.month = month;
    this.day = dayOfMonth;
  }

  < static factories (call on any LocalDate) >
  LocalDate of(int year, int month, int dayOfMonth) { return new LocalDate(year, month, dayOfMonth); }
  LocalDate ofEpochDay(long epochDay) {
    checkField("EpochDay", epochDay, -365243219162, 365241780471);
    long zeroDay = epochDay + 719528;
    zeroDay = zeroDay - 60;
    long adjust = 0;
    if (zeroDay < 0) {
      long adjustCycles = (zeroDay + 1) / 146097 - 1;
      adjust = adjustCycles * 400;
      zeroDay = zeroDay + (0 - adjustCycles) * 146097;
    }
    long yearEst = (400 * zeroDay + 591) / 146097;
    long doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
    if (doyEst < 0) {
      yearEst = yearEst - 1;
      doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
    }
    yearEst = yearEst + adjust;
    long marchDoy0 = doyEst;
    long marchMonth0 = (marchDoy0 * 5 + 2) / 153;
    long m = (marchMonth0 + 2) % 12 + 1;
    long dom = marchDoy0 - (marchMonth0 * 306 + 5) / 10 + 1;
    yearEst = yearEst + marchMonth0 / 10;
    return new LocalDate((int) yearEst, (int) m, (int) dom);
  }
  LocalDate ofYearDay(int year, int dayOfYear) {
    checkField("Year", year, -999999999, 999999999);
    checkField("DayOfYear", dayOfYear, 1, 366);
    boolean lp = leap(year);
    if (dayOfYear == 366 && !lp) {
      throw new DateTimeException("Invalid date 'DayOfYear 366' as '" + year + "' is not a leap year");
    }
    int m = 1;
    int remaining = dayOfYear;
    while (remaining > monthLength(m, lp)) { remaining = remaining - monthLength(m, lp); m = m + 1; }
    return new LocalDate(year, m, remaining);
  }
  LocalDate parse(String text) {
    parseBegin(text);
    long y = parseYear();
    expectUnit(45);
    long m = digits(2, 2);
    expectUnit(45);
    long d = digits(2, 2);
    parseEnd();
    return new LocalDate((int) y, (int) m, (int) d);
  }

  < fields >
  int getYear() { return this.year; }
  int getMonthValue() { return this.month; }
  Month getMonth() { return Month.of(this.month); }
  int getDayOfMonth() { return this.day; }
  int getDayOfYear() {
    Month mo = getMonth();
    return mo.firstDayOfYear(isLeapYear()) + this.day - 1;
  }
  DayOfWeek getDayOfWeek() {
    return DayOfWeek.of((int) floorMod(toEpochDay() + 3, 7) + 1);
  }
  boolean isLeapYear() { return leap(this.year); }
  int lengthOfMonth() { return monthLength(this.month, isLeapYear()); }
  int lengthOfYear() { if (isLeapYear()) { return 366; } return 365; }
  long toEpochDay() {
    long y = this.year;
    long m = this.month;
    long total = 365 * y;
    if (y >= 0) { total = total + (y + 3) / 4 - (y + 99) / 100 + (y + 399) / 400; }
    else { total = total - (y / -4 - y / -100 + y / -400); }
    total = total + (367 * m - 362) / 12;
    total = total + this.day - 1;
    if (m > 2) {
      total = total - 1;
      if (!isLeapYear()) { total = total - 1; }
    }
    return total - 719528;
  }
  long getProlepticMonth() { long y = this.year; return y * 12 + this.month - 1; }

  < arithmetic >
  LocalDate resolvePreviousValid(long y, long m, int d) {
    int len = monthLength((int) m, leap(y));
    int dd = d;
    if (dd > len) { dd = len; }
    return new LocalDate((int) y, (int) m, dd);
  }
  LocalDate plusYears(long yearsToAdd) {
    if (yearsToAdd == 0) { return this; }
    long y = this.year + yearsToAdd;
    checkField("Year", y, -999999999, 999999999);
    return resolvePreviousValid(y, this.month, this.day);
  }
  LocalDate plusMonths(long monthsToAdd) {
    if (monthsToAdd == 0) { return this; }
    long calcMonths = getProlepticMonth() + monthsToAdd;
    long y = floorDiv(calcMonths, 12);
    checkField("Year", y, -999999999, 999999999);
    return resolvePreviousValid(y, floorMod(calcMonths, 12) + 1, this.day);
  }
  LocalDate plusWeeks(long weeksToAdd) { return plusDays(weeksToAdd * 7); }
  LocalDate plusDays(long daysToAdd) {
    if (daysToAdd == 0) { return this; }
    return ofEpochDay(toEpochDay() + daysToAdd);
  }
  LocalDate minusYears(long n) { return plusYears(0 - n); }
  LocalDate minusMonths(long n) { return plusMonths(0 - n); }
  LocalDate minusWeeks(long n) { return plusWeeks(0 - n); }
  LocalDate minusDays(long n) { return plusDays(0 - n); }
  LocalDate withYear(int y) { return resolvePreviousValid(y, this.month, this.day); }
  LocalDate withMonth(int m) { checkField("MonthOfYear", m, 1, 12); return resolvePreviousValid(this.year, m, this.day); }
  LocalDate withDayOfMonth(int d) { return new LocalDate(this.year, this.month, d); }
  LocalDate withDayOfYear(int dayOfYear) { return ofYearDay(this.year, dayOfYear); }
  Period until(LocalDate end) {
    long totalMonths = end.getProlepticMonth() - getProlepticMonth();
    long days = end.getDayOfMonth() - this.day;
    if (totalMonths > 0 && days < 0) {
      totalMonths = totalMonths - 1;
      LocalDate calcDate = plusMonths(totalMonths);
      days = end.toEpochDay() - calcDate.toEpochDay();
    } else {
      if (totalMonths < 0 && days > 0) {
        totalMonths = totalMonths + 1;
        days = days - end.lengthOfMonth();
      }
    }
    return new Period((int) (totalMonths / 12), (int) (totalMonths % 12), (int) days);
  }
  LocalDateTime atTime(int hour, int minute, int second, int nanoOfSecond) {
    return new LocalDateTime(this, new LocalTime(hour, minute, second, nanoOfSecond));
  }
  LocalDateTime atStartOfDay() { return new LocalDateTime(this, new LocalTime(0, 0, 0, 0)); }

  < comparison and identity >
  int compareTo(LocalDate other) {
    int c = this.year - other.getYear();
    if (c == 0) {
      c = this.month - other.getMonthValue();
      if (c == 0) { c = this.day - other.getDayOfMonth(); }
    }
    return c;
  }
  boolean isAfter(LocalDate other) { return compareTo(other) > 0; }
  boolean isBefore(LocalDate other) { return compareTo(other) < 0; }
  boolean isEqual(LocalDate other) { return compareTo(other) == 0; }
  boolean equals(Any obj) {
    if (obj instanceof LocalDate) { LocalDate o = (LocalDate) obj; return compareTo(o) == 0; }
    return false;
  }
  << Java: (year and 0xFFFFF800) xor ((year shl 11) + (month shl 6) + day). >>
  int hashCode() {
    long y = this.year;
    int high = wrap32(y - floorMod(y, 2048));
    int low = wrap32(y * 2048 + this.month * 64 + this.day);
    return xorInt(high, low);
  }
  String toString() { return yearText(this.year) + "-" + pad(this.month, 2) + "-" + pad(this.day, 2); }
''' + COMMON

LOCALTIME = r'''  < ---- java.time.LocalTime members (procedural implementation in SLeeLa) ---- >
  << A time of day without date or zone, to nanosecond precision. The
     constructor is the validating LocalTime.of(hour, minute, second,
     nanoOfSecond); arithmetic wraps around midnight as in Java. >>
  int hour;
  int minute;
  int second;
  int nano;
  LocalTime(int hour, int minute, int second, int nanoOfSecond) {
    checkField("HourOfDay", hour, 0, 23);
    checkField("MinuteOfHour", minute, 0, 59);
    checkField("SecondOfMinute", second, 0, 59);
    checkField("NanoOfSecond", nanoOfSecond, 0, 999999999);
    this.hour = hour;
    this.minute = minute;
    this.second = second;
    this.nano = nanoOfSecond;
  }
  LocalTime of(int hour, int minute, int second, int nanoOfSecond) { return new LocalTime(hour, minute, second, nanoOfSecond); }
  LocalTime ofSecondOfDay(long secondOfDay) {
    checkField("SecondOfDay", secondOfDay, 0, 86399);
    return new LocalTime((int) (secondOfDay / 3600), (int) ((secondOfDay / 60) % 60), (int) (secondOfDay % 60), 0);
  }
  LocalTime ofNanoOfDay(long nanoOfDay) {
    checkField("NanoOfDay", nanoOfDay, 0, 86399999999999);
    long s = nanoOfDay / 1000000000;
    return new LocalTime((int) (s / 3600), (int) ((s / 60) % 60), (int) (s % 60), (int) (nanoOfDay % 1000000000));
  }
  LocalTime parse(String text) {
    parseBegin(text);
    long nod = parseTimeNanos();
    parseEnd();
    return ofNanoOfDay(nod);
  }
  int getHour() { return this.hour; }
  int getMinute() { return this.minute; }
  int getSecond() { return this.second; }
  int getNano() { return this.nano; }
  int toSecondOfDay() { return this.hour * 3600 + this.minute * 60 + this.second; }
  long toNanoOfDay() {
    long s = toSecondOfDay();
    return s * 1000000000 + this.nano;
  }
  LocalTime plusNanos(long nanosToAdd) {
    if (nanosToAdd == 0) { return this; }
    long nofd = toNanoOfDay();
    long nn = floorMod(nanosToAdd % 86400000000000 + nofd + 86400000000000, 86400000000000);
    return ofNanoOfDay(nn);
  }
  LocalTime plusSeconds(long secondsToAdd) {
    if (secondsToAdd == 0) { return this; }
    long sofd = toSecondOfDay();
    long ns = floorMod(secondsToAdd % 86400 + sofd + 86400, 86400);
    return new LocalTime((int) (ns / 3600), (int) ((ns / 60) % 60), (int) (ns % 60), this.nano);
  }
  LocalTime plusMinutes(long minutesToAdd) {
    if (minutesToAdd == 0) { return this; }
    long mofd = this.hour * 60 + this.minute;
    long nm = floorMod(minutesToAdd % 1440 + mofd + 1440, 1440);
    return new LocalTime((int) (nm / 60), (int) (nm % 60), this.second, this.nano);
  }
  LocalTime plusHours(long hoursToAdd) {
    if (hoursToAdd == 0) { return this; }
    long nh = floorMod(hoursToAdd % 24 + this.hour + 24, 24);
    return new LocalTime((int) nh, this.minute, this.second, this.nano);
  }
  LocalTime minusHours(long n) { return plusHours(0 - (n % 24)); }
  LocalTime minusMinutes(long n) { return plusMinutes(0 - (n % 1440)); }
  LocalTime minusSeconds(long n) { return plusSeconds(0 - (n % 86400)); }
  LocalTime minusNanos(long n) { return plusNanos(0 - (n % 86400000000000)); }
  LocalTime withHour(int h) { return new LocalTime(h, this.minute, this.second, this.nano); }
  LocalTime withMinute(int m) { return new LocalTime(this.hour, m, this.second, this.nano); }
  LocalTime withSecond(int s) { return new LocalTime(this.hour, this.minute, s, this.nano); }
  LocalTime withNano(int n) { return new LocalTime(this.hour, this.minute, this.second, n); }
  LocalDateTime atDate(LocalDate date) { return new LocalDateTime(date, this); }
  int compareTo(LocalTime other) {
    long a = toNanoOfDay();
    long b = other.toNanoOfDay();
    if (a < b) { return -1; }
    if (a > b) { return 1; }
    return 0;
  }
  boolean isAfter(LocalTime other) { return compareTo(other) > 0; }
  boolean isBefore(LocalTime other) { return compareTo(other) < 0; }
  boolean equals(Any obj) {
    if (obj instanceof LocalTime) { LocalTime o = (LocalTime) obj; return compareTo(o) == 0; }
    return false;
  }
  int hashCode() { Long l = new Long(toNanoOfDay()); return l.hashCode(); }
  String toString() {
    String s = pad(this.hour, 2) + ":" + pad(this.minute, 2);
    if (this.second > 0 || this.nano > 0) {
      s = s + ":" + pad(this.second, 2);
      if (this.nano > 0) { s = s + nanoText(this.nano); }
    }
    return s;
  }
''' + COMMON

LOCALDATETIME = r'''  < ---- java.time.LocalDateTime members (procedural implementation in SLeeLa) ---- >
  << A date-time without zone: a LocalDate plus a LocalTime. The constructor
     is LocalDateTime.of(date, time); time arithmetic carries into the date. >>
  LocalDate date;
  LocalTime time;
  LocalDateTime(LocalDate date, LocalTime time) {
    if (date == null) { throw new NullPointerException("date"); }
    if (time == null) { throw new NullPointerException("time"); }
    this.date = date;
    this.time = time;
  }
  LocalDateTime of(int year, int month, int dayOfMonth, int hour, int minute, int second, int nanoOfSecond) {
    return new LocalDateTime(new LocalDate(year, month, dayOfMonth), new LocalTime(hour, minute, second, nanoOfSecond));
  }
  LocalDateTime ofEpochSecond(long epochSecond, int nanoOfSecond, int offsetSeconds) {
    checkField("NanoOfSecond", nanoOfSecond, 0, 999999999);
    long localSecond = epochSecond + offsetSeconds;
    long localEpochDay = floorDiv(localSecond, 86400);
    long secsOfDay = floorMod(localSecond, 86400);
    LocalDate d = this.date.ofEpochDay(localEpochDay);
    LocalTime t = new LocalTime((int) (secsOfDay / 3600), (int) ((secsOfDay / 60) % 60), (int) (secsOfDay % 60), nanoOfSecond);
    return new LocalDateTime(d, t);
  }
  LocalDateTime parse(String text) {
    parseBegin(text);
    long y = parseYear();
    expectUnit(45);
    long m = digits(2, 2);
    expectUnit(45);
    long d = digits(2, 2);
    expectUnit(84);
    long nod = parseTimeNanos();
    parseEnd();
    LocalTime t0 = new LocalTime(0, 0, 0, 0);
    return new LocalDateTime(new LocalDate((int) y, (int) m, (int) d), t0.ofNanoOfDay(nod));
  }
  LocalDate toLocalDate() { return this.date; }
  LocalTime toLocalTime() { return this.time; }
  int getYear() { return this.date.getYear(); }
  int getMonthValue() { return this.date.getMonthValue(); }
  Month getMonth() { return this.date.getMonth(); }
  int getDayOfMonth() { return this.date.getDayOfMonth(); }
  int getDayOfYear() { return this.date.getDayOfYear(); }
  DayOfWeek getDayOfWeek() { return this.date.getDayOfWeek(); }
  int getHour() { return this.time.getHour(); }
  int getMinute() { return this.time.getMinute(); }
  int getSecond() { return this.time.getSecond(); }
  int getNano() { return this.time.getNano(); }
  LocalDateTime with(LocalDate newDate, LocalTime newTime) {
    if (newDate == this.date && newTime == this.time) { return this; }
    return new LocalDateTime(newDate, newTime);
  }
  LocalDateTime plusYears(long n) { return with(this.date.plusYears(n), this.time); }
  LocalDateTime plusMonths(long n) { return with(this.date.plusMonths(n), this.time); }
  LocalDateTime plusWeeks(long n) { return with(this.date.plusWeeks(n), this.time); }
  LocalDateTime plusDays(long n) { return with(this.date.plusDays(n), this.time); }
  << Java plusWithOverflow: add a nano amount, carrying whole days into the date. >>
  LocalDateTime plusNanosCarry(long hours, long minutes, long seconds, long nanos) {
    if (hours == 0 && minutes == 0 && seconds == 0 && nanos == 0) { return this; }
    long totDays = nanos / 86400000000000 + seconds / 86400 + minutes / 1440 + hours / 24;
    long totNanos = nanos % 86400000000000 + (seconds % 86400) * 1000000000 + (minutes % 1440) * 60000000000 + (hours % 24) * 3600000000000;
    long curNoD = this.time.toNanoOfDay();
    totNanos = totNanos + curNoD;
    totDays = totDays + floorDiv(totNanos, 86400000000000);
    long newNoD = floorMod(totNanos, 86400000000000);
    LocalTime nt = this.time;
    if (newNoD != curNoD) { nt = this.time.ofNanoOfDay(newNoD); }
    return with(this.date.plusDays(totDays), nt);
  }
  LocalDateTime plusHours(long n) { return plusNanosCarry(n, 0, 0, 0); }
  LocalDateTime plusMinutes(long n) { return plusNanosCarry(0, n, 0, 0); }
  LocalDateTime plusSeconds(long n) { return plusNanosCarry(0, 0, n, 0); }
  LocalDateTime plusNanos(long n) { return plusNanosCarry(0, 0, 0, n); }
  LocalDateTime minusYears(long n) { return plusYears(0 - n); }
  LocalDateTime minusMonths(long n) { return plusMonths(0 - n); }
  LocalDateTime minusWeeks(long n) { return plusWeeks(0 - n); }
  LocalDateTime minusDays(long n) { return plusDays(0 - n); }
  LocalDateTime minusHours(long n) { return plusHours(0 - n); }
  LocalDateTime minusMinutes(long n) { return plusMinutes(0 - n); }
  LocalDateTime minusSeconds(long n) { return plusSeconds(0 - n); }
  LocalDateTime minusNanos(long n) { return plusNanos(0 - n); }
  LocalDateTime withYear(int y) { return with(this.date.withYear(y), this.time); }
  LocalDateTime withMonth(int m) { return with(this.date.withMonth(m), this.time); }
  LocalDateTime withDayOfMonth(int d) { return with(this.date.withDayOfMonth(d), this.time); }
  LocalDateTime withDayOfYear(int d) { return with(this.date.withDayOfYear(d), this.time); }
  LocalDateTime withHour(int h) { return with(this.date, this.time.withHour(h)); }
  LocalDateTime withMinute(int m) { return with(this.date, this.time.withMinute(m)); }
  LocalDateTime withSecond(int s) { return with(this.date, this.time.withSecond(s)); }
  LocalDateTime withNano(int n) { return with(this.date, this.time.withNano(n)); }
  << Seconds from the epoch for a zone offset given in seconds. >>
  long toEpochSecond(int offsetSeconds) {
    long epochDay = this.date.toEpochDay();
    long secs = epochDay * 86400 + this.time.toSecondOfDay();
    return secs - offsetSeconds;
  }
  int compareTo(LocalDateTime other) {
    int c = this.date.compareTo(other.toLocalDate());
    if (c == 0) { c = this.time.compareTo(other.toLocalTime()); }
    return c;
  }
  boolean isAfter(LocalDateTime other) { return compareTo(other) > 0; }
  boolean isBefore(LocalDateTime other) { return compareTo(other) < 0; }
  boolean isEqual(LocalDateTime other) { return compareTo(other) == 0; }
  boolean equals(Any obj) {
    if (obj instanceof LocalDateTime) { LocalDateTime o = (LocalDateTime) obj; return compareTo(o) == 0; }
    return false;
  }
  int hashCode() { return xorInt(this.date.hashCode(), this.time.hashCode()); }
  String toString() { return this.date.toString() + "T" + this.time.toString(); }
''' + COMMON

DURATION = r'''  < ---- java.time.Duration members (procedural implementation in SLeeLa) ---- >
  << A time-based amount: seconds plus a nano adjustment in 0 .. 999999999.
     The constructor is Duration.ofSeconds(seconds, nanoAdjustment) with
     normalization; ofSeconds(long) is ofSeconds, ofSeconds(long, long) is
     ofSecondsAdjusted. multipliedBy / dividedBy use 128-bit arithmetic, as
     Java does with BigDecimal. >>
  long seconds;
  int nanos;
  Duration(long seconds, long nanoAdjustment) {
    this.seconds = seconds + floorDiv(nanoAdjustment, 1000000000);
    this.nanos = (int) floorMod(nanoAdjustment, 1000000000);
  }
  Duration ofSecondsAdjusted(long s, long nanoAdjustment) { return new Duration(s, nanoAdjustment); }
  Duration ofSeconds(long s) { return new Duration(s, 0); }
  Duration ofDays(long days) { return new Duration(new Math().multiplyExactLong(days, 86400), 0); }
  Duration ofHours(long hours) { return new Duration(new Math().multiplyExactLong(hours, 3600), 0); }
  Duration ofMinutes(long minutes) { return new Duration(new Math().multiplyExactLong(minutes, 60), 0); }
  Duration ofMillis(long millis) { return new Duration(floorDiv(millis, 1000), floorMod(millis, 1000) * 1000000); }
  Duration ofNanos(long nanos) { return new Duration(0, nanos); }
  << Duration.between for LocalTime, LocalDateTime and Instant arguments. >>
  Duration between(Any startInclusive, Any endExclusive) {
    if (startInclusive instanceof Instant && endExclusive instanceof Instant) {
      Instant a = (Instant) startInclusive;
      Instant b = (Instant) endExclusive;
      return new Duration(b.getEpochSecond() - a.getEpochSecond(), b.getNano() - a.getNano());
    }
    if (startInclusive instanceof LocalTime && endExclusive instanceof LocalTime) {
      LocalTime c = (LocalTime) startInclusive;
      LocalTime d = (LocalTime) endExclusive;
      return new Duration(0, d.toNanoOfDay() - c.toNanoOfDay());
    }
    if (startInclusive instanceof LocalDateTime && endExclusive instanceof LocalDateTime) {
      LocalDateTime e = (LocalDateTime) startInclusive;
      LocalDateTime f = (LocalDateTime) endExclusive;
      long secs = f.toEpochSecond(0) - e.toEpochSecond(0);
      return new Duration(secs, f.getNano() - e.getNano());
    }
    throw new DateTimeException("Unsupported temporal pair for Duration.between");
  }
  Duration parse(String text) {
    parseBegin(text);
    boolean negate = false;
    if (peekUnit() == 45) { negate = true; this.parsePos = this.parsePos + 1; }
    else { if (peekUnit() == 43) { this.parsePos = this.parsePos + 1; } }
    int p = peekUnit();
    if (p != 80 && p != 112) { throw parseError(); }
    this.parsePos = this.parsePos + 1;
    int128 total = 0;
    boolean any = false;
    if (peekUnit() != 84 && peekUnit() != 116) {
      long dd = signedNumber();
      int u = peekUnit();
      if (u != 68 && u != 100) { throw parseError(); }
      this.parsePos = this.parsePos + 1;
      total = total + dd * 86400000000000;
      any = true;
    }
    if (peekUnit() == 84 || peekUnit() == 116) {
      this.parsePos = this.parsePos + 1;
      boolean part = false;
      while (this.parsePos < this.parseLen) {
        boolean neg = false;
        if (peekUnit() == 45) { neg = true; this.parsePos = this.parsePos + 1; }
        else { if (peekUnit() == 43) { this.parsePos = this.parsePos + 1; } }
        long v = digits(1, 18);
        long frac = 0;
        int fracDigits = 0;
        if (peekUnit() == 46 || peekUnit() == 44) {
          this.parsePos = this.parsePos + 1;
          int start = this.parsePos;
          frac = digits(1, 9);
          fracDigits = this.parsePos - start;
        }
        int u = peekUnit();
        this.parsePos = this.parsePos + 1;
        int128 amount = 0;
        if ((u == 72 || u == 104) && fracDigits == 0) { amount = v; amount = amount * 3600000000000; }
        else {
          if ((u == 77 || u == 109) && fracDigits == 0) { amount = v; amount = amount * 60000000000; }
          else {
            if (u == 83 || u == 115) {
              while (fracDigits < 9) { frac = frac * 10; fracDigits = fracDigits + 1; }
              amount = v;
              amount = amount * 1000000000 + frac;
            } else { throw parseError(); }
          }
        }
        if (neg) { amount = 0 - amount; }
        total = total + amount;
        part = true;
      }
      if (!part) { throw parseError(); }
      any = true;
    }
    if (!any) { throw parseError(); }
    parseEnd();
    if (negate) { total = 0 - total; }
    return fromNanos128(total);
  }
  long signedNumber() {
    boolean neg = false;
    if (peekUnit() == 45) { neg = true; this.parsePos = this.parsePos + 1; }
    else { if (peekUnit() == 43) { this.parsePos = this.parsePos + 1; } }
    long v = digits(1, 18);
    if (neg) { return 0 - v; }
    return v;
  }
  Duration fromNanos128(int128 total) {
    int128 q = total / 1000000000;
    int128 r = total % 1000000000;
    if (r < 0) { r = r + 1000000000; q = q - 1; }
    if (q > 9223372036854775807 || q < -9223372036854775807 - 1) { throw new ArithmeticException("Exceeds capacity of Duration: " + q); }
    return new Duration((long) q, (long) r);
  }
  int128 totalNanos128() {
    int128 t = this.seconds;
    t = t * 1000000000 + this.nanos;
    return t;
  }

  long getSeconds() { return this.seconds; }
  int getNano() { return this.nanos; }
  boolean isZero() { return this.seconds == 0 && this.nanos == 0; }
  boolean isNegative() { return this.seconds < 0; }
  boolean isPositive() { return this.seconds > 0 || (this.seconds == 0 && this.nanos > 0); }
  Duration withSeconds(long s) { return new Duration(s, this.nanos); }
  Duration withNanos(int nanoOfSecond) {
    checkField("NanoOfSecond", nanoOfSecond, 0, 999999999);
    return new Duration(this.seconds, nanoOfSecond);
  }
  Duration plus(Duration d) { return plusParts(d.getSeconds(), d.getNano()); }
  Duration minus(Duration d) { return fromNanos128(totalNanos128() - d.totalNanos128()); }
  Duration plusParts(long secondsToAdd, long nanosToAdd) {
    if (secondsToAdd == 0 && nanosToAdd == 0) { return this; }
    int128 t = totalNanos128();
    int128 s = secondsToAdd;
    t = t + s * 1000000000 + nanosToAdd;
    return fromNanos128(t);
  }
  Duration plusDays(long n) { return plusParts(new Math().multiplyExactLong(n, 86400), 0); }
  Duration plusHours(long n) { return plusParts(new Math().multiplyExactLong(n, 3600), 0); }
  Duration plusMinutes(long n) { return plusParts(new Math().multiplyExactLong(n, 60), 0); }
  Duration plusSeconds(long n) { return plusParts(n, 0); }
  Duration plusMillis(long n) { return plusParts(n / 1000, (n % 1000) * 1000000); }
  Duration plusNanos(long n) { return plusParts(0, n); }
  Duration minusDays(long n) { return plusDays(0 - n); }
  Duration minusHours(long n) { return plusHours(0 - n); }
  Duration minusMinutes(long n) { return plusMinutes(0 - n); }
  Duration minusSeconds(long n) { return plusSeconds(0 - n); }
  Duration minusMillis(long n) { return plusMillis(0 - n); }
  Duration minusNanos(long n) { return plusNanos(0 - n); }
  Duration multipliedBy(long multiplicand) {
    if (multiplicand == 0) { return new Duration(0, 0); }
    if (multiplicand == 1) { return this; }
    return fromNanos128(totalNanos128() * multiplicand);
  }
  Duration dividedBy(long divisor) {
    if (divisor == 0) { throw new ArithmeticException("Cannot divide by zero"); }
    if (divisor == 1) { return this; }
    return fromNanos128(totalNanos128() / divisor);
  }
  Duration negated() { return multipliedBy(-1); }
  Duration abs() { if (isNegative()) { return negated(); } return this; }
  long toDays() { return this.seconds / 86400; }
  long toHours() { return this.seconds / 3600; }
  long toMinutes() { return this.seconds / 60; }
  long toSeconds() { return this.seconds; }
  long toMillis() {
    long s = this.seconds;
    long n = this.nanos;
    if (s < 0) { s = s + 1; n = n - 1000000000; }
    Math M = new Math();
    long millis = M.multiplyExactLong(s, 1000);
    return M.addExactLong(millis, n / 1000000);
  }
  long toNanos() {
    long s = this.seconds;
    long n = this.nanos;
    if (s < 0) { s = s + 1; n = n - 1000000000; }
    Math M = new Math();
    long total = M.multiplyExactLong(s, 1000000000);
    return M.addExactLong(total, n);
  }
  long toDaysPart() { return this.seconds / 86400; }
  int toHoursPart() { return (int) (toHours() % 24); }
  int toMinutesPart() { return (int) (toMinutes() % 60); }
  int toSecondsPart() { return (int) (this.seconds % 60); }
  int toMillisPart() { return this.nanos / 1000000; }
  int toNanosPart() { return this.nanos; }
  int compareTo(Duration other) {
    if (this.seconds < other.getSeconds()) { return -1; }
    if (this.seconds > other.getSeconds()) { return 1; }
    return this.nanos - other.getNano();
  }
  boolean equals(Any obj) {
    if (obj instanceof Duration) { Duration o = (Duration) obj; return this.seconds == o.getSeconds() && this.nanos == o.getNano(); }
    return false;
  }
  int hashCode() { Long l = new Long(this.seconds); return wrap32(l.hashCode() + 51 * this.nanos); }
  String toString() {
    if (isZero()) { return "PT0S"; }
    long effectiveTotalSecs = this.seconds;
    if (this.seconds < 0 && this.nanos > 0) { effectiveTotalSecs = effectiveTotalSecs + 1; }
    long hours = effectiveTotalSecs / 3600;
    long minutes = (effectiveTotalSecs % 3600) / 60;
    long secs = effectiveTotalSecs % 60;
    String buf = "PT";
    if (hours != 0) { buf = buf + hours + "H"; }
    if (minutes != 0) { buf = buf + minutes + "M"; }
    if (secs == 0 && this.nanos == 0 && buf != "PT") { return buf; }
    if (this.seconds < 0 && this.nanos > 0) {
      if (secs == 0) { buf = buf + "-0"; } else { buf = buf + secs; }
    } else { buf = buf + secs; }
    if (this.nanos > 0) {
      long f = this.nanos;
      if (this.seconds < 0) { f = 1000000000 - this.nanos; }
      int width = 9;
      while (f % 10 == 0) { f = f / 10; width = width - 1; }
      buf = buf + "." + pad(f, width);
    }
    return buf + "S";
  }
''' + COMMON

PERIOD = r'''  < ---- java.time.Period members (procedural implementation in SLeeLa) ---- >
  << A date-based amount of years, months and days (ISO calendar). The
     constructor is Period.of(years, months, days). >>
  int years;
  int months;
  int days;
  Period(int years, int months, int days) { this.years = years; this.months = months; this.days = days; }
  Period of(int y, int m, int d) { return new Period(y, m, d); }
  Period ofYears(int y) { return new Period(y, 0, 0); }
  Period ofMonths(int m) { return new Period(0, m, 0); }
  Period ofWeeks(int w) { return new Period(0, 0, new Math().multiplyExactInt(w, 7)); }
  Period ofDays(int d) { return new Period(0, 0, d); }
  Period between(LocalDate startDateInclusive, LocalDate endDateExclusive) { return startDateInclusive.until(endDateExclusive); }
  Period parse(String text) {
    parseBegin(text);
    boolean negate = false;
    if (peekUnit() == 45) { negate = true; this.parsePos = this.parsePos + 1; }
    else { if (peekUnit() == 43) { this.parsePos = this.parsePos + 1; } }
    int p = peekUnit();
    if (p != 80 && p != 112) { throw parseError(); }
    this.parsePos = this.parsePos + 1;
    long y = 0;
    long m = 0;
    long w = 0;
    long d = 0;
    boolean any = false;
    while (this.parsePos < this.parseLen) {
      boolean neg = false;
      if (peekUnit() == 45) { neg = true; this.parsePos = this.parsePos + 1; }
      else { if (peekUnit() == 43) { this.parsePos = this.parsePos + 1; } }
      long v = digits(1, 10);
      if (neg) { v = 0 - v; }
      int u = peekUnit();
      this.parsePos = this.parsePos + 1;
      if (u == 89 || u == 121) { y = v; }
      else { if (u == 77 || u == 109) { m = v; }
      else { if (u == 87 || u == 119) { w = v; }
      else { if (u == 68 || u == 100) { d = v; } else { throw parseError(); } } } }
      any = true;
    }
    if (!any) { throw parseError(); }
    long dd = d + w * 7;
    if (negate) { y = 0 - y; m = 0 - m; dd = 0 - dd; }
    return new Period((int) y, (int) m, (int) dd);
  }
  int getYears() { return this.years; }
  int getMonths() { return this.months; }
  int getDays() { return this.days; }
  boolean isZero() { return this.years == 0 && this.months == 0 && this.days == 0; }
  boolean isNegative() { return this.years < 0 || this.months < 0 || this.days < 0; }
  Period withYears(int y) { return new Period(y, this.months, this.days); }
  Period withMonths(int m) { return new Period(this.years, m, this.days); }
  Period withDays(int d) { return new Period(this.years, this.months, d); }
  Period plus(Period p) {
    Math M = new Math();
    return new Period(M.addExactInt(this.years, p.getYears()), M.addExactInt(this.months, p.getMonths()), M.addExactInt(this.days, p.getDays()));
  }
  Period minus(Period p) {
    Math M = new Math();
    return new Period(M.subtractExactInt(this.years, p.getYears()), M.subtractExactInt(this.months, p.getMonths()), M.subtractExactInt(this.days, p.getDays()));
  }
  Period plusYears(long n) { return new Period(new Math().toIntExact(this.years + n), this.months, this.days); }
  Period plusMonths(long n) { return new Period(this.years, new Math().toIntExact(this.months + n), this.days); }
  Period plusDays(long n) { return new Period(this.years, this.months, new Math().toIntExact(this.days + n)); }
  Period minusYears(long n) { return plusYears(0 - n); }
  Period minusMonths(long n) { return plusMonths(0 - n); }
  Period minusDays(long n) { return plusDays(0 - n); }
  Period multipliedBy(int scalar) {
    Math M = new Math();
    return new Period(M.multiplyExactInt(this.years, scalar), M.multiplyExactInt(this.months, scalar), M.multiplyExactInt(this.days, scalar));
  }
  Period negated() { return multipliedBy(-1); }
  long toTotalMonths() { long y = this.years; return y * 12 + this.months; }
  Period normalized() {
    long totalMonths = toTotalMonths();
    long splitYears = totalMonths / 12;
    int splitMonths = (int) (totalMonths % 12);
    if (splitYears == this.years && splitMonths == this.months) { return this; }
    return new Period(new Math().toIntExact(splitYears), splitMonths, this.days);
  }
  boolean equals(Any obj) {
    if (obj instanceof Period) { Period o = (Period) obj; return this.years == o.getYears() && this.months == o.getMonths() && this.days == o.getDays(); }
    return false;
  }
  int hashCode() {
    Integer I = new Integer(0);
    return wrap32(this.years + I.rotateLeft(this.months, 8) + I.rotateLeft(this.days, 16));
  }
  String toString() {
    if (isZero()) { return "P0D"; }
    String b = "P";
    if (this.years != 0) { b = b + this.years + "Y"; }
    if (this.months != 0) { b = b + this.months + "M"; }
    if (this.days != 0) { b = b + this.days + "D"; }
    return b;
  }
''' + COMMON

INSTANT = r'''  < ---- java.time.Instant members (procedural implementation in SLeeLa) ---- >
  << An instantaneous point on the UTC time-line: epoch seconds plus a nano
     adjustment. The constructor is Instant.ofEpochSecond(epochSecond,
     nanoAdjustment); now() reads the VM UTC clock (timeUtcNanos).
     ofEpochSecond(long, long) is ofEpochSecondAdjusted. >>
  long seconds;
  int nanos;
  Instant(long epochSecond, long nanoAdjustment) {
    long s = epochSecond + floorDiv(nanoAdjustment, 1000000000);
    checkInstant(s);
    this.seconds = s;
    this.nanos = (int) floorMod(nanoAdjustment, 1000000000);
  }
  void checkInstant(long s) {
    if (s < -31557014167219200 || s > 31556889864403199) { throw new DateTimeException("Instant exceeds minimum or maximum instant"); }
  }
  Instant ofEpochSecond(long epochSecond) { return new Instant(epochSecond, 0); }
  Instant ofEpochSecondAdjusted(long epochSecond, long nanoAdjustment) { return new Instant(epochSecond, nanoAdjustment); }
  Instant ofEpochMilli(long epochMilli) { return new Instant(floorDiv(epochMilli, 1000), floorMod(epochMilli, 1000) * 1000000); }
  Instant now() { long n = timeUtcNanos(); return new Instant(floorDiv(n, 1000000000), floorMod(n, 1000000000)); }
  Instant parse(String text) {
    parseBegin(text);
    long y = parseYear();
    expectUnit(45);
    long m = digits(2, 2);
    expectUnit(45);
    long d = digits(2, 2);
    expectUnit(84);
    long nod = parseTimeNanos();
    expectUnit(90);
    parseEnd();
    LocalDate date = new LocalDate((int) y, (int) m, (int) d);
    long secs = date.toEpochDay() * 86400 + nod / 1000000000;
    return new Instant(secs, nod % 1000000000);
  }
  long getEpochSecond() { return this.seconds; }
  int getNano() { return this.nanos; }
  long toEpochMilli() {
    Math M = new Math();
    if (this.seconds < 0 && this.nanos > 0) {
      long millis = M.multiplyExactLong(this.seconds + 1, 1000);
      long adjustment = this.nanos / 1000000 - 1000;
      return M.addExactLong(millis, adjustment);
    }
    long millis2 = M.multiplyExactLong(this.seconds, 1000);
    return M.addExactLong(millis2, this.nanos / 1000000);
  }
  Instant plusParts(long secondsToAdd, long nanosToAdd) {
    if (secondsToAdd == 0 && nanosToAdd == 0) { return this; }
    Math M = new Math();
    long epochSec = M.addExactLong(this.seconds, secondsToAdd);
    epochSec = M.addExactLong(epochSec, nanosToAdd / 1000000000);
    long nanoAdjustment = this.nanos + nanosToAdd % 1000000000;
    return new Instant(epochSec, nanoAdjustment);
  }
  Instant plusSeconds(long n) { return plusParts(n, 0); }
  Instant plusMillis(long n) { return plusParts(n / 1000, (n % 1000) * 1000000); }
  Instant plusNanos(long n) { return plusParts(0, n); }
  Instant plus(Duration d) { return plusParts(d.getSeconds(), d.getNano()); }
  Instant minusSeconds(long n) { return plusSeconds(0 - n); }
  Instant minusMillis(long n) { return plusMillis(0 - n); }
  Instant minusNanos(long n) { return plusNanos(0 - n); }
  Instant minus(Duration d) { return plusParts(0 - d.getSeconds(), 0 - d.getNano()); }
  int compareTo(Instant other) {
    if (this.seconds < other.getEpochSecond()) { return -1; }
    if (this.seconds > other.getEpochSecond()) { return 1; }
    return this.nanos - other.getNano();
  }
  boolean isAfter(Instant other) { return compareTo(other) > 0; }
  boolean isBefore(Instant other) { return compareTo(other) < 0; }
  boolean equals(Any obj) {
    if (obj instanceof Instant) { Instant o = (Instant) obj; return this.seconds == o.getEpochSecond() && this.nanos == o.getNano(); }
    return false;
  }
  int hashCode() { Long l = new Long(this.seconds); return wrap32(l.hashCode() + 51 * this.nanos); }
  << ISO-8601 in UTC (DateTimeFormatter.ISO_INSTANT): seconds always shown,
     the fraction in groups of three digits. >>
  String toString() {
    LocalDate h = new LocalDate(1970, 1, 1);
    LocalDate date = h.ofEpochDay(floorDiv(this.seconds, 86400));
    long sod = floorMod(this.seconds, 86400);
    String t = pad(sod / 3600, 2) + ":" + pad((sod / 60) % 60, 2) + ":" + pad(sod % 60, 2);
    if (this.nanos > 0) { t = t + nanoText(this.nanos); }
    return date.toString() + "T" + t + "Z";
  }
''' + COMMON

YEAR = r'''  < ---- java.time.Year members (procedural implementation in SLeeLa) ---- >
  << A year in the ISO calendar. The constructor is Year.of(year). >>
  int year;
  Year(int isoYear) { checkField("Year", isoYear, -999999999, 999999999); this.year = isoYear; }
  Year of(int isoYear) { return new Year(isoYear); }
  Year parse(String text) { parseBegin(text); long y = parseYear(); parseEnd(); return new Year((int) y); }
  boolean isLeapValue(long y) { return leap(y); }
  int getValue() { return this.year; }
  boolean isLeap() { return leap(this.year); }
  int length() { if (isLeap()) { return 366; } return 365; }
  boolean isValidMonthDay(MonthDay monthDay) { return monthDay != null && monthDay.isValidYear(this.year); }
  LocalDate atDay(int dayOfYear) { LocalDate h = new LocalDate(1970, 1, 1); return h.ofYearDay(this.year, dayOfYear); }
  YearMonth atMonth(int month) { return new YearMonth(this.year, month); }
  LocalDate atMonthDay(MonthDay monthDay) { return monthDay.atYear(this.year); }
  Year plusYears(long n) { if (n == 0) { return this; } return new Year((int) (this.year + n)); }
  Year minusYears(long n) { return plusYears(0 - n); }
  int compareTo(Year other) { return this.year - other.getValue(); }
  boolean isAfter(Year other) { return this.year > other.getValue(); }
  boolean isBefore(Year other) { return this.year < other.getValue(); }
  boolean equals(Any obj) {
    if (obj instanceof Year) { Year o = (Year) obj; return o.getValue() == this.year; }
    return false;
  }
  int hashCode() { return this.year; }
  String toString() { return "" + this.year; }
''' + COMMON

YEARMONTH = r'''  < ---- java.time.YearMonth members (procedural implementation in SLeeLa) ---- >
  << A year and month in the ISO calendar. The constructor is
     YearMonth.of(year, month). >>
  int year;
  int month;
  YearMonth(int year, int month) {
    checkField("Year", year, -999999999, 999999999);
    checkField("MonthOfYear", month, 1, 12);
    this.year = year;
    this.month = month;
  }
  YearMonth of(int year, int month) { return new YearMonth(year, month); }
  YearMonth parse(String text) {
    parseBegin(text);
    long y = parseYear();
    expectUnit(45);
    long m = digits(2, 2);
    parseEnd();
    return new YearMonth((int) y, (int) m);
  }
  int getYear() { return this.year; }
  int getMonthValue() { return this.month; }
  Month getMonth() { return Month.of(this.month); }
  boolean isLeapYear() { return leap(this.year); }
  boolean isValidDay(int dayOfMonth) { return dayOfMonth >= 1 && dayOfMonth <= lengthOfMonth(); }
  int lengthOfMonth() { return monthLength(this.month, isLeapYear()); }
  int lengthOfYear() { if (isLeapYear()) { return 366; } return 365; }
  LocalDate atDay(int dayOfMonth) { return new LocalDate(this.year, this.month, dayOfMonth); }
  LocalDate atEndOfMonth() { return new LocalDate(this.year, this.month, lengthOfMonth()); }
  long getProlepticMonth() { long y = this.year; return y * 12 + this.month - 1; }
  YearMonth plusMonths(long monthsToAdd) {
    if (monthsToAdd == 0) { return this; }
    long calcMonths = getProlepticMonth() + monthsToAdd;
    return new YearMonth((int) floorDiv(calcMonths, 12), (int) floorMod(calcMonths, 12) + 1);
  }
  YearMonth plusYears(long n) { if (n == 0) { return this; } return new YearMonth((int) (this.year + n), this.month); }
  YearMonth minusMonths(long n) { return plusMonths(0 - n); }
  YearMonth minusYears(long n) { return plusYears(0 - n); }
  YearMonth withYear(int y) { return new YearMonth(y, this.month); }
  YearMonth withMonth(int m) { return new YearMonth(this.year, m); }
  int compareTo(YearMonth other) {
    int c = this.year - other.getYear();
    if (c == 0) { c = this.month - other.getMonthValue(); }
    return c;
  }
  boolean isAfter(YearMonth other) { return compareTo(other) > 0; }
  boolean isBefore(YearMonth other) { return compareTo(other) < 0; }
  boolean equals(Any obj) {
    if (obj instanceof YearMonth) { YearMonth o = (YearMonth) obj; return compareTo(o) == 0; }
    return false;
  }
  << Java: year xor (month shl 27). >>
  int hashCode() { return xorInt(this.year, wrap32(this.month * 134217728)); }
  String toString() { return yearText(this.year) + "-" + pad(this.month, 2); }
''' + COMMON

MONTHDAY = r'''  < ---- java.time.MonthDay members (procedural implementation in SLeeLa) ---- >
  << A month-day in the ISO calendar, such as --12-03. The constructor is
     MonthDay.of(month, dayOfMonth); February 29 is allowed. >>
  int month;
  int day;
  MonthDay(int month, int dayOfMonth) {
    checkField("MonthOfYear", month, 1, 12);
    checkField("DayOfMonth", dayOfMonth, 1, 31);
    if (dayOfMonth > monthLength(month, true)) {
      throw new DateTimeException("Illegal value for DayOfMonth field, value " + dayOfMonth + " is not valid for month " + monthName(month));
    }
    this.month = month;
    this.day = dayOfMonth;
  }
  MonthDay of(int month, int dayOfMonth) { return new MonthDay(month, dayOfMonth); }
  MonthDay parse(String text) {
    parseBegin(text);
    expectUnit(45);
    expectUnit(45);
    long m = digits(2, 2);
    expectUnit(45);
    long d = digits(2, 2);
    parseEnd();
    return new MonthDay((int) m, (int) d);
  }
  int getMonthValue() { return this.month; }
  Month getMonth() { return Month.of(this.month); }
  int getDayOfMonth() { return this.day; }
  boolean isValidYear(int year) { return !(this.day == 29 && this.month == 2 && !leap(year)); }
  LocalDate atYear(int year) {
    int d = this.day;
    if (!isValidYear(year)) { d = 28; }
    return new LocalDate(year, this.month, d);
  }
  MonthDay withMonth(int m) {
    checkField("MonthOfYear", m, 1, 12);
    int d = this.day;
    if (d > monthLength(m, true)) { d = monthLength(m, true); }
    return new MonthDay(m, d);
  }
  MonthDay withDayOfMonth(int d) { return new MonthDay(this.month, d); }
  int compareTo(MonthDay other) {
    int c = this.month - other.getMonthValue();
    if (c == 0) { c = this.day - other.getDayOfMonth(); }
    return c;
  }
  boolean isAfter(MonthDay other) { return compareTo(other) > 0; }
  boolean isBefore(MonthDay other) { return compareTo(other) < 0; }
  boolean equals(Any obj) {
    if (obj instanceof MonthDay) { MonthDay o = (MonthDay) obj; return compareTo(o) == 0; }
    return false;
  }
  int hashCode() { return this.month * 64 + this.day; }
  String toString() { return "--" + pad(this.month, 2) + "-" + pad(this.day, 2); }
''' + COMMON

files = {
    "java.time.DayOfWeek": DAYOFWEEK,
    "java.time.Month": MONTH,
    "java.time.LocalDate": LOCALDATE,
    "java.time.LocalTime": LOCALTIME,
    "java.time.LocalDateTime": LOCALDATETIME,
    "java.time.Duration": DURATION,
    "java.time.Period": PERIOD,
    "java.time.Instant": INSTANT,
    "java.time.Year": YEAR,
    "java.time.YearMonth": YEARMONTH,
    "java.time.MonthDay": MONTHDAY,
}
for k, v in files.items():
    (OUT / f"{k}.body").write_text("@import java.java.time\n" + v)
implement.apply_dir(OUT)
