solution 'eventStreamViewer'
    configurations {'Release', 'Debug'}
    location 'build'

    project 'eventStreamViewer'

        -- General settings
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/**.hpp', 'source/**.cpp'}

        -- Run moc and link to the Qt library (required only when using the Chameleon library displays)
        local mocFiles = { -- must contain Chameleon components paths
            '/usr/local/include/chameleon/backgroundCleaner.hpp',
            '/usr/local/include/chameleon/changeDetectionDisplay.hpp',
            '/usr/local/include/chameleon/logarithmicDisplay.hpp',
        }
        local mocCommand = '/usr/lib/x86_64-linux-gnu/qt5/bin/moc' -- must point to the moc executable
        local qtIncludeDirectory = '/usr/include/x86_64-linux-gnu/qt5' -- must point to the directory containing Qt's headers
        local qtLibDirectory = '/usr/lib/x86_64-linux-gnu' -- must point to the directory containing Qt's dynamic libraries
        local mocDirectory = path.getdirectory(_SCRIPT) .. '/build/moc'
        os.rmdir(mocDirectory)
        os.mkdir(mocDirectory)
        for index, mocFile in pairs(mocFiles) do
            if os.execute(mocCommand
                .. ' -I\'' .. qtIncludeDirectory .. '/QtQml\''
                .. ' -o \'' .. mocDirectory .. '/' .. path.getbasename(mocFile) .. '.cpp\''
                .. ' \''.. mocFile .. '\''
            ) ~= 0 then
                print(string.char(27) .. '[31mPre-compiling ' .. mocFile .. ' failed' .. string.char(27) .. '[0m')
                os.exit(1)
            end
            print(string.char(27) .. '[32m' .. mocFile .. ' was successfully pre-compiled' .. string.char(27) .. '[0m')
        end
        files {'build/moc/**.h', 'build/moc/**.cpp', 'source/**.qml'}
        includedirs {qtIncludeDirectory, qtIncludeDirectory .. '/QtQml'}
        configuration 'linux'
            libdirs {qtLibDirectory}
            links {'Qt5Core', 'Qt5Gui', 'Qt5Qml', 'Qt5Quick'}
            buildoptions {'-fPIC'}
        configuration 'macosx'
            linkoptions {
                '-F' .. qtLibDirectory,
                '-framework QtCore',
                '-framework QtGui',
                '-framework QtQml',
                '-framework QtQuick',
                '-framework QtQuickControls2',
            }
        configuration {}

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
