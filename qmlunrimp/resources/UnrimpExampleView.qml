import QtQuick 2.1
import Unrimp 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

Item {
	id: unrimp
	width: 640
	height: 480
	clip: true
	
	ExampleModel {
		id:exampleModel
	}

	SlideOut {
		handle_text: "Examples"
		height: 310
		ColumnLayout {
			anchors.fill: parent
			ExampleList2 {
				model:exampleModel;
				Layout.fillWidth: true
				Layout.fillHeight: true
				onCurrentExampleChanged: {
					unrimpitem.exampleItem = model.get(index);
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
		exampleItem: exampleModel.get(4)
		anchors.fill: parent
	}
}