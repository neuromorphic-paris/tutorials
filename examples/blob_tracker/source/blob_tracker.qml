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
                blob_display.trigger_draw();
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
            onPaintAreaChanged: {
                blob_display.x = paint_area.x
                blob_display.y = paint_area.y
                blob_display.width = paint_area.width
                blob_display.height = paint_area.height
            }
        }
        BlobDisplay {
            objectName: "blob_display"
            id: blob_display
            width: window.width
            height: window.height
            canvas_size: Qt.size(header_width, header_height)
            stroke_color: "#7eaa5f"
            stroke_thickness: 3
            fill_color: "#887eaa5f"
        }
    }
)""
