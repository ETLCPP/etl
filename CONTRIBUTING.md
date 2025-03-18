# How to contribute

If your are considering creating a pull request, please observe the following:

- If you are adding or modifying a feature, add *new* unit tests that test that feature.
- If you are fixing a bug, add a unit test that *fails* before the bug fix is implemented.
- Do not initiate a pull request until all of the units tests pass.
- Branches should be based on the branch `master`.

There is a project file for VS2022 for C++14, 17, 20, and bash scripts that run the tests for C++11, 14, 17, 20 under Linux with GCC and Clang.

If you are thinking of adding a new feature then raise this on the GitHub Issues page for disccussion as the maintainers and user of the ETL may have questions or suggestions.
It is possible that the maintainer of the ETL or another contributor is already working on the same or a related feature.
