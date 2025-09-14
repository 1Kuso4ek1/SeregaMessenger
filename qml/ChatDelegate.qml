import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Pane {
    width: chatsList.width

    Material.background: Material.color(Material.Grey, Material.Shade900)

    RowLayout {
        anchors.fill: parent

        spacing: 10

        Avatar {
            Layout.alignment: Qt.AlignVCenter

            size: 50
        }

        ColumnLayout {
            spacing: 20

            RowLayout {
                Label {
                    Layout.fillWidth: true

                    text: "Hello"
                    horizontalAlignment: Qt.AlignLeft

                    font.bold: true
                }

                Label {
                    Layout.fillWidth: true

                    text: (new Date()).toLocaleString(Qt.locale(), "dd.MM.yyyy hh:mm");
                    horizontalAlignment: Qt.AlignRight

                    font.pixelSize: 12
                    color: "#606060"
                }
            }

            Label {
                Layout.fillWidth: true

                text: "Lorem ipsum dolor sit amet, occaecat nostrud nostrud aliqua dolor aliquip"

                elide: Text.ElideRight
            }
        }
    }
}
