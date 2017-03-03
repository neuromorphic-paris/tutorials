import QtQuick 2.3
import QtQuick.Layouts 1.1
import BackgroundCleaner 1.0
import ChangeDetectionDisplay 1.0
import FlowDisplay 1.0

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
            flowDisplay.triggerDraw();
        }
    }

    BackgroundCleaner {
        id: backgroundCleaner
        width: item.width
        height: item.height
        color: "#333333"
    }

    ChangeDetectionDisplay {
        objectName: "changeDetectionDisplay"
        id: changeDetectionDisplay
        width: item.width
        height: item.height
        canvasSize: "304x240"
        backgroundColor: "#333333"
    }

    FlowDisplay {
        objectName: "flowDisplay"
        id: flowDisplay
        x: changeDetectionDisplay.x
        y: changeDetectionDisplay.y
        width: changeDetectionDisplay.width
        height: changeDetectionDisplay.height
        canvasSize: "304x240"
    }
}
