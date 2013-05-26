import QtQuick 2.0

Rectangle {
	id: showhide
	signal clicked()

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
	border.color: "#1a1a1a"

	MouseArea {
		anchors.fill: parent

		onClicked: showhide.clicked()
	}

	Rectangle {
		id: toolbar22
		x: 0
		y: -2
		radius: 12
		gradient: Gradient {
			GradientStop {
				position: 0
				color: "#5a5a5a"
			}

			GradientStop {
				position: 1
				color: "#000000"
			}
		}
		rotation: -35
		anchors.rightMargin: 6
		anchors.bottomMargin: 6
		anchors.leftMargin: 6
		anchors.topMargin: 6
		anchors.fill: parent
	}
	border.width: 2
}