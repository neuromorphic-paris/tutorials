R""(
    import Chameleon 1.0
    import QtQuick 2.7
    import QtQuick.Controls 2.0
    import QtQuick.Layouts 1.1
    import QtQuick.Window 2.2
    Window {
        id: window
        visible: true
        width: header_width
        height: header_height + 90
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
        ColumnLayout {
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
            Slider {
                id: slider
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                value: 0.5
                Component.onCompleted: parameters.temporal_window = Math.round(Math.exp(Math.log(1e7) * position))
                onPositionChanged: {
                    parameters.temporal_window = Math.round(Math.exp(Math.log(1e7) * position))
                }
            }
            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                font.family: 'Helvetica'
                font.pointSize: 20
                text: parameters.temporal_window + ' µs'
                color: 'white'
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
            }
        }
    }
)""
