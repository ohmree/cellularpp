project('cellular++', 'cpp',
		version : '0.1',
		default_options : ['warning_level=3', 'cpp_std=c++2a', 'b_ndebug=if-release'])

# MDSpan
cmake = import('cmake')
mdspan = cmake.subproject('mdspan')
mdspan_dep = mdspan.dependency('mdspan')

# Library
cellularpp_include_dirs = include_directories('include')
cellularpp_dep = declare_dependency(dependencies : mdspan_dep, include_directories : cellularpp_include_dirs)

# Tests
doctest_dep = dependency('doctest',
                         fallback : ['onqtam-doctest', 'doctest_dep'])
subdir('src')
subdir('tests')
