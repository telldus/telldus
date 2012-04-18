import QtQuick 1.1
import QtDesktop 0.1

Text {
	property int currentState: -1
	property int state: 0
	id: stateLabel;
	text: ""
	font.bold: state == currentState
}
