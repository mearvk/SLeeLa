# Alphabet and Character-Set Examples

The examples below describe the intended SLeeLa 1.9 API surface. They are illustrative until the SLeeLa compiler and SLVM bind these calls to the native runtime.

## General alphabet

    #sleela 1.9
    let builder = new AlphabetBuilder("Latin");
    builder.vowel("A");
    builder.consonant("B");
    let latin = builder.build();
    print(latin.has("A"));

## Capacity signature

    // C256 means a catalogue capacity of 256 entries, not 256 bits.
    let symbols = new CharacterSet();
    symbols.configure("MySymbols", 256);

## UTF-4088

    // Pseudocode: keep experimental IDs separate from Unicode text.
    let id = Utf4088.symbolFromInput(inputState);
    if (Utf4088.isValidCodePoint(id)) {
      let record = Utf4088.lookup(id);
    }

Do not pass an experimental UTF-4088 identifier above U+10FFFF to ordinary UTF-8 encoding APIs.
