import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Rectangle {
	id: container
	property alias model: list.model
	signal currentExampleChanged(string name, int index)

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
	ScrollView {
		anchors.fill: parent
		ListView {
			id:list
			delegate: exampleDelegate
			model: myModel
			section.property: "type"
			section.criteria: ViewSection.FullString
			section.delegate: sectionHeading
			highlight: Rectangle { color: "lightsteelblue"; }
			focus: true
		}
	}

	Component {
		id: sectionHeading
		Rectangle {
			width: parent.width
			height: sectionText.height*1.5
			color:"yellow"
			Text {
				id:sectionText
				text: section
				font.bold: true
				anchors.verticalCenter: parent.verticalCenter
			}
		}
	}

	Component {
		id: exampleDelegate
		Item {
			id: wrapper
			height:20
			width: parent.width;
			anchors.leftMargin:2
			Column {
				Text {
					text: name
					color: wrapper.ListView.isCurrentItem ? "black": "white"
				}
			}
			MouseArea {
				anchors.fill: parent
				onClicked:{
					list.currentIndex = index;
					container.currentExampleChanged(name, index);
				}
			}

		}
	}
}
