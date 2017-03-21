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
        -- Edit the variable 'mocFiles' to add or remove Chameleon components
        -- Edit the qtPath variable to match your Qt version
        local mocFiles = {
            '/usr/local/include/chameleon/backgroundCleaner.hpp',
            '/usr/local/include/chameleon/changeDetectionDisplay.hpp',
        }
        local qtPath = '/usr/local/Cellar/qt5/5.8.0_1'
        local mocDirectory = path.getdirectory(_SCRIPT) .. '/build/moc'
        os.rmdir(mocDirectory)
        os.mkdir(mocDirectory)
        for index, mocFile in pairs(mocFiles) do
            if os.execute('moc'
                .. ' -I' .. qtPath .. '/include/QtQml'
                .. ' -o ' .. mocDirectory .. '/' .. path.getbasename(mocFile) .. '.cpp '
                .. mocFile
            ) ~= 0 then
                print(string.char(27) .. '[31mPre-compiling ' .. mocFile .. ' failed' .. string.char(27) .. '[0m')
                os.exit(1)
            end
            print(string.char(27) .. '[32m' .. mocFile .. ' was successfully pre-compiled' .. string.char(27) .. '[0m')
        end
        files {'build/moc/**.h', 'build/moc/**.cpp', 'source/**.qml'}
        includedirs {qtPath .. '/include', qtPath .. '/include/QtGui', qtPath .. '/include/QtQml'}
        configuration 'macosx'
            linkoptions {
                '-F' .. qtPath .. '/lib',
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
