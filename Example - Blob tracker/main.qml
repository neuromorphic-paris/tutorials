import QtQuick 2.3
import QtQuick.Layouts 1.1
import BackgroundCleaner 1.0
import ChangeDetectionDisplay 1.0
import BlobDisplay 1.0

Item {
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
            hiddenBlobDisplay.update();
            blobDisplay.update();
        }
    }

    BackgroundCleaner {
        id: backgroundCleaner
        anchors.fill: parent
        color: "#000000"
    }

    ChangeDetectionDisplay {
        objectName: "changeDetectionDisplay"
        id: changeDetectionDisplay
        anchors.fill: parent
        canvasSize: "304x240"
        decay: 5e5
        initialTimestamp: 0
        onPaintAreaChanged: {
            blobDisplay.width = paintArea.width
            blobDisplay.height = paintArea.height
            blobDisplay.x = paintArea.x
            blobDisplay.y = paintArea.y
            hiddenBlobDisplay.width = paintArea.width
            hiddenBlobDisplay.height = paintArea.height
            hiddenBlobDisplay.x = paintArea.x
            hiddenBlobDisplay.y = paintArea.y
        }
    }

    BlobDisplay {
        objectName: "hiddenBlobDisplay"
        id: hiddenBlobDisplay
        canvasSize: "304x240"
        strokeColor: "#ff5b5a"
        strokeThickness: 1
        fillColor: "#33ff5b5a"
    }

    BlobDisplay {
        objectName: "blobDisplay"
        id: blobDisplay
        canvasSize: "304x240"
        strokeColor: "#00cb56"
        strokeThickness: 1
        fillColor: "#3300cb56"
    }
}
