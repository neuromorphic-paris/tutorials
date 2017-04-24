solution 'eventStreamPrinter'
    configurations {'Release', 'Debug'}
    location 'build'

    project 'eventStreamPrinter'

        -- General settings
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/**.hpp', 'source/**.cpp'}

        -- Declare the configurations
        configuration 'Release'
            targetdir 'build/Release'
            defines {'NDEBUG'}
            flags {'OptimizeSpeed'}
        configuration 'Debug'
            targetdir 'build/Debug'
            defines {'DEBUG'}
            flags {'Symbols'}

        -- Linux specific settings
        configuration 'linux'
            buildoptions {'-std=c++11'}
            linkoptions {'-std=c++11'}
            includedirs {'/usr/local/include'}
            links {'pthread'}

        -- Mac OS X specific settings
        configuration 'macosx'
            buildoptions {'-std=c++11', '-stdlib=libc++'}
            linkoptions {'-std=c++11', '-stdlib=libc++'}
            includedirs {'/usr/local/include'}
