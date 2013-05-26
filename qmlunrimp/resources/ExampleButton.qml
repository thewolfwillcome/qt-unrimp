import QtQuick 2.0
Item {
    property alias text : text_element.text
    property string exampleName: ""
	property QtObject exampleItem: null
	
	width: text_element.paintedWidth+8
	height: 25
	
	id: example_Button
	Rectangle {
        
        width: example_Button.width
        height: example_Button.height
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
        anchors.top: parent.top
        anchors.right: parent.right
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: { example_Button.exampleItem.exampleName = example_Button.exampleName }
        }

        Text {
            id:text_element
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
                visible: example_Button.exampleItem.exampleName === example_Button.exampleName
            }
         }
        border.width: 2
    }
}
