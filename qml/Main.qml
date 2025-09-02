import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    width: 640
    height: 480

    minimumWidth: 320
    minimumHeight: 240

    visible: true
    title: "Serega"

    Material.theme: Material.System

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Login{}

        Component.onCompleted: Navigation.stackView = stackView
    }

    onClosing: (close) => {
        close.accepted = stackView.depth === 1

        if(!close.accepted)
            stackView.pop()
    }
}
