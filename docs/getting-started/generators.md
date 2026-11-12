---
title: "Generators"
weight: 8
---

Several of the framework classes have 'generators'.  

Some template classes in the ETL have specialisations to handle a variable numbers of template parameters. It is not possible to know beforehand what the maximum size will be required for any particular project, so the ETL supplies definitions with what seems to be a reasonable default size.  

For many of the classes this is set to 16. To enable the use of the library in projects that require a higher capacity, generators have been created to automatically create the source code for supporting N variants.

See Generators Tutorial for more information.

## Header generation with CMake

The supplied C++03 implementations support 16 handlers/types by default. To change the
fixed-arity limits, set any of the following CMake cache variables (leave a
variable empty to skip that group unless another group requires it):

| Variable | Generated headers |
| --- | --- |
| `ETL_NUMBER_OF_HANDLERS` | `fsm`, `message_router`, `message_packet` |
| `ETL_NUMBER_OF_TYPES` | `largest`, `smallest`, `type_lookup`, `type_select`, `variant_pool` |
| `ETL_IS_ONE_OF` | `type_traits` |

For example:

```sh
cmake -S . -B build -DETL_NUMBER_OF_HANDLERS=32
```

For `add_subdirectory` or `FetchContent`, set the variables before adding ETL:

```cmake
set(ETL_NUMBER_OF_HANDLERS 32 CACHE STRING "ETL handler capacity")
```

Unset type limits follow the handler count, and unset `is_one_of` limits follow
the type count, with a minimum of 16. Explicit limits below these requirements
fail configuration.

The generated files live under `etl/private` and implement the fixed-arity
C++03 paths; variadic implementations are unchanged.

All capacities must be decimal integers of at least 16.

Generation runs at configure time only when a capacity variable is nonempty.
CMake installs `cogapp==3.6.0` in `<ETL build directory>/generated/venv` and
reuses it on subsequent configures, leaving the host Python environment unchanged.

- **Requirements:** Python 3 with `venv`/pip support and initial package-index
  access. Default builds require neither Python nor downloads.
- **Offline use:** provide a Cog wheel directory through `PIP_FIND_LINKS` and
  set `PIP_NO_INDEX=1`.
- **Interpreter:** select `Python3_EXECUTABLE` with CMake 3.12 or later, or
  `PYTHON_EXECUTABLE` with older CMake versions.

Like `include/etl/generators/generate.bat`, generation overwrites the headers in
`include/etl/private/`. Application includes, CMake include paths and installation
remain unchanged. All builds sharing a checkout therefore share its generated
capacities; use separate checkouts for different configurations.

Clearing a setting stops requesting generation; it does not restore previously
generated files. To restore the supplied capacities, configure once with all three
settings explicitly set to `16`, then clear them. All translation units in a
program must use the same ETL configuration.

Run `python3 scripts/test_cmake_generation.py` for a smoke test of default
configuration and a 32-handler C++03 consumer. It requires Python 3 (including
`venv`), CMake and a C++ compiler; CMake provisions Cog as described above.
