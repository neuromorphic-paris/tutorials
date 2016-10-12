import QtQuick 2.3
import QtQuick.Layouts 1.1
import BackgroundCleaner 1.0
import ChangeDetectionDisplay 1.0
import LogarithmicDisplay 1.0
import Recorder 1.0

Item {
    visible: true
    width: 304 * 2 + 10 * 3
    height: 240 + 10 * 2

    Timer {
        interval: 20
        running: true
        repeat: true
        onTriggered: {
            backgroundCleaner.triggerDraw();
            changeDetectionDisplay.triggerDraw();
            logarithmicDisplay.triggerDraw();
            recorder.triggerDraw();
        }
    }

    Recorder {
        objectName: "recorder"
        id: recorder
        interval: 40000
        initialTimestamp: 0
        anchors.fill: parent
        directory: "/path/to/the/recorded/frames/directory"
        onRendered: {
            var discards = logarithmicDisplay.discards;
            console.log('Shot index:', shotIndex, ', timestamp:', timestamp, ', black discard:', discards.x, ', white discard:', discards.y);
            recorder.acknowledgeRender();
        }
    }

    BackgroundCleaner {
        id: backgroundCleaner
        anchors.fill: parent
        color: "#000000"
    }

    RowLayout {
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.fill: parent
        spacing: 10

        ChangeDetectionDisplay {
            objectName: "changeDetectionDisplay"
            id: changeDetectionDisplay
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            canvasSize: "304x240"
            decay: 5e5
            initialTimestamp: 0
        }

        LogarithmicDisplay {
            objectName: "logarithmicDisplay"
            id: logarithmicDisplay
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            canvasSize: "304x240"
            discardRatio: 0.01
            colormap: LogarithmicDisplay.Heat
        }
    }
}
