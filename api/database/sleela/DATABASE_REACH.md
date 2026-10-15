# Database — BODI™ and Munction™ table surfaces

Two SLeeLa procedural ways to **query, alter, or read a table** in the SLeeLa
Native Database Connector (`api/database`). Both are thin SLeeLa surfaces over
the connector's own vocabulary (validate, connect, ping, query, execute, begin,
commit, rollback, close); the native connector + platform driver still perform
the actual database operation.

| Surface | File | Shape | When |
|---|---|---|---|
| **BODI™** | [`DatabaseBodi.sleela`](DatabaseBodi.sleela) | witnessed middle verbs on an addressed table | step-by-step work over a held handle; a transaction with explicit begin/commit/rollback; every action recorded by the Witness |
| **Munction™** | [`DatabaseMunction.sleela`](DatabaseMunction.sleela) | one bounded reach sentence that connects, sends, consumes, latches, and closes with a receipt | reaching the DB like any other system method; one sane sentence per statement/transaction |

Both run on syntax `#sleela 1.3`. The statement datum may be written in **either**
dialect the engine accepts — classic SQL or the fluent SLeeLaSQL.

## BODI™ surface — witnessed change

BODI™ (see `markdown/BODI.md`, `src/implementations/_001_/bodi/BODI_VERBS.md`) is
the witness layer for change to an addressed object. The database action is the
*change*; the Witness records that it was addressed and observed.

```text
connector verb   BODI™ middle verb   meaning
--------------   -----------------   ------------------------------------
connect          open                open the addressed DB resource
execute (alter)  push                publish a change (CREATE/INSERT/DROP…)
query  (read)    pull                retrieve a datum / result set
begin            activate            make a transaction operational
commit           commit              retain the witnessed change
rollback         rollback            reverse the witnessed change
close            close               close the addressed DB resource
```

Usage (procedural, over a held handle):

```sleela
DatabaseBodi db = new DatabaseBodi();
TableBodi games = db.open("mysql://app@db:3306/catalog", "001", "dba", "games");

db.readRows(games, "SELECT title, year FROM games WHERE year = '1986'");   // pull
db.begin(games);                                                            // activate
db.alter(games, "table('games').create(id, title, year)");                 // push (SLeeLaSQL)
db.alter(games, "INSERT INTO games VALUES (1, 'Metroid', 1986)");           // push (SQL)
db.commit(games);                                                           // commit
db.query(games, "from('games').select(*)");                                 // pull
db.closeTable(games);                                                       // close
```

A verb against a closed resource stops at the boundary (BODI™ mitigative
circumference) rather than inventing meaning.

> `readRows` and `closeTable` are named to avoid `read` / `close`, which are
> reserved SLeeLa file-I/O built-ins.

## Munction™ surface — one bounded reach

Munction™ (see `markdown/MUNCTION.md`) reaches every system method with one
fluent sentence; the database is the `db:` channel. Each sentence stays inside
the 4..16-call sanity bound and the legal verb ladder, and closes with a
**receipt** — a witnessed, immutable record of what was actually reached.

```text
db:<kind>://<user>@<host>:<port>/<database>      e.g. db:mysql://app@db:3306/catalog

send(statement)   -> coherent push of a SQL or SLeeLaSQL statement
thatch(interims)  -> weave interim stages over the reach
consume()         -> pull one row of the result set (repeatable; receivable)
latch()           -> commit the transaction so it survives the close
closeWithReceipt  -> close the connection; hand back a witnessed receipt
```

```sleela
// ALTER — the minimum sane reach (4 calls).
Munction.start("ddl")
        .connect("db:mysql://app@db:3306/catalog")
        .send("table('games').create(id, title, year)")
        .closeWithReceipt();

// READ — send a query, consume rows, latch, close (7 calls).
Munction.start("scan")
        .connect("db:mysql://app@db:3306/catalog")
        .send("SELECT title, year FROM games WHERE year = '1986'")
        .consume()
        .consume()
        .latch()
        .closeWithReceipt();
```

The `db:` channel satisfies the Munction™ receivability + coherent-send contract
(`MUNCTION.md` §5): a `send` accounts offered vs. acknowledged bytes, a `consume`
never fabricates a row, and a bump leaves the reach `CONTAINED` with the residual
recorded — no fictional rollback.

## Running

```sh
# build the compiler/runtime once
make -C impl

# then check or run either surface
SLEELA_BIN=impl/build/sleela bin/SLeeLa check api/database/sleela/DatabaseBodi.sleela
SLEELA_BIN=impl/build/sleela bin/SLeeLa run   api/database/sleela/DatabaseBodi.sleela
SLEELA_BIN=impl/build/sleela bin/SLeeLa run   api/database/sleela/DatabaseMunction.sleela
```

## Credentials

As with the rest of the connector, credentials are supplied through environment
variables or an OS credential facility — never in source control, a URI, or a
command-line argument. A Munction™ `db:` URI carries the user and endpoint, not a
secret.
