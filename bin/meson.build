cmake = import('cmake')

sdl2_dep = dependency('sdl2', fallback : ['sdl2', 'sdl2_dep'], required : get_option('sdl2'))

bin_dependencies = []

if sdl2_dep.found()
  bin_dependencies += sdl2_dep
endif

executable('cellular++',
		'main.cpp',
    include_directories : include_dir,
    dependencies : bin_dependencies,
		install : true)
