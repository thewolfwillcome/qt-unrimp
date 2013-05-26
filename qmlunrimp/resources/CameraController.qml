import QtQuick 2.0
Rectangle {
	id: camera_controller_container
	width: 139
	height: 208
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
	clip: false

	Behavior on opacity { PropertyAnimation { } }

	Image {
		id: image1
		width: 135
		height: 31
		anchors.top: parent.top
		anchors.topMargin: 9
		anchors.right: parent.right
		anchors.rightMargin: 0
		anchors.left: parent.left
		anchors.leftMargin: 0
		fillMode: "PreserveAspectFit"
		smooth: true
		source: "qt-logo.png"
	}

	Item {
		id: rectangle3
		x: 89
		y: 95
		width: 30
		height: 30
		anchors.horizontalCenterOffset: 35
		anchors.centerIn: image7

		MouseArea {
			id: mouse_area1
			anchors.fill: parent

			onPressed: camerawrapper.yaw += 20
		}

		Image {
			id: image5
			rotation: -90
			smooth: true
			anchors.fill: parent
			source: "arrow.png"
		}
	}

	Item {
		id: rectangle4
		x: 27
		y: 95
		width: 30
		height: 30
		anchors.horizontalCenterOffset: -35
		anchors.centerIn: image7
		MouseArea {
			id: mouse_area2
			anchors.fill: parent

			onPressed: camerawrapper.yaw -= 20
		}

		Image {
			id: image4
			rotation: 90
			smooth: true
			anchors.fill: parent
			source: "arrow.png"
		}
	}

	Item {
		id: rectangle5
		y: 64
		width: 30
		height: 30
		anchors.verticalCenterOffset: -35
		anchors.centerIn: image7
		MouseArea {
			id: mouse_area3
			anchors.fill: parent

			onPressed: camerawrapper.pitch -= 20
		}

		Image {
			id: image3
			rotation: 180
			smooth: true
			anchors.fill: parent
			source: "arrow.png"
		}
	}

	Item {
		id: rectangle6
		x: 58
		y: 124
		width: 30
		height: 30
		anchors.verticalCenterOffset: 35
		anchors.centerIn: image7
		anchors.horizontalCenterOffset: 0
		MouseArea {
			id: mouse_area4
			x: 0
			y: -1
			anchors.rightMargin: 0
			anchors.bottomMargin: 0
			anchors.leftMargin: 0
			anchors.topMargin: 0
			anchors.fill: parent

			onPressed: camerawrapper.pitch += 20
		}

		Image {
			id: image6
			smooth: true
			anchors.fill: parent
			source: "arrow.png"
		}
	}

	Image {
		id: image7
		x: 84
		y: 0
		width: 24
		height: 24
		anchors.verticalCenterOffset: 9
		anchors.centerIn: parent
		smooth: true
		source: "circle.png"

		MouseArea {
			id: mouse_area5
			x: 0
			y: -1
			anchors.fill: parent
			anchors.topMargin: 0
			anchors.rightMargin: 0
			anchors.bottomMargin: 0
			anchors.leftMargin: 0

			onClicked: { camerawrapper.yaw = 0; camerawrapper.pitch = 0 }
		}
	}

	Image {
		id: image8
		x: 20
		y: 182
		width: 30
		height: 30
		anchors.bottomMargin: 9
		anchors.bottom: parent.bottom
		anchors.horizontalCenterOffset: 14
		anchors.horizontalCenter: rectangle4.horizontalCenter
		smooth: true
		source: "minus.png"

		MouseArea {
			id: mouse_area6
			x: 0
			y: -1
			anchors.fill: parent
			anchors.topMargin: 0
			anchors.rightMargin: 0
			anchors.bottomMargin: 0
			anchors.leftMargin: 0

			onPressed: camerawrapper.zoom /= 1.3
		}
	}

	Image {
		id: image9
		width: 30
		height: 30
		anchors.horizontalCenterOffset: -14
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 9
		anchors.horizontalCenter: rectangle3.horizontalCenter
		smooth: true
		source: "plus.png"

		MouseArea {
			id: mouse_area7
			x: 0
			y: 0
			anchors.fill: parent
			anchors.topMargin: 0
			anchors.rightMargin: 0
			anchors.bottomMargin: 0
			anchors.leftMargin: 0

			onPressed: camerawrapper.zoom *= 1.3
		}
	}
}