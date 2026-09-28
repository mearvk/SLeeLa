# Natural Form

Natural Form lets a user read a pattern before learning traditional regex notation.

Compact:

```text
begin <user: letter (letter | digit | "_")*> "@" <host: letter+ ("." letter+)*> end
```

Expanded:

```text
begin
<user:
    letter
    (letter | digit | "_")*
>
"@"
<host:
    letter+
    ("." letter+)*
>
end
```

Both forms compile to the same canonical representation.
