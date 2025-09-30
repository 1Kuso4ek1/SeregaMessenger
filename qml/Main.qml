import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    width: 640
    height: 480

    minimumWidth: 320
    minimumHeight: 240

    flags: Qt.ExpandedClientAreaHint

    visible: true
    title: "Serega"

    header: ToolBar { // For the status bar background on mobile
        implicitHeight: parent.SafeArea.margins.top

        Material.primary: Material.color(Material.Grey, Material.Shade900)
    }

    Material.theme: Material.Dark
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
