# Release X.Y (202?-??-??)

- Two new builtin functions,
  [`builtins.parseFlakeRef`](@docroot@/language/builtins.md#builtins-parseFlakeRef)
  and
  [`builtins.flakeRefToString`](@docroot@/language/builtins.md#builtins-flakeRefToString),
  have been added.
  These functions are useful for converting between flake references encoded as attribute sets and URLs.

- [`builtins.toJSON`](@docroot@/language/builtins.md#builtins-parseFlakeRef) now prints [--show-trace](@docroot@/command-ref/conf-file.html#conf-show-trace) items for the path in which it finds an evaluation error.

- Error messages regarding malformed input to [`derivation add`](@docroot@/command-ref/new-cli/nix3-derivation-add.md) are now clearer and more detailed.
