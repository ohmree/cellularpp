catch2_dep = dependency('catch2',
                        fallback : ['catch2', 'catch2_dep'])

test_example = executable('test_example', ['test_example.cpp'],
                           include_directories : include_dir,
                           dependencies : [catch2_dep])

test('Catch2 example test',
     test_example)
