# How to contribute

Thanks for considering a contribution! Here’s what you need to know before opening a pull request:

- If you are adding or modifying a feature, add *new* unit tests that test that feature.
- If you are fixing a bug, add a unit test that *fails* before the bug fix is implemented.
- Do not initiate a pull request until all of the units tests pass. See below for information on project files and test scripts.
- Branches should be based on the branch `master`. If `development` has pending updates, I’ll rebase the PR against it before pulling..

There is a project file for VS2022 for C++14, 17, 20, 23, and bash scripts that run the tests for C++11, 14, 17, 20, 23 under Linux with GCC and Clang.
There are syntax-only check bash scripts that cover C++03, 11, 14, 17, 20, 23 under Linux with GCC and Clang.

If you are thinking of adding a new feature then raise this on the GitHub Issues page for discussion as the maintainers and user of the ETL may have questions or suggestions.
It is possible that the maintainer of the ETL or another contributor is already working on the same or a related feature. 
  
Take a look through our current issues and see if anything sparks your interest!
