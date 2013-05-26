import QtQuick 2.0
import Unrimp 1.0

Rectangle {
    id: unrimp
    width: 1024
    height: 768
    color: "black"

    Image {
        id: back
        anchors.fill: parent
        source: "GrassandSky.png"
        Behavior on opacity { NumberAnimation { } }
    }


    UnrimpItem {
        id: unrimpitem
        width: 600; height: 400
        anchors.left: move_handle1.left
        anchors.leftMargin: -5
        anchors.top: move_handle1.bottom
        anchors.topMargin: 6
        
        exampleName: "FirstTriangle"


        Behavior on opacity { NumberAnimation { } }
        Behavior on width { NumberAnimation { } }
        Behavior on height { NumberAnimation { } }

        states: [
            State {
                name: "State1"

                PropertyChanges {
                    target: unrimpitem
                    width: unrimp.width
                    height: unrimp.height
                }
                PropertyChanges {
                    target: move_handle1
                    x: 5
                    y: -move_handle1.height - 6
                }

                PropertyChanges {
                    target: changeAA_handle1
                    anchors.top: unrimpitem.top
                    anchors.topMargin: 5
                }
                PropertyChanges {
                    target: toolbar3
                    anchors.top: unrimpitem.top
                    anchors.topMargin: 5
                }
                PropertyChanges {
                    target: back
                    opacity: 0
                }
            }
        ]
    }

    Rectangle {
        id: move_handle1
        x: 200
        y: 200
        width: 25
        height: 25
        radius: 5
        Behavior on x { NumberAnimation { } }
        Behavior on y { NumberAnimation { } }

        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }

        border.width: 2
        border.color: "#1a1a1a"

        Image {
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent
            smooth: true
            fillMode: "Stretch"
            source: "move.gif"
        }

        MouseArea {
            anchors.fill: parent
            drag.target: move_handle1
            drag.axis: "XandYAxis"
            drag.minimumX: 0
            drag.minimumY: 0
            drag.maximumX: unrimp.width - move_handle1.width
            drag.maximumY: unrimp.height - move_handle1.height
        }
    }

    Rectangle {
        id: showhide_handle1
        width: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.left: move_handle1.right
        anchors.leftMargin: 6
        anchors.top: move_handle1.top
        anchors.bottom: move_handle1.bottom
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent

            onClicked: unrimpitem.opacity = unrimpitem.opacity == 1 ? 0 : 1
        }

        Rectangle {
            id: toolbar22
            x: 0
            y: -2
            radius: 12
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#5a5a5a"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            rotation: -35
            anchors.rightMargin: 6
            anchors.bottomMargin: 6
            anchors.leftMargin: 6
            anchors.topMargin: 6
            anchors.fill: parent
        }
        border.width: 2
    }

    Rectangle {
        id: changeAA_handle1
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: move_handle1.top
        anchors.right: toolbar3.left
        anchors.rightMargin: 6
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: { unrimpitem.smooth = !unrimpitem.smooth }
        }

        Text {
            anchors.fill: parent
            text: "AA"
            font.bold: true
            font.pixelSize: parent.height * 0.55
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            Rectangle {
                height: parent.height
                width: 2
                anchors.centerIn: parent
                color: "#BB1111"
                rotation: 40
                visible: !unrimpitem.smooth
            }
         }
        border.width: 2
    }
    
    Rectangle {
        id: example1_handle1
        width: 30
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: move_handle1.top
        anchors.right: example2_handle1.left
        anchors.rightMargin: 6
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: { unrimpitem.exampleName = "FirstTriangle" }
        }

        Text {
            anchors.fill: parent
            text: "EX1"
            font.bold: true
            font.pixelSize: parent.height * 0.55
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            Rectangle {
                height: parent.height
                width: 2
                anchors.centerIn: parent
                color: "#BB1111"
                rotation: 40
                visible: unrimpitem.exampleName != "FirstTriangle"
            }
         }
        border.width: 2
    }
    
    
    
    Rectangle {
        id: example2_handle1
        width: 30
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: move_handle1.top
        anchors.right: changeAA_handle1.left
        anchors.rightMargin: 6
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: { unrimpitem.exampleName = "VertexBuffer" }
        }

        Text {
            anchors.fill: parent
            text: "EX2"
            font.bold: true
            font.pixelSize: parent.height * 0.55
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            Rectangle {
                height: parent.height
                width: 2
                anchors.centerIn: parent
                color: "#BB1111"
                rotation: 40
                visible: unrimpitem.exampleName != "VertexBuffer"
            }
         }
        border.width: 2
    }

    Rectangle {
        id: toolbar3
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: move_handle1.top
        anchors.right: unrimpitem.right
        anchors.rightMargin: 5
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: { unrimpitem.state = unrimpitem.state == '' ? 'State1' : '' }
        }

        Rectangle {
            id: toolbar31
            color: "#28ffffff"
            radius: 2
            border.width: 2
            border.color: "#000000"
            anchors.rightMargin: 7
            anchors.leftMargin: 7
            anchors.topMargin: 7
            anchors.bottomMargin: 7
            anchors.fill: parent

            Rectangle {
                id: toolbar311
                height: 3
                color: "#000000"
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
            }
        }
        border.width: 2
    }

    Item {
        id: camerawrapper
        property real yaw: 0
        property real pitch: 0
        property real zoom: 1

        onYawChanged: unrimpitem.camera.yaw = yaw
        onPitchChanged: unrimpitem.camera.pitch = pitch
        onZoomChanged: unrimpitem.camera.zoom = zoom

        Behavior on yaw { NumberAnimation{ } }
        Behavior on pitch { NumberAnimation{ } }
        Behavior on zoom { NumberAnimation{ } }
    }

    Rectangle {
        id: rectangle1
        width: 139
        height: 208
        radius: 15
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#6f6f6f"
            }

            GradientStop {
                position: 0.24
                color: "#141414"
            }

            GradientStop {
                position: 1
                color: "#50000000"
            }
        }
        anchors.left: rectangle2.left
        anchors.leftMargin: -5
        anchors.top: rectangle2.bottom
        anchors.topMargin: 6
        border.width: 4
        border.color: "#1a1a1a"
        clip: false

        Behavior on opacity { PropertyAnimation { } }

        Image {
            id: image1
            width: 135
            height: 31
            anchors.top: parent.top
            anchors.topMargin: 9
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            fillMode: "PreserveAspectFit"
            smooth: true
            source: "qt-logo.png"
        }

        Item {
            id: rectangle3
            x: 89
            y: 95
            width: 30
            height: 30
            anchors.horizontalCenterOffset: 35
            anchors.centerIn: image7

            MouseArea {
                id: mouse_area1
                anchors.fill: parent

                onPressed: camerawrapper.yaw += 20
            }

            Image {
                id: image5
                rotation: -90
                smooth: true
                anchors.fill: parent
                source: "arrow.png"
            }
        }

        Item {
            id: rectangle4
            x: 27
            y: 95
            width: 30
            height: 30
            anchors.horizontalCenterOffset: -35
            anchors.centerIn: image7
            MouseArea {
                id: mouse_area2
                anchors.fill: parent

                onPressed: camerawrapper.yaw -= 20
            }

            Image {
                id: image4
                rotation: 90
                smooth: true
                anchors.fill: parent
                source: "arrow.png"
            }
        }

        Item {
            id: rectangle5
            y: 64
            width: 30
            height: 30
            anchors.verticalCenterOffset: -35
            anchors.centerIn: image7
            MouseArea {
                id: mouse_area3
                anchors.fill: parent

                onPressed: camerawrapper.pitch -= 20
            }

            Image {
                id: image3
                rotation: 180
                smooth: true
                anchors.fill: parent
                source: "arrow.png"
            }
        }

        Item {
            id: rectangle6
            x: 58
            y: 124
            width: 30
            height: 30
            anchors.verticalCenterOffset: 35
            anchors.centerIn: image7
            anchors.horizontalCenterOffset: 0
            MouseArea {
                id: mouse_area4
                x: 0
                y: -1
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.fill: parent

                onPressed: camerawrapper.pitch += 20
            }

            Image {
                id: image6
                smooth: true
                anchors.fill: parent
                source: "arrow.png"
            }
        }

        Image {
            id: image7
            x: 84
            y: 0
            width: 24
            height: 24
            anchors.verticalCenterOffset: 9
            anchors.centerIn: parent
            smooth: true
            source: "circle.png"

            MouseArea {
                id: mouse_area5
                x: 0
                y: -1
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onClicked: { camerawrapper.yaw = 0; camerawrapper.pitch = 0 }
            }
        }

        Image {
            id: image8
            x: 20
            y: 182
            width: 30
            height: 30
            anchors.bottomMargin: 9
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: 14
            anchors.horizontalCenter: rectangle4.horizontalCenter
            smooth: true
            source: "minus.png"

            MouseArea {
                id: mouse_area6
                x: 0
                y: -1
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onPressed: camerawrapper.zoom /= 1.3
            }
        }

        Image {
            id: image9
            width: 30
            height: 30
            anchors.horizontalCenterOffset: -14
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 9
            anchors.horizontalCenter: rectangle3.horizontalCenter
            smooth: true
            source: "plus.png"

            MouseArea {
                id: mouse_area7
                x: 0
                y: 0
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onPressed: camerawrapper.zoom *= 1.3
            }
        }
    }

    Rectangle {
        id: rectangle2
        x: 31
        y: 269
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }

        border.width: 2
        border.color: "#1a1a1a"

        Image {
            id: image2
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent
            smooth: true
            fillMode: "Stretch"
            source: "move.gif"
        }

        MouseArea {
            anchors.fill: parent
            drag.target: rectangle2
            drag.axis: "XandYAxis"
            drag.minimumX: 0
            drag.minimumY: 0
            drag.maximumX: unrimp.width - rectangle2.width
            drag.maximumY: unrimp.height - rectangle2.height
        }
    }

    Rectangle {
        id: rectangle10
        width: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.left: rectangle2.right
        anchors.leftMargin: 6
        anchors.top: rectangle2.top
        anchors.bottom: rectangle2.bottom
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            drag.minimumY: 0
            drag.axis: "XandYAxis"
            drag.minimumX: 0
            drag.target: rectangle10
            drag.maximumY: unrimp.height - rectangle10.height
            drag.maximumX: unrimp.width - rectangle10.width

            onClicked: unrimp.state = unrimp.state == '' ? 'State1' : ''
        }

        Rectangle {
            id: rectangle11
            x: 0
            y: -2
            radius: 12
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#5a5a5a"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            rotation: -35
            anchors.rightMargin: 6
            anchors.bottomMargin: 6
            anchors.leftMargin: 6
            anchors.topMargin: 6
            anchors.fill: parent
        }
        border.width: 2
    }

    Rectangle {
        id: rectangle12
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: rectangle2.top
        anchors.right: rectangle1.right
        anchors.rightMargin: 5
        border.color: "#1a1a1a"

        MouseArea {
            property bool fullscreen: false
            anchors.fill: parent
            onClicked: {
                if (fullscreen)
                    Window.showNormal();
                else
                    Window.showFullScreen();
                fullscreen = !fullscreen;
            }
        }

        Rectangle {
            id: rectangle13
            color: "#28ffffff"
            radius: 2
            border.width: 2
            border.color: "#000000"
            anchors.rightMargin: 7
            anchors.leftMargin: 7
            anchors.topMargin: 7
            anchors.bottomMargin: 7
            anchors.fill: parent

            Rectangle {
                id: rectangle14
                height: 3
                color: "#000000"
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
            }
        }
        border.width: 2
    }
    states: [
        State {
            name: "State1"

            PropertyChanges {
                target: rectangle1
                opacity: 0
            }
        }
    ]
}
