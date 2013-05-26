import QtQuick 2.0
Rectangle {
	id: container
	
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
	
	border.width: 4
	border.color: "#1a1a1a"
	
	width: 139
	height: 200
	signal currentExampleChanged(string name)
	property alias model: list.model
	Component {
		id: exampleDelegate
		Item {
			id: wrapper
			width: container.width-container.border.width*4; height: 20
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
					container.currentExampleChanged(name);
				}
			}
		}
	}
	
	Component {
		id: sectionHeading
		Rectangle {
			width: container.width-container.border.width*4
			height: childrenRect.height
			color:"yellow"
			Text {
				text: section
				font.bold: true
				
			}
		}
	}
	
	Gradient {
		id: clubcolors
		GradientStop { position: 0.0; color: "#8EE2FE"}
		GradientStop { position: 0.66; color: "#7ED2EE"}
	}

	ListView {
		id: list
		anchors.fill: parent
		anchors.topMargin: container.border.width*2
		anchors.leftMargin: container.border.width*2
		anchors.bottomMargin: container.border.width*2
		delegate: exampleDelegate
		highlight: Rectangle { color: "lightsteelblue"; }
		focus: true
		section.property: "type"
		section.criteria: ViewSection.FullString
		section.delegate: sectionHeading
		populate: Transition {
			NumberAnimation { properties: "x,y"; duration: 500 }
		}
		
		clip:true;
		
	}
}