import QtQuick 2.0
Rectangle {
	id: title_exampleList
	property alias text: textItem.text
	anchors.left: showHide_exampleList.right
	anchors.right: maximize_window.left
	anchors.leftMargin: 6
	anchors.rightMargin: 6
	anchors.top: moveItem_exampleList.top
	anchors.bottom: moveItem_exampleList.bottom
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
	Text {
		id:textItem
		anchors.leftMargin: 3
		anchors.rightMargin: 3
		anchors.fill: parent
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignHCenter
		
	}
	clip:true;
}