catch2_dep = dependency('catch2',
                        fallback : ['catch2', 'catch2_dep'])

test_s = executable('test_s', ['test_s.cpp'],
                           include_directories : include_dirs,
                           dependencies : [catch2_dep, cellularpp_dep])

test('S test',
     test_s)
