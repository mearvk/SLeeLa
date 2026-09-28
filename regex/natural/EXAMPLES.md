# Natural Form Examples

## Identifier
```text
begin <name: letter (letter | digit | "_")*> end
```

## Phone-like number
```text
begin <area: digit{3}> "-" <number: digit{3}> "-" digit{4} end
```

## Dotted host
```text
begin letter+ ("." letter+)* end
```

## Choice
```text
("cat" or "dog" or "bird")
```

## Optional extension
```text
<name: letter+> ("." <extension: letter{1,8}>)?
```
