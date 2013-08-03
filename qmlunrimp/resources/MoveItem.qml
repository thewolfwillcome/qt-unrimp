import QtQuick 2.0

Rectangle {
	id: move_handle
	
	property variant window: null
	
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

	border.width: 2
	border.color: "#1a1a1a"

	Image {
		anchors.rightMargin: 5
		anchors.leftMargin: 5
		anchors.bottomMargin: 5
		anchors.topMargin: 5
		anchors.fill: parent
		smooth: true
		fillMode: "Stretch"
		source: "move.gif"
	}

	MouseArea {
		anchors.fill: parent
		drag.target: move_handle
		drag.axis: "XandYAxis"
		drag.minimumX: 0
		drag.minimumY: 0
		drag.maximumX: move_handle.window.width - move_handle.width
		drag.maximumY: move_handle.window.height - move_handle.height
	}
}