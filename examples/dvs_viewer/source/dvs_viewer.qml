R""(
    import Chameleon 1.0
    import QtQuick 2.7
    import QtQuick.Window 2.2
    Window {
        id: window
        visible: true
        width: header_width
        height: header_height
        Timer {
            interval: 20
            running: true
            repeat: true
            onTriggered: {
                dvs_display.trigger_draw();
            }
        }
        BackgroundCleaner {
            width: window.width
            height: window.height
        }
        DvsDisplay {
            objectName: "dvs_display"
            id: dvs_display
            width: window.width
            height: window.height
            canvas_size: Qt.size(header_width, header_height)
        }
    }
)""
