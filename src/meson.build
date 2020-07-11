lib_src_files = []
if get_option('game_of_life').enabled()
  lib_src_files += 'game_of_life.cpp'
endif
cellularpp_lib = library('cellular++', lib_src_files, include_directories : lib_include_dirs)
cellularpp_dep = declare_dependency(link_with : cellularpp_lib, include_directories : lib_include_dirs)
subdir('bin')
