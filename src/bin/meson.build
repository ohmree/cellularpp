gui_option = get_option('gui')
sdl2_dep = dependency('sdl2', required : true)

bin_dependencies = [cellularpp_dep]

if sdl2_dep.found()
  bin_dependencies += sdl2_dep
endif

bin_src_files = ['main.cpp']

executable('cellular++',
           bin_src_files,
           dependencies : bin_dependencies,
           gui_app : true,
		   install : true)
