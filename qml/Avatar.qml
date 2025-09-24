import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Effects

Item {
    property var avatarUrl: ""
    property int size: 32

    width: size
    height: size

    Image {
        id: avatar

        source: avatarUrl || "qrc:/resources/icons/profile-pic.png"

        anchors.fill: parent

        fillMode: Image.PreserveAspectCrop
        mipmap: true

        visible: false
    }

    MultiEffect {
        source: avatar

        anchors.fill: parent

        maskEnabled: true
        maskThresholdMin: 0.5 // https://forum.qt.io/post/815710
        maskSpreadAtMin: 1.0
        maskSource: mask
    }

    Rectangle {
        id: mask

        width: size
        height: size

        layer.enabled: true
        layer.smooth: true

        anchors.fill: parent

        radius: size / 2
        visible: false

        color: "white"
    }
}
