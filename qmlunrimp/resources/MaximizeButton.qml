import QtQuick 2.0
Rectangle {
        id: maximize
        
        signal clicked()
        
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
        
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: maximize.clicked()
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