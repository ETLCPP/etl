---
title: Source code formatting
weight: 1
---

This project uses **clang-format** (version 18) to enforce a consistent coding style
for C and C++ source files. For convenience, **treefmt** is also configured as a
single-command wrapper that discovers and formats every file in the tree.

---

## clang-format

### Configuration file

The formatting rules live in [`.clang-format`](../.clang-format) at the repository
root. The style is based on **LLVM**.

See the `.clang-format` file itself for the complete list.

### Version requirement

clang-format **18** is required.
The helper script [`scripts/clang-format-wrapper`](../scripts/clang-format-wrapper)
automatically resolves the correct binary: it first looks for `clang-format-18` on
`PATH`, then falls back to `clang-format` and verifies that its major version is 18.
All other tooling in the repo calls this wrapper instead of `clang-format` directly.

### Running clang-format manually

Format every tracked source file in the repository:

```bash
git ls-files -z \
            '*.c' '*.cc' '*.cpp' \
            '*.h' '*.hh' '*.hpp' \
            ':(exclude)include/etl/generators/*' | xargs -0 scripts/clang-format-wrapper -i --verbose --style=file
```

You can also format individual files directly:

```bash
scripts/clang-format-wrapper -i --style=file path/to/file.cpp
```

---

## treefmt

[treefmt](https://treefmt.com) is a language-agnostic source-tree formatter.
It reads a single configuration file and dispatches each file to the appropriate
formatter. In this project, it delegates all C/C++ formatting to the same
`clang-format-wrapper` described above.

In comparison to calling clang-format directly, it brings a significant speedup.

### Configuration file

The configuration lives in [`.treefmt.toml`](../.treefmt.toml) at the repository root.

### Installing treefmt

treefmt is a standalone Go binary. Install it with any of:

```bash
# Using the official install script
curl -fsSL https://raw.githubusercontent.com/numtide/treefmt/main/install.sh | bash

# Or via Homebrew
brew install treefmt

# Or via Nix
nix profile install nixpkgs#treefmt2
```

See the [treefmt documentation](https://treefmt.com) for more options.

### Running treefmt

From the repository root:

```bash
# Format everything
treefmt

# Check formatting without modifying files (useful in CI)
treefmt --fail-on-change
```

---

## Excluded paths

`.treefmt.toml` excludes generated files under
`include/etl/generators/`. Do **not** format those files manually via clang-format or treefmt.

## Pre-commit

Before submitting a PR / contribution, run `treefmt --fail-on-change` to catch
unformatted code before merge.

Alternatively, a plain `treefmt` automatically fixes any issues.