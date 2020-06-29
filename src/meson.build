libcellularpp = library('libcellular++', 
		sources : ['cellular.cpp'],
		include_directories : include_dir)

cellularpp_dep = declare_dependency(link_with : libcellularpp,
		include_directories : include_dir)
subdir('bin')
