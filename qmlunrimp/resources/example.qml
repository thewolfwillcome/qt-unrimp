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
                    target: maximize_handle1
                    anchors.top: unrimpitem.top
                    anchors.topMargin: 5
                }
                
                PropertyChanges {
                    target: example1_handle1
                    anchors.top: unrimpitem.top
                    anchors.topMargin: 5
                }
                PropertyChanges {
                    target: example2_handle1
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
	
	MoveItem {
		id: move_handle1
		x: 200
		y: 200
		window: unrimp
		
		Behavior on x { NumberAnimation { } }
		Behavior on y { NumberAnimation { } }
	}

	ShowHideButton {
		id: showhide_handle1
		
		anchors.left: move_handle1.right
		anchors.leftMargin: 6
		anchors.top: move_handle1.top
		onClicked: unrimpitem.opacity = unrimpitem.opacity == 1 ? 0 : 1
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
        anchors.right: maximize_handle1.left
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
    
	ExampleButton {
		id: example1_handle1
		text: "Ext1"
		anchors.top: move_handle1.top
		anchors.right: example2_handle1.left
        anchors.rightMargin: 6
		exampleName: "FirstTriangle"
		exampleItem: unrimpitem
	}
	
	ExampleButton {
		id: example2_handle1
		text: "Ext2"
		anchors.top: move_handle1.top
		anchors.right: changeAA_handle1.left
        anchors.rightMargin: 6
		exampleName: "VertexBuffer"
		exampleItem: unrimpitem
	}

	MaximizeButton {
		id: maximize_handle1
		anchors.top: move_handle1.top
		anchors.right: unrimpitem.right
		anchors.rightMargin: 5
		
		onClicked: { unrimpitem.state = unrimpitem.state == '' ? 'State1' : '' }
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

    CameraController {
		id: camera_controller_container
		anchors.left: moveItem_cameraControl.left
		anchors.leftMargin: -5
		anchors.top: moveItem_cameraControl.bottom
		anchors.topMargin: 6
	}

    MoveItem {
		id: moveItem_cameraControl
		x: 31
        y: 269
		window: unrimp
	}
	
	ShowHideButton {
		id: showHide_cameraControl
		
		anchors.left: moveItem_cameraControl.right
        anchors.leftMargin: 6
        anchors.top: moveItem_cameraControl.top
		onClicked: camera_controller_container.opacity = camera_controller_container.opacity == 1 ? 0 : 1
	}

	MaximizeButton {
		id: maximize_handle2
		anchors.top: moveItem_cameraControl.top
		anchors.right: camera_controller_container.right
		anchors.rightMargin: 5
		
		property bool fullscreen: false
		onClicked: {
			if (fullscreen)
				Window.showNormal();
			else
				Window.showFullScreen();
			fullscreen = !fullscreen;
		}
	}
}
