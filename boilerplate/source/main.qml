import QtQuick 2.3
import BackgroundCleaner 1.0
import ChangeDetectionDisplay 1.0

Item {
    id: item
    visible: true
    width: 304
    height: 240

    Timer {
        interval: 20
        running: true
        repeat: true
        onTriggered: {
            backgroundCleaner.triggerDraw();
            changeDetectionDisplay.triggerDraw();
        }
    }

    BackgroundCleaner {
        id: backgroundCleaner
        width: item.width
        height: item.height
    }

    ChangeDetectionDisplay {
        objectName: "changeDetectionDisplay"
        id: changeDetectionDisplay
        width: item.width
        height: item.height
        canvasSize: "304x240"
    }
}
