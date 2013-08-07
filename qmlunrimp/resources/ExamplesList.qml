import QtQuick 2.0
Rectangle {
	id: container
	property alias model: list.model
	
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
	
	signal currentExampleChanged(string name, int index)
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
					container.currentExampleChanged(name, index);
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
		// slide in from left animation
		populate: Transition {
			id: dispTrans
			SequentialAnimation {
				// First position items outside on the left of the Listview
				ParallelAnimation {
					NumberAnimation {
						property: "x"; to: dispTrans.ViewTransition.destination.x - dispTrans.ViewTransition.item.width
						duration:0
					}
					NumberAnimation {
						property: "y"; to: dispTrans.ViewTransition.destination.y
						duration:0
					}
				}
				// Wait some time to have a stair like enter animation (an item starts entering the view after the previous item has started moving)
				PauseAnimation {
					duration: (dispTrans.ViewTransition.index -
							dispTrans.ViewTransition.targetIndexes[0]) * 100
				}
				// The move animation
				NumberAnimation { properties: "x"; duration: 750; easing.type: Easing.InOutQuad }
			}
		}
		clip:true;
		
	}
}