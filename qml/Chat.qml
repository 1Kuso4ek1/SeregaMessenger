import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    required property string chatName

    ColumnLayout {
        id: root
        spacing: 0

        anchors.fill: parent

        ToolBar {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 54

            Material.primary: Material.color(Material.Grey, Material.Shade900)
            Material.elevation: 6

            z: 1

            RoundButton {
                id: backButton

                text: "←"

                font.pixelSize: 20
                font.bold: true

                onClicked: Navigation.pop()
            }

            RowLayout {
                anchors.centerIn: parent

                Avatar {}

                Label {
                    text: chatName

                    font.pixelSize: 18
                    font.bold: false

                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }
        }

        ListView {
            id: messagesList
            spacing: 6

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            model: ([ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ])
            delegate: Message{}
        }
    }
}
