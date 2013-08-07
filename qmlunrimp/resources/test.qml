import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import Unrimp 1.0
import QtQuick.Window 2.0

Window {
	title: qsTr("Unrimp Example Viewer")
	visible:true
	id: unrimp
	width: 800
	height: 600
	
	SlideOut {
		center:true
		handle_text: "Examples"
		height: 310
		z: 100
		ColumnLayout {
			anchors.fill: parent
			ExampleList2 {
				
				model:myModel
				Layout.fillWidth: true
				Layout.fillHeight: true
				onCurrentExampleChanged: {
					unrimpitem.exampleName = name;
				}
			}
		}
	}
	
	SlideOut {
		expandable: false
		z:100
		width: 300
		position: "top"
		center:true
		handle_text: "Current Example: "+unrimpitem.exampleName
	}
	
	UnrimpItem {
		id: unrimpitem
		anchors.fill: parent

		exampleName: "FirstTriangle"
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