import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import Unrimp 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    minimumHeight : 160

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    ExampleModel {
        id:exampleModel
    }

    UnrimpItem
    {
        id: unrimpitem
        exampleItem: exampleModel.get(0)
        anchors.fill: parent
    }
// 
//     SlideOut {
//             handle_text: "Examples"
//             /*anchors.top: parent.top - 20
//             anchors.bottom: parent.bottom -20*/
//             height: parent.height - topSlideOut.fullHeight - 20
//             width: 300
//             ColumnLayout {
//                 anchors.fill: parent
//                 ExampleList2 {
//                     id:exampleList
//                     model:exampleModel
//                     Layout.fillWidth: true
//                     Layout.fillHeight: true
//                     onCurrentExampleChanged: {
//                         unrimpitem.exampleItem = model.get(index);
// 
//                     }
//                 }
//             }
//         }
// 
//     SlideOut {
//         id: topSlideOut
//             expandable: false
//             height:5
//             width: 300
//             position: "top"
// //             handle_text: "Current Example: " + exampleList.model.get(exampleList.currentIndex).name
//         }
}
