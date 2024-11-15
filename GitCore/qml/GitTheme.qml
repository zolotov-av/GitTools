pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property double fontSize: 16
    readonly property double smallerFont: fontSize * 0.9
    readonly property double smallFont: fontSize * 0.8;
    readonly property double largerFont: fontSize * 1.1
    readonly property double largeFont: fontSize * 1.2;
}
