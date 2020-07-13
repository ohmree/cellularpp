gui_option = get_option('gui')
sdl2_dep = dependency('sdl2', required : true)

bin_dependencies = [cellularpp_dep]

if sdl2_dep.found()
  bin_dependencies += sdl2_dep
endif


executable('game_of_life',
           'game_of_life_gui.cpp',
           dependencies : bin_dependencies,
           gui_app : gui_option.enabled(),
		   install : true)

executable('wireworld',
           'wireworld_gui.cpp',
           dependencies : bin_dependencies,
           gui_app : gui_option.enabled(),
		   install : true)
