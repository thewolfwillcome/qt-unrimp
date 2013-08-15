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
	
	UnrimpExampleView {
		anchors.fill:parent
	}
}