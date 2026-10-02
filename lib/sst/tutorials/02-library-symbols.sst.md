# SST Tutorial 02 — Using /lib Classes

An SST document can reference classes from different /lib packages.

~~~sst
sheet LibrarySymbols {
  use sst.SSTSymbol;
  use nordshrift.NordshriftSymbol;
  use vm.SleelaVMCompilerManager;
  target sleela;
}
~~~

All three references use the same canonical library index.

~~~text
package-qualified SST references
          |
          v
    /lib Library Index
          |
          v
    Nordshrift Resolver
          |
          v
       .sleela
~~~

## Exercise

Add another package-qualified /lib class and verify that it follows the same discovery and resolution path.
