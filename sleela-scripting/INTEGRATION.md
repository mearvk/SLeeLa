# SLeeLa Source Integration

Sleela Script integrates with ordinary SLeeLa source without becoming a replacement language.

## Attachment

A SLeeLa source document may reference a script through an explicit scripting directive or host API. The exact source directive is versioned by the main SLeeLa language specification.

Conceptually:

    SLeeLa Document
      -> Script Attachment
           -> permissions
           -> timeout
           -> inputs
           -> known variables
           -> queue bindings
           -> result contract

## Authority hierarchy

1. SLeeLa document and language contract.
2. VM/runtime safety contract.
3. Host capability policy.
4. Script permissions.
5. Script instructions.

A lower level cannot override a higher contract.

## Inputs

Scripts can receive immutable document metadata, declared variables, object snapshots, VM status, sequence identifiers, queue records and approved lookup results.

## Outputs

Scripts can produce calculated values, definitions, lookup records, derived scientific/engineering results, queue messages, explicit object updates, VM conditions and structured diagnostics.

## Source integrity

The script operates beside the source document. It does not silently rewrite the source file. Source generation, when enabled, produces a normal source artifact subject to the ordinary compiler and validation path.
