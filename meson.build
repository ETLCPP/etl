project('etl',
    ['cpp'],
    default_options : [
        'warning_level=2',
        'werror=false',
        # `build.*` options affect `native: true targets`
        # plain options affect `native: false` targets.
        'cpp_std=c++17', 'build.cpp_std=c++17',
    ],
    meson_version: '>=0.57.0',
    version: files('version.txt')
)

compile_args = []
if not get_option('use_stl')
    compile_args += '-DETL_NO_STL'
endif

etl_dep = declare_dependency(
    include_directories: include_directories('include'),
    compile_args: compile_args
)

# Only register tests with the test runner when built as a primary project
if meson.is_subproject() == false
    subdir('test')
endif
