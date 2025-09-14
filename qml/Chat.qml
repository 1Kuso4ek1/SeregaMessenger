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
            delegate: RowLayout {
                Item {
                    Layout.preferredWidth: index % 2 === 0 ? 0 : messagesList.width - message.width;
                }

                Rectangle {
                    id: message
                    Layout.preferredWidth: content.implicitWidth + 20
                    Layout.preferredHeight: content.implicitHeight + 20
                    Layout.maximumWidth: messagesList.width * 0.8

                    Label {
                        id: content
                        anchors.centerIn: parent

                        width: parent.width - 20

                        text: "Lorem ipsum dolor sit amet, occaecat nostrud nostrud aliqua dolor aliquip"
                        wrapMode: Text.WordWrap

                        font.pixelSize: 16
                    }

                    color: index % 2 === 0 ? Material.color(Material.Blue) : Material.color(Material.DeepPurple, Material.Shade600)

                    radius: 20
                }
            }
        }
    }
}
