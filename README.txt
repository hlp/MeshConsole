
# Try the default to settings (with no agrs) first to get a sense of the size:

./MeshConsole.exe

# This makes a less dense mesh

./MeshConsole.exe -cell_size 10.0 -facet_size 4.0

# This is the suggested way to get the cool looking thing on the website, but it won't finish computing on my computer

./MeshConsole.exe -cell_size 10.0 -facet_size 4.0 -facet_distance 0.01 -cell_radius_edge 0.01

