import QtQuick 2.1
import Unrimp 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

Item {
	id: unrimp
	width: 800
	height: 600
	clip: true
	
	SlideOut {
		handle_text: "Examples"
		height: 310
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
		width: 300
		position: "top"
		handle_text: "Current Example: "+unrimpitem.exampleName
	}
	
	UnrimpItem {
		id: unrimpitem
		anchors.fill: parent
	}
}