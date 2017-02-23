import QtQuick 2.3
import QtQuick.Layouts 1.1
import BackgroundCleaner 1.0
import ChangeDetectionDisplay 1.0
import LogarithmicDisplay 1.0

Item {
    id: item
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
        }
    }

    BackgroundCleaner {
        id: backgroundCleaner
        width: item.width
        height: item.height
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
        }

        LogarithmicDisplay {
            objectName: "logarithmicDisplay"
            id: logarithmicDisplay
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            canvasSize: "304x240"
        }
    }
}
