import QtQuick 2.0

Item {
	id:item
	width:200
	height:150
	z: 100

	default property alias children: childContainer.children
	property string position: "left"
	property int handle_height: -1
	property alias expandable: handle_click.enabled

	property alias handle_text: handle_text.text

	function getXPosition() {
		if(position == "left")
			return width*-1
		if (position == "top" || position == "bottom") 
			return (item.parent.width-item.width)/2;
		if (position == "right")
			return item.parent.width
	}

	function getYPosition() {
		if(position == "left" || position == "right")
			return (item.parent.height-item.height)/2;
		if (position == "top")
			return height*-1
		if (position == "bottom")
			return item.parent.height
	}
	
	x: getXPosition()
	y: getYPosition()
	
	Component.onCompleted: {
		// fix invalid values for position
		if (position != "top" && position != "bottom" && position != "left" && position != "right")
			position = "left";
		
		// fix position depending of position
		if (position == "top" || position == "bottom")
			item.y = getYPosition();
		else
			item.x = getXPosition();
	}

	Rectangle {
		id:handle
		y: (position == "left" || position == "right") ? container.y+container.height/2-height/2 : 0
		x: (position == "top" || position == "bottom") ? container.x+container.width/2-width/2 : 0
		anchors.left: {
			if(position == "left")
				return container.right
		}
		anchors.right: {
			if(position == "right")
				return container.left
		}
		anchors.top: {
			if(position == "top")
				return container.bottom
		}

		anchors.bottom: {
			if(position == "bottom")
				return container.top
		}

		anchors.leftMargin: {
			if(position == "left")
				return handle.width/2*-1
		}

		anchors.rightMargin: {
			if(position == "right")
				return handle.width/2*-1
		}

		anchors.topMargin: {
			if(position == "top")
				return handle.height/2*-1
		}

		anchors.bottomMargin: {
			if(position == "bottom")
				return handle.height/2*-1
		}
		border.width: 2
		radius: 15
		width: (position == "left" || position == "right") ? handle_text.font.pixelSize*3 : (item.handle_height <= 0 ? handle_text.contentWidth+container.radius*2+10 : item.handle_height)
		
		height: (position == "top" || position == "bottom") ? handle_text.font.pixelSize*3 : (item.handle_height <= 0 ? handle_text.contentWidth+container.radius*2+10 : item.handle_height)

		MouseArea {
			id: handle_click
			anchors.fill: parent
			onClicked: {
				if(position == "left")
					handle.state == 'expandedLeft' ? handle.state = "" : handle.state = 'expandedLeft';
				if(position == "top")
					handle.state == 'expandedTop' ? handle.state = "" : handle.state = 'expandedTop';
				if(position == "right")
					handle.state == 'expandedRight' ? handle.state = "" : handle.state = 'expandedRight';
				if(position == "bottom")
					handle.state == 'expandedBottom' ? handle.state = "" : handle.state = 'expandedBottom';
			}
		}

		Text {
			id: handle_text
			anchors.verticalCenter: parent.verticalCenter
			anchors.horizontalCenter: parent.horizontalCenter

			rotation: {
				if (position == "left")
					return 90;
				else if (position == "right")
					return -90

				return 0;
			}
			anchors.horizontalCenterOffset : {
				if (position == "left")
					return parent.anchors.leftMargin*-1 - handle_text.font.pixelSize+2
				if (position == "right")
					return parent.anchors.rightMargin + handle_text.font.pixelSize-2

				return 0;
			}
			anchors.verticalCenterOffset: {
				if (position == "top")
					return parent.anchors.topMargin*-1 - handle_text.font.pixelSize+2
				if (position == "bottom")
					return parent.anchors.bottomMargin + handle_text.font.pixelSize-2
				return 0;
			}
		}

		states: [
			State {
				name: "expandedLeft"
				PropertyChanges { target: item; x:-container.radius }
			},
			State {
				name: "expandedTop"
				PropertyChanges { target: item; y:-container.radius }
			},
			State {
				name: "expandedRight"
				PropertyChanges { target: item; x:item.parent.width - item.width + container.radius }
			},
			State {
				name: "expandedBottom"
				PropertyChanges { target: item; y:item.parent.height - item.height + container.radius }
			}
		]

		transitions: [
			Transition {
				from: "*"; to: "expandedLeft, expandedRight"
				reversible: true
				NumberAnimation { properties: "x";}
			},
			Transition {
				from: "*"; to: "expandedTop, expandedBottom"
				reversible: true
				NumberAnimation { properties: "y";}
			}
		]
	}

	Rectangle {
		id:container
		clip:true

		border.width: 2
		radius:5

		height:item.height
		width:item.width

		Item {
			id:childContainer
			anchors.fill: parent
			anchors.margins: container.border.width
			anchors.leftMargin: {
				if(position == "left")
					return container.radius
			}

			anchors.rightMargin: {
				if(position == "right")
					return container.radius
			}

			anchors.topMargin: {
				if(position == "top")
					return container.radius
			}

			anchors.bottomMargin: {
				if(position == "bottom")
					return container.radius
			}
			clip: true
		}
	}
}
