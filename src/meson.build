programs = [
        # executable name, sources
        ['riech-o-mat-backend-serial', [
                'crom-serial.c',
                'riech-o-mat-backend-serial.c'
        ]],
        ['cosy-riech-o-mat', [
                'cosy-riech-o-mat.c'
        ]]
]

foreach prog : programs
        executable(
                prog[0],
                prog[1],
                dependencies : DEPS,
                install : true
        )
endforeach
