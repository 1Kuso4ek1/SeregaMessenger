import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    ColumnLayout {
        id: root

        anchors.fill: parent
        anchors.centerIn: parent
        anchors.margins: parent.width * 0.1

        spacing: 10

        Label {
            text: "Serega"

            Layout.fillWidth: true

            font.pixelSize: 24
            font.bold: true

            horizontalAlignment: Text.AlignHCenter
        }

        ColumnLayout {
            spacing: 8

            TextField {
                id: username
                placeholderText: "Username"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter
            }

            RowLayout {
                Layout.fillWidth: true

                TextField {
                    id: password
                    placeholderText: "Password"

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter

                    echoMode: TextInput.Password
                }

                RoundButton {
                    id: showPasswordButton
                    text: "Show"

                    Layout.preferredWidth: 70

                    onClicked: {
                        if(password.echoMode === TextInput.Password) {
                            password.echoMode = TextInput.Normal
                            showPasswordButton.text = "Hide"
                        }
                        else {
                            password.echoMode = TextInput.Password
                            showPasswordButton.text = "Show"
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            spacing: 10

            Button {
                text: "Login"

                Layout.preferredWidth: 100
                Layout.fillWidth: true

                Material.background: Material.accent
            }

            Button {
                text: "Register"

                Layout.preferredWidth: 100
                Layout.fillWidth: true
            }
        }
    }
}
