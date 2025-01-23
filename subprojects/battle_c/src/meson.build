src_files = ['battle_c.cpp', 'tcp_client.cpp', 'list.cpp', generated]

local_includes = include_directories('/usr/local/include')



battle_c_lib = static_library(
    'battle_c_lib',  # Name of the library (produces libbattle_c_lib.a)
    src_files,
    dependencies: [boost_dep, protobuf_dep],  # Link with dependencies
    include_directories: [inc_dir, local_includes]  # Include directories
)



# Make the library available to the parent project
battle_c_dep = declare_dependency(
  include_directories: [inc_dir, local_includes],
  link_with: battle_c_lib,
  dependencies: [protobuf_dep]
)

# Provide the dependency
meson.override_dependency('battle_c', battle_c_dep)

