solution 'myProject'
    configurations {'Release', 'Debug'}
    location 'build'

    project 'myProject'

        -- General settings
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/**.hpp', 'source/**.cpp'}

        -- Run moc and link to the Qt library (required only when using the Chameleon library displays)
        local mocFiles = { -- must contain Chameleon components paths
            '/usr/local/include/chameleon/backgroundCleaner.hpp',
            '/usr/local/include/chameleon/changeDetectionDisplay.hpp',
        }
        local mocCommand = '/usr/local/Cellar/qt5/5.8.0_1/bin/moc' -- must point to the moc executable
        local qtIncludeDirectory = '/usr/local/Cellar/qt5/5.8.0_1/include' -- must point to the directory containing Qt's headers
        local qtLibDirectory = '/usr/local/Cellar/qt5/5.8.0_1/lib' -- must point to the directory containing Qt's dynamic libraries
        local mocDirectory = path.getdirectory(_SCRIPT) .. '/build/moc'
        os.rmdir(mocDirectory)
        os.mkdir(mocDirectory)
        for index, mocFile in pairs(mocFiles) do
            if os.execute(mocCommand
                .. ' -I' .. qtIncludeDirectory .. '/QtQml'
                .. ' -o ' .. mocDirectory .. '/' .. path.getbasename(mocFile) .. '.cpp '
                .. mocFile
            ) ~= 0 then
                print(string.char(27) .. '[31mPre-compiling ' .. mocFile .. ' failed' .. string.char(27) .. '[0m')
                os.exit(1)
            end
            print(string.char(27) .. '[32m' .. mocFile .. ' was successfully pre-compiled' .. string.char(27) .. '[0m')
        end
        files {'build/moc/**.h', 'build/moc/**.cpp', 'source/**.qml'}
        includedirs {qtIncludeDirectory, qtIncludeDirectory .. '/QtGui', qtIncludeDirectory .. '/QtQml'}
        configuration 'linux'
            libdirs {qtLibDirectory}
            links {'Qt5Core', 'Qt5Gui', 'Qt5Qml', 'Qt5Quick', 'Qt5QuickControls'}
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

        -- Define the include paths
        includedirs {'/usr/local/include'}
        libdirs {'/usr/local/lib'}

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
            links {'pthread'}

        -- Mac OS X specific settings
        configuration 'macosx'
            buildoptions {'-std=c++11', '-stdlib=libc++'}
            linkoptions {'-std=c++11', '-stdlib=libc++'}
