local qt = require 'third_party/chameleon/qt'

solution 'events_to_frames'
    configurations {'release', 'debug'}
    location 'build'
    project 'events_to_frames'
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/*.qml', 'source/*.cpp'}
        buildoptions {'-std=c++11'}
        linkoptions {'-std=c++11'}
        files(qt.moc({
            'third_party/chameleon/source/background_cleaner.hpp',
            'third_party/chameleon/source/dvs_display.hpp',
            'third_party/chameleon/source/t_delta_display.hpp',
            'third_party/chameleon/source/frame_generator.hpp'},
            'build/moc'))
        includedirs(qt.includedirs())
        libdirs(qt.libdirs())
        links(qt.links())
        buildoptions(qt.buildoptions())
        linkoptions(qt.linkoptions())
        configuration 'release'
            targetdir 'build/release'
            defines {'NDEBUG'}
            flags {'OptimizeSpeed'}
        configuration 'debug'
            targetdir 'build/debug'
            defines {'DEBUG'}
            flags {'Symbols'}
        configuration 'linux'
            links {'pthread'}
