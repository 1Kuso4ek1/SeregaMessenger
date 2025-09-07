import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    Connections {
        target: SeregaApp.api.requestHandler

        function onErrorOccurred(error) {
            errorLabel.opacity = 1.0
            console.error(error)
        }
    }

    Connections {
        target: SeregaApp.api.auth

        function onUserLoggedIn() {
            console.log("success")
        }
    }

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

                onTextChanged: errorLabel.opacity = 0.0
            }

            RowLayout {
                Layout.fillWidth: true

                TextField {
                    id: password
                    placeholderText: "Password"

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter

                    echoMode: TextInput.Password
                    onTextChanged: errorLabel.opacity = 0.0
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

                onClicked: SeregaApp.api.auth.login(username.text, password.text)
            }

            Button {
                text: "Register"

                Layout.preferredWidth: 100
                Layout.fillWidth: true

                onClicked: SeregaApp.api.auth.registerUser(username.text, password.text)
            }
        }

        Label {
            id: errorLabel
            Layout.fillWidth: true

            horizontalAlignment: Text.AlignHCenter

            text: "Failed to log in"
            color: "red"
            opacity: 0.0

            Behavior on opacity { NumberAnimation { easing.type: Easing.InOutBack; duration: 1000 } }
        }
    }
}
