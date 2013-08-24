import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import Unrimp 1.0
import QtQuick.Window 2.0

Window {
	title: qsTr("Unrimp Example Viewer")
	visible:true
	width: 800
	height: 600
// 	width: view1.x + view2.x + view2.width
// 	height: view1.y + view2.y + view2.height

	UnrimpExampleView {
		id:view1
		x:5
		y:5
		anchors.fill:parent
	}
	
// 	UnrimpExampleView {
// 		id: view2
// 		x:650
// 		y:5
// 	}
}