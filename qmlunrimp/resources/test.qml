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
				
				model:unrimpitem.exampleModel;
				Layout.fillWidth: true
				Layout.fillHeight: true
				onCurrentExampleChanged: {
					unrimpitem.exampleIndex = index
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
	}
}