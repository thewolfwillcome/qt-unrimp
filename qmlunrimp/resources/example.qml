import QtQuick 2.1
import Unrimp 1.0
import QtQuick.Window 2.0

Window {
	visible:true
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
					target: back
					opacity: 0
				}
			}
		]
	}

	MoveItem {
		id: move_handle1
		x: 250
		y: 200
		window: unrimp
		
		Behavior on x { NumberAnimation { } }
		Behavior on y { NumberAnimation { } }
	}
	
	
	TitleBarText {
		text: "Current Example: "+unrimpitem.exampleName
		anchors.left: showhide_handle1.right
		anchors.right: changeAA_handle1.left
		anchors.top: move_handle1.top
		anchors.bottom: move_handle1.bottom
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

	MaximizeButton {
		id: maximize_handle1
		anchors.top: move_handle1.top
		anchors.right: unrimpitem.right
		anchors.rightMargin: 5
		
		onClicked: { unrimpitem.state = unrimpitem.state == '' ? 'State1' : '' }
	}
	
	ExamplesList {
		id: example_list_container
		anchors.left: moveItem_exampleList.left
		anchors.leftMargin: -5
		anchors.top: moveItem_exampleList.bottom
		anchors.topMargin: 6
		width:200
		height: 200
		model: myModel
		
		onCurrentExampleChanged: {
			unrimpitem.exampleName = name;
		}
	}

	MoveItem {
		id: moveItem_exampleList
		x: 30
		y: 200
		window: unrimp
	}

	ShowHideButton {
		id: showHide_exampleList
		
		anchors.left: moveItem_exampleList.right
		anchors.leftMargin: 6
		anchors.top: moveItem_exampleList.top
		onClicked: example_list_container.opacity = example_list_container.opacity == 1 ? 0 : 1
	}
	TitleBarText {
		text: "Examples"
		anchors.left: showHide_exampleList.right
		anchors.right: maximize_window.left
		anchors.top: moveItem_exampleList.top
		anchors.bottom: moveItem_exampleList.bottom
	}

	MaximizeButton {
		id: maximize_window
		anchors.top: moveItem_exampleList.top
		anchors.right: example_list_container.right
		anchors.rightMargin: 5
		
		property bool fullscreen: false
		onClicked: {
			if (fullscreen)
				unrimp.showNormal();
			else
				unrimp.showFullScreen();
			fullscreen = !fullscreen;
		}
	}
	
	ListModel {
		id: myModel
		ListElement {
			name: "FirstTriangle"
			type: "Basic"
		}
		ListElement {
			name: "VertexBuffer"
			type: "Basic"
		}
		ListElement {
			name: "FirstTexture"
			type: "Basic"
		}
		ListElement {
			name: "FirstRenderToTexture"
			type: "Basic"
		}
		ListElement {
			name: "FirstMultipleRenderTargets"
			type: "Basic"
		}
		/*ListElement { // not possbile here because a swap chain is a window not a framebuffer object
			name: "FirstMultipleSwapChains"
			type: "Basic"
		}*/
		ListElement {
			name: "FirstInstancing"
			type: "Basic"
		}
		ListElement {
			name: "FirstGeometryShader"
			type: "Basic"
		}
		ListElement {
			name: "FirstTessellation"
			type: "Basic"
		}
		ListElement {
			name: "FirstPostProcessing"
			type: "Advanced"
		}
		ListElement {
			name: "Fxaa"
			type: "Advanced"
		}
		/*ListElement { // not possbile here because it doesn't render anything to screen
			name: "FirstGpgpu"
			type: "Advanced"
		}*/
		ListElement {
			name: "InstancedCubes"
			type: "Advanced"
		}
		ListElement {
			name: "IcosahedronTessellation"
			type: "Advanced"
		}
		ListElement {
			name: "FirstFont"
			type: "Advanced"
		}
	}
}
