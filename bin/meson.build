gui_option = get_option('gui')
sdl2_dep = dependency('sdl2', required : gui_option)

bin_dependencies = []

if sdl2_dep.found()
  bin_dependencies += sdl2_dep
endif

src_files = ['main.cpp', 'game_of_life.cpp']

executable('cellular++',
           src_files,
           include_directories : include_dir,
           dependencies : bin_dependencies,
           gui_app : gui_option.enabled(),
		   install : true)
