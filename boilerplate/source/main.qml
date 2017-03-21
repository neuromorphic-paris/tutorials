import QtQuick 2.3
import QtQuick.Layouts 1.1
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
        color: "#657b83"
    }

    ChangeDetectionDisplay {
        objectName: "changeDetectionDisplay"
        id: changeDetectionDisplay
        width: item.width
        height: item.height
        canvasSize: "304x240"
        backgroundColor: "#657b83"
        increaseColor: "#fdf6e3"
        idleColor: "#657b83"
        decreaseColor: "#002b36"
    }
}
