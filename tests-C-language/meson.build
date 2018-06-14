programs = [
        # executable name, sources
        ['test-open-valve', ['test-open-valve.c']],
        ['test-parallel-port', ['test-parallel-port.c']]
]

foreach prog : programs
        executable(
                prog[0],
                prog[1],
                dependencies : DEPS,
                install : false
        )
endforeach
