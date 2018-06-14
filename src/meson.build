programs = [
        # executable name, sources
        ['riech-o-mat-backend-serial', ['riech-o-mat-backend-serial.c']]
]

foreach prog : programs
        executable(
                prog[0],
                prog[1],
                dependencies : DEPS,
                install : true
        )
endforeach
