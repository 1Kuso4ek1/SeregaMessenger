import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    width: 640
    height: 480

    minimumWidth: 320
    minimumHeight: 240

    flags: Qt.Window | Qt.ExpandedClientAreaHint | Qt.NoTitleBarBackgroundHint

    visible: true
    title: "Serega"

    Material.theme: Material.System
    Material.accent: Material.color(Material.DeepPurple, Material.Shade600)

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Chats { userId: 1 }

        Component.onCompleted: Navigation.stackView = stackView
    }

    onClosing: (close) => {
        close.accepted = stackView.depth === 1

        if(!close.accepted)
            stackView.pop()
    }
}
