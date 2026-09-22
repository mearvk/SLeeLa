# SLeeLa Data Analytics

The data-analytics shelf adds bounded numeric data-analysis operations to the SLeeLa science API.

## Operations

- sum
- mean
- min
- max
- variance
- stddev
- correlation (two equally sized data sets)

The native C facade accepts at most 1024 finite values per data set. It performs calculations in-process and does not open files, execute SQL, or create network connections.

## BODI XML

Use data-analytics as a science discipline. The BODI runner uses an explicit allow-list for analytics operations. Unsupported operations are rejected and witnessed.
