automata_include_dir = include_directories('.')
game_of_life_dep = declare_dependency(sources : 'game_of_life.cpp',
                                      dependencies : cellularpp_dep,
                                      include_directories : automata_include_dir)

wireworld_dep = declare_dependency(sources : 'wireworld.cpp',
                                   dependencies : cellularpp_dep,
                                   include_directories : automata_include_dir)

if get_option('gui').enabled()
  subdir('bin')
endif
