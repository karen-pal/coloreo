project('Coloreo de grafos',
        'c',
        version : '0.1',
        default_options : [
          'warning_level=3',
          'c_std=c99'
          ])

add_project_arguments('-DLOG_USE_COLOR', '-Wstrict-prototypes', '-Wunreachable-code', '-Wconversion', '-Wshadow', '-fstack-protector-strong',language: 'c')

# Execute the build file inside this dir, keeps this one cleaner
subdir('CangrejoEstelar')
