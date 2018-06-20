R""(
    import Chameleon 1.0
    import QtQuick 2.3
    import QtQuick.Layouts 1.1
    import QtQuick.Window 2.2
    Window {
        id: window
        visible: true
        width: header_width * 2
        height: header_height
        Timer {
            interval: 20
            running: true
            repeat: true
            onTriggered: {
                dvs_display.trigger_draw();
                delta_t_display.trigger_draw();
            }
        }
        BackgroundCleaner {
            width: window.width
            height: window.height
        }
        RowLayout {
            width: window.width
            height: window.height
            spacing: 0
            DvsDisplay {
                objectName: "dvs_display"
                id: dvs_display
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                canvas_size: Qt.size(header_width, header_height)
            }
            DeltaTDisplay {
                objectName: "delta_t_display"
                id: delta_t_display
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                canvas_size: Qt.size(header_width, header_height)
            }
        }
    }
)""
