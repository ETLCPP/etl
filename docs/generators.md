# Code Generation for Pre-C++11 Support

ETL supports C++03 (also referred to as C++98) environments where variadic
templates, `constexpr`, and other modern features are unavailable.  To
provide equivalent functionality, certain headers are **generated** using
[Cog](https://nedbatchelder.com/code/cog/), a Python-based code generation
tool that embeds Python snippets inside source files.

This document explains how the code generation system works and how to
regenerate the headers if you modify a generator template.

---

## Overview

| Directory | Contents |
|---|---|
| `include/etl/generators/` | Generator templates (`*_generator.h`) and batch scripts |
| `include/etl/private/` | Generated output (`*_cpp03.h`) committed to the repository |
| `scripts/generator_test.py` | CI script that verifies generators match committed files |

The generator templates contain embedded Python code (delimited by `[[[cog`
and `]]]`) that produces the repetitive C++03 boilerplate.  Cog processes
these templates and writes the expanded output to `include/etl/private/`.

---

## Generated Headers

The following C++03 compatibility headers are generated:

| Generator | Output | Purpose |
|---|---|---|
| `fsm_fwd_decl_cpp03_generator.h` | `fsm_fwd_decl_cpp03.h` | FSM forward declarations |
| `fsm_friend_decl_cpp03_generator.h` | `fsm_friend_decl_cpp03.h` | FSM friend declarations |
| `fsm_cpp03_generator.h` | `fsm_cpp03.h` | Finite state machine implementation |
| `message_router_cpp03_generator.h` | `message_router_cpp03.h` | Message router |
| `message_packet_cpp03_generator.h` | `message_packet_cpp03.h` | Message packet |
| `largest_type_cpp03_generator.h` | `largest_type_cpp03.h` | Largest type metafunction |
| `largest_alignment_cpp03_generator.h` | `largest_alignment_cpp03.h` | Largest alignment metafunction |
| `largest_cpp03_generator.h` | `largest_cpp03.h` | Largest type/size utilities |
| `smallest_cpp03_generator.h` | `smallest_cpp03.h` | Smallest type/size utilities |
| `type_traits_cpp03_generator.h` | `type_traits_cpp03.h` | Type traits (`is_one_of`, etc.) |
| `type_lookup_cpp03_generator.h` | `type_lookup_cpp03.h` | Type lookup metafunction |
| `type_select_cpp03_generator.h` | `type_select_cpp03.h` | Type selection metafunction |
| `variant_pool_cpp03_generator.h` | `variant_pool_cpp03.h` | Variant pool |

---

## Generator Parameters

Cog variables control how many template parameter overloads are generated:

| Variable | Default | Used by |
|---|---|---|
| `Handlers` | 16 | FSM and message router generators |
| `NTypes` | 16 | Type utility generators (largest, smallest, lookup, select, variant pool) |
| `IsOneOf` | 16 | Type traits generator (`is_one_of`) |

These defaults produce overloads supporting up to 16 types or handlers,
which is sufficient for most embedded applications while keeping compile
times reasonable.

---

## Prerequisites

* **Python 3**
* **cogapp** – install via:

  ```bash
  pip install cogapp
  ```

---

## Regenerating Headers

### Using the batch scripts (Windows)

Each generator has a corresponding `.bat` file in `include/etl/generators/`:

```bat
cd include/etl/generators
generate.bat          # Regenerate all headers
generate_fsm.bat      # Regenerate FSM headers only
generate_smallest.bat # Regenerate smallest_cpp03.h only
# etc.
```

### Manual invocation

Run Cog directly from the `include/etl/generators/` directory:

```bash
cd include/etl/generators

# Example: regenerate smallest_cpp03.h
python3 -m cogapp -d -e -o../private/smallest_cpp03.h -DNTypes=16 smallest_cpp03_generator.h

# Example: regenerate fsm_cpp03.h
python3 -m cogapp -d -e -o../private/fsm_cpp03.h -DHandlers=16 fsm_cpp03_generator.h
```

Cog options used:

| Option | Meaning |
|---|---|
| `-d` | Delete the generator markers from output |
| `-e` | Warn if the input file has no generator markers |
| `-o<file>` | Write output to the specified file |
| `-D<var>=<value>` | Define a Cog variable |

### Regenerating all headers

The `generate.bat` script regenerates every header:

```bash
cd include/etl/generators
./generate.bat   # Windows
# or run the commands manually on Linux/macOS
```

On Linux/macOS you can run the commands from `generate.bat` directly in
your shell (they are standard `python3 -m cogapp` invocations).

---

## Verifying Generators

After modifying a generator template, verify the output matches the
committed file:

```bash
python3 scripts/generator_test.py
```

This script:

1. Runs Cog on every `*_generator.h` file.
2. Compares each output against the corresponding file in
   `include/etl/private/`.
3. Reports success or failure.

The `generator.yml` GitHub Actions workflow runs this automatically on
every push and pull request.

---

## How Generators Work

A generator template contains standard C++ code interspersed with Cog
directives.  For example, from `smallest_cpp03_generator.h`:

```cpp
/*[[[cog
import cog
cog.outl("template <typename T1, ")
for n in range(2, int(NTypes)):
    cog.out("typename T%s = void, " % n)
cog.outl("typename T%s = void>" % int(NTypes))
]]]*/
// Generated code appears here after running Cog
/*[[[end]]]*/
```

When Cog processes this file with `-DNTypes=16`, the Python code executes
and outputs the expanded template parameter list supporting 16 types.

---

## Adding a New Generator

1. Create `include/etl/generators/<name>_cpp03_generator.h` with Cog
   directives.
2. Add a corresponding entry to `generate.bat`.
3. Run `generate.bat` (or the equivalent Cog command) to produce
   `include/etl/private/<name>_cpp03.h`.
4. Commit both the generator and the generated output.
5. Verify with `python3 scripts/generator_test.py`.

---

## Troubleshooting

| Problem | Solution |
|---|---|
| `ModuleNotFoundError: No module named 'cogapp'` | Install Cog: `pip install cogapp` |
| Generator output differs from committed file | Regenerate and commit the updated output |
| Need more than 16 types/handlers | Change `-DNTypes=` or `-DHandlers=` and regenerate |
