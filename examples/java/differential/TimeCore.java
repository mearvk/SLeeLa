import java.time.*;

/** Java side of the TimeCore differential test; prints one result per line. */
public class TimeCore {
    public static void main(String[] args) {
        LocalDate d = LocalDate.of(2024, 2, 29);
        System.out.println(d + " " + d.getDayOfWeek() + " " + d.getDayOfYear() + " " + d.toEpochDay() + " " + d.lengthOfMonth() + " " + d.isLeapYear() + " " + d.hashCode());
        System.out.println(d.plusYears(1) + " " + d.plusMonths(12) + " " + d.plusDays(400) + " " + d.minusMonths(1) + " " + d.plusWeeks(-10) + " " + d.withDayOfYear(1));
        for (long e : new long[]{0, -1, 19000, -719528, 2932896, -100000000}) {
            LocalDate x = LocalDate.ofEpochDay(e);
            System.out.println(e + " " + x + " " + x.toEpochDay() + " " + x.getDayOfWeek() + " " + x.hashCode());
        }
        System.out.println(LocalDate.of(-44, 3, 15) + " " + LocalDate.of(12345, 1, 1) + " " + LocalDate.of(5, 6, 7) + " " + LocalDate.ofYearDay(2023, 365));
        System.out.println(LocalDate.parse("1999-12-31").plusDays(1) + " " + LocalDate.of(2023, 1, 31).until(LocalDate.of(2024, 3, 1)) + " " + Period.between(LocalDate.of(2024, 5, 20), LocalDate.of(2023, 2, 10)));
        try { LocalDate.of(2023, 2, 29); } catch (DateTimeException ex) { System.out.println(ex.getMessage()); }
        try { LocalDate.of(2023, 4, 31); } catch (DateTimeException ex) { System.out.println(ex.getMessage()); }
        try { LocalDate.of(2023, 13, 1); } catch (DateTimeException ex) { System.out.println(ex.getMessage()); }
        System.out.println(d.compareTo(LocalDate.of(2024, 3, 1)) + " " + d.isBefore(LocalDate.of(2024, 3, 1)) + " " + d.equals(LocalDate.parse("2024-02-29")));
        LocalTime t = LocalTime.of(23, 59, 30, 500000000);
        System.out.println(t + " " + t.plusSeconds(45) + " " + t.plusHours(-25) + " " + t.plusNanos(1) + " " + t.minusMinutes(1500) + " " + LocalTime.of(7, 0, 0, 0) + " " + LocalTime.of(7, 5, 3, 120000) + " " + t.hashCode());
        System.out.println(LocalTime.parse("10:15:30.123456789") + " " + LocalTime.ofSecondOfDay(3661) + " " + t.toNanoOfDay() + " " + t.toSecondOfDay());
        LocalDateTime dt = LocalDateTime.of(2024, 12, 31, 23, 59, 59, 999999999);
        System.out.println(dt + " " + dt.plusNanos(1) + " " + dt.plusHours(-48) + " " + dt.minusSeconds(86401) + " " + dt.toEpochSecond(ZoneOffset.UTC) + " " + dt.hashCode());
        System.out.println(LocalDateTime.ofEpochSecond(1700000000, 5, ZoneOffset.ofHours(2)) + " " + LocalDateTime.parse("2007-12-03T10:15:30") + " " + dt.getDayOfWeek() + " " + dt.getMonth());
        Duration du = Duration.ofSeconds(-90061, 5000000);
        System.out.println(du + " " + Duration.ofMillis(1500) + " " + Duration.ofHours(25).plusMinutes(1) + " " + Duration.ofNanos(-1) + " " + Duration.ZERO + " " + Duration.ofDays(2) + " " + Duration.ofSeconds(59, 999999999));
        System.out.println(du.toMillis() + " " + du.toNanos() + " " + du.toHours() + " " + du.toMinutesPart() + " " + du.toSecondsPart() + " " + du.getSeconds() + " " + du.getNano() + " " + du.hashCode());
        System.out.println(du.multipliedBy(3) + " " + du.dividedBy(7) + " " + du.negated() + " " + du.abs() + " " + Duration.parse("PT8H6M12.345S") + " " + Duration.parse("P2DT3H4M") + " " + Duration.parse("-PT-6H+3M"));
        System.out.println(Duration.between(LocalTime.of(10, 0, 0, 0), LocalTime.of(8, 30, 0, 0)) + " " + Duration.between(LocalDateTime.of(2024, 1, 1, 0, 0, 0, 0), dt) + " " + du.compareTo(Duration.ZERO));
        Period p = Period.of(1, 14, -3);
        System.out.println(p + " " + p.normalized() + " " + p.toTotalMonths() + " " + p.negated() + " " + p.multipliedBy(2) + " " + p.hashCode() + " " + Period.ZERO + " " + Period.parse("P2Y3W") + " " + Period.ofWeeks(3));
        Instant i = Instant.ofEpochSecond(1700000000, 123000000);
        System.out.println(i + " " + Instant.ofEpochMilli(-1) + " " + Instant.EPOCH + " " + i.plusMillis(877) + " " + i.toEpochMilli() + " " + Instant.ofEpochSecond(-1, 1).toEpochMilli() + " " + i.hashCode() + " " + Instant.parse("2007-12-03T10:15:30.00Z"));
        System.out.println(Instant.ofEpochSecond(253402300800L) + " " + Instant.ofEpochSecond(0, 1500) + " " + Duration.between(Instant.EPOCH, i));
        Year y = Year.of(2100);
        System.out.println(y + " " + y.isLeap() + " " + y.length() + " " + y.atDay(60) + " " + Year.of(2000).atDay(366) + " " + y.atMonth(2).atEndOfMonth());
        YearMonth ym = YearMonth.of(2024, 11);
        System.out.println(ym + " " + ym.plusMonths(3) + " " + ym.minusMonths(23) + " " + ym.lengthOfMonth() + " " + ym.atDay(5) + " " + ym.hashCode() + " " + YearMonth.parse("0999-01"));
        MonthDay md = MonthDay.of(2, 29);
        System.out.println(md + " " + md.isValidYear(2023) + " " + md.atYear(2023) + " " + md.atYear(2024) + " " + md.hashCode() + " " + MonthDay.parse("--12-03").compareTo(md));
        System.out.println(DayOfWeek.MONDAY.plus(-1) + " " + DayOfWeek.of(3) + " " + DayOfWeek.SUNDAY.getValue() + " " + Month.of(2).length(true) + " " + Month.MARCH.firstDayOfYear(true) + " " + Month.NOVEMBER.firstMonthOfQuarter() + " " + Month.JANUARY.minus(13));
    }
}
