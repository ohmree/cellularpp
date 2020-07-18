sdl2_dep = dependency('sdl2', required : true)

bin_dependencies = [cellularpp_dep, sdl2_dep]

executable('game_of_life',
           'game_of_life_gui.cpp',
           dependencies : [bin_dependencies, game_of_life_dep],
           gui_app : true,
		   install : true)

executable('wireworld',
           'wireworld_gui.cpp',
           dependencies : [bin_dependencies, wireworld_dep],
           gui_app : true,
		   install : true)
