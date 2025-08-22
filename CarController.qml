// MyScene.qml
import QtQuick
import QtQuick3D 6.7
import QtQuick3D.Helpers
import QtQuick.Layouts
import QtQuick.Controls
import QtLocation 5.15
import QtPositioning 5.15

Rectangle {
    id: rectangle
    color: "#aeacac"
    antialiasing: true
    width: 640
    height: 480

    NumberAnimation {
        id: startupRotation
        target: carRoot
        property: "eulerRotation.y"
        from: 0
        to: 360
        duration: 3000
        loops: 1
        easing.type: Easing.InOutQuad
    }

    property bool leftDoorOpen: false
    property bool rightDoorOpen: false

    // Kapı parçalarının indeksleri - mesh'lere göre güncellendi
        property var leftDoorParts: [41, 33, 37, 55] // Sol kapı parçaları
        property var rightDoorParts: [14, 15, 25, 31] // Sağ kapı parçaları

    // Orijinal rotasyon ve pozisyon değerlerini sakla
    property var leftDoorOriginalRotation: []
    property var leftDoorOriginalY: []
    property var rightDoorOriginalRotation: []
    property var rightDoorOriginalY: []

    // Kapı parçalarını bulma fonksiyonu
        function findDoorNodes(side) {
            var parts = [];
            var doorIndices = (side === "left") ? leftDoorParts : rightDoorParts;

            for (var i = 0; i < doorIndices.length; i++) {
                var partIndex = doorIndices[i];
                // Instantiator'dan parçayı bulmak için carRoot.children'ı kontrol ediyoruz
                for (var j = 0; j < carRoot.children.length; j++) {
                    var child = carRoot.children[j];
                    if (child && child.objectName === "part_" + partIndex) {
                        parts.push(child);
                        break;
                    }
                }
            }
            return parts;
        }

        // Orijinal değerleri kaydet
        Component.onCompleted: {
            // Sol kapı değerleri
            var leftParts = findDoorNodes("left");
            for (var i = 0; i < leftParts.length; i++) {
                leftDoorOriginalRotation.push(Qt.vector3d(leftParts[i].eulerRotation.x,
                                                      leftParts[i].eulerRotation.y,
                                                      leftParts[i].eulerRotation.z));
                leftDoorOriginalY.push(leftParts[i].y);
            }

            // Sağ kapı değerleri
            var rightParts = findDoorNodes("right");
            for (var j = 0; j < rightParts.length; j++) {
                rightDoorOriginalRotation.push(Qt.vector3d(rightParts[j].eulerRotation.x,
                                                       rightParts[j].eulerRotation.y,
                                                       rightParts[j].eulerRotation.z));
                rightDoorOriginalY.push(rightParts[j].y);
            }
        }
    // --- Sol kapı animasyonu ---
    ParallelAnimation {
        id: leftDoorAnimation

        // Rotasyon animasyonları
        SequentialAnimation {
            PropertyAnimation {
                targets: findDoorNodes("left")
                property: "eulerRotation.x"
                to: leftDoorOpen
                    ? leftDoorOriginalRotation[0].x + 70
                    : leftDoorOriginalRotation[0].x
                duration: 250
                easing.type: Easing.OutBack
            }
        }

        // Y ekseni animasyonları (kapı yukarı kalkma)
        SequentialAnimation {
            PropertyAnimation {
                targets: findDoorNodes("left")
                property: "y"
                to: leftDoorOpen
                    ? leftDoorOriginalY[0] + 1
                    : leftDoorOriginalY[0]
                duration: 250
                easing.type: Easing.OutQuad
            }
        }
    }

    // --- Sağ kapı animasyonu ---
    ParallelAnimation {
        id: rightDoorAnimation

        // Rotasyon animasyonları
        SequentialAnimation {
            PropertyAnimation {
                targets: findDoorNodes("right")
                property: "eulerRotation.x"
                to: rightDoorOpen
                    ? rightDoorOriginalRotation[0].x + 70
                    : rightDoorOriginalRotation[0].x
                duration: 250
                easing.type: Easing.OutBack
            }
        }

        // Y ekseni animasyonları (kapı yukarı kalkma)
        SequentialAnimation {
            PropertyAnimation {
                targets: findDoorNodes("right")
                property: "y"
                to: rightDoorOpen
                    ? rightDoorOriginalY[0] + 1
                    : rightDoorOriginalY[0]
                duration: 250
                easing.type: Easing.OutQuad
            }
        }
    }

    View3D {
        anchors.fill: parent

        SceneEnvironment {
            id: sceneEnvironment
            antialiasingQuality: SceneEnvironment.VeryHigh
            antialiasingMode: SceneEnvironment.MSAA
            backgroundMode: SceneEnvironment.SkyBox
            lightProbe: Texture {
                textureData: ProceduralSkyTextureData {
                }
            }
        }

        PerspectiveCamera {
            id: sceneCamera
            x: 0
            y: 200
            z: -600
            eulerRotation.x: -15
            eulerRotation.y: 180
        }

        DirectionalLight {
            brightness: 0.8
            eulerRotation: Qt.vector3d(-90, 0, 0)
        }

        Node {
            id: carRoot
            objectName: "carRoot"
            scale: Qt.vector3d(100, 100, 100)
            eulerRotation: Qt.vector3d(-5, 0, 0)
            x: 0
            y: 50
            z: 0

            property real wheelRotation: 0
            property bool wheelsSpinning: false

            // Tekerlek Node'ları
            Node {
                id: wheelFL
                objectName: "wheelFL"
                position: Qt.vector3d(-0.98338, 0.33361, 1.22232)
                rotation: Qt.quaternion(0.707107, -0.707107, 0, 0)
                scale: Qt.vector3d(-1.11979, -1.11979, -1.11979)
            }

            Node {
                id: wheelRL
                objectName: "wheelRL"
                position: Qt.vector3d(-1.00646, 0.33361, -1.47900)
                rotation: Qt.quaternion(0.707107, -0.707107, 0, 0)
                scale: Qt.vector3d(-1.11979, -1.11979, -1.11979)
            }

            Node {
                id: wheelRR
                objectName: "wheelRR"
                position: Qt.vector3d(1.00646, 0.33361, -1.47900)
                rotation: Qt.quaternion(0.707107, 0.707107, 0, 0)
                scale: Qt.vector3d(1.11979, 1.11979, 1.11979)
            }

            Node {
                id: wheelFR
                objectName: "wheelFR"
                position: Qt.vector3d(0.98338, 0.33361, 1.22232)
                rotation: Qt.quaternion(0.707107, 0.707107, 0, 0)
                scale: Qt.vector3d(1.11979, 1.11979, 1.11979)
            }

            // --- TEXTURES ---
            Texture { id: _0_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData0.png" }
            Texture { id: _1_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData14.png" }
            Texture { id: _2_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData19.png" }
            Texture { id: _3_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData24.png" }
            Texture { id: _4_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData48.png" }
            Texture { id: _5_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData57.png" }
            Texture { id: _6_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData78.png" }
            Texture { id: _7_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData109.png" }
            Texture { id: _8_texture; source: "qrc:/textures/Generated/QtQuick3D/Node2012_lamborghini_aventador/maps/textureData84.png" }

            // --- MATERIALS ---
            property var partMaterials: [
                vehicle_Exterior_mm_wheel_material,    // 0
                vehicle_Exterior_mm_tyre_material,      // 1
                vehicle_Exterior_mm_misc_material,      // 2
                vehicle_Exterior_mm_rotor_material,     // 3
                vehicle_Exterior_mm_wheel_material,     // 4
                vehicle_Exterior_mm_tyre_material,      // 5
                vehicle_Exterior_mm_misc_material,      // 6
                vehicle_Exterior_mm_rotor_material,     // 7
                vehicle_Exterior_mm_wheel_material,     // 8
                vehicle_Exterior_mm_tyre_material,      // 9
                vehicle_Exterior_mm_misc_material,      // 10
                vehicle_Exterior_mm_rotor_material,     // 11
                vehicle_Exterior_mm_ext_material,       // 12
                vehicle_Exterior_mm_misc_material,      // 13
                vehicle_Exterior_mm_cab_material,       // 14
                vehicle_Exterior_mm_misc_material,      // 15
                vehicle_Exterior_mm_misc_material,      // 16
                vehicle_Exterior_mm_wheel_material,     // 17
                vehicle_Exterior_mm_tyre_material,      // 18
                vehicle_Exterior_mm_misc_material,      // 19
                vehicle_Exterior_mm_rotor_material,     // 20
                vehicle_Exterior_mm_chassis_material,   // 21
                vehicle_Exterior_mm_badges_material,    // 22
                vehicle_Exterior_mm_ext_material,       // 23
                vehicle_Exterior_mm_windows_material,   // 24
                vehicle_Exterior_mm_ext_material,       // 25
                vehicle_Exterior_mm_cab_material,       // 26
                vehicle_Exterior_mm_windows_material,   // 27
                vehicle_Exterior_mm_windows_material,   // 28
                vehicle_Exterior_mm_lights_material,    // 29
                vehicle_Exterior_mm_lights_material,    // 30
                vehicle_Exterior_mm_ext_material,       // 31
                vehicle_Exterior_mm_misc_material,      // 32
                vehicle_Exterior_mm_ext_material,       // 33
                vehicle_Exterior_mm_windows_material,   // 34
                vehicle_Exterior_mm_lights_material,    // 35
                vehicle_Exterior_mm_windows_material,   // 36
                vehicle_Exterior_mm_ext_material,       // 37
                vehicle_Exterior_mm_windows_material,   // 38
                vehicle_Exterior_mm_windows_material,   // 39
                vehicle_Exterior_mm_windows_material,   // 40
                vehicle_Exterior_mm_misc_material,      // 41
                vehicle_Exterior_mm_misc_material,      // 42
                vehicle_Exterior_mm_ext_material,       // 43
                vehicle_Exterior_mm_badges_material,    // 44
                vehicle_Exterior_mm_ext_material,       // 45
                vehicle_Exterior_mm_ext_material,       // 46
                vehicle_Exterior_mm_windows_material,   // 47
                vehicle_Exterior_mm_lights_material,    // 48
                vehicle_Exterior_mm_ext_material,       // 49
                vehicle_Exterior_mm_ext_material,       // 50
                vehicle_Exterior_mm_misc_material,      // 51
                vehicle_Exterior_mm_chassis_material,   // 52
                vehicle_Exterior_mm_misc_material,      // 53
                vehicle_Exterior_mm_windows_material,   // 54
                vehicle_Exterior_mm_cab_material,       // 55
                vehicle_Exterior_mm_badges_material,    // 56
                vehicle_Exterior_mm_windows_material,   // 57
                vehicle_Exterior_mm_ext_material        // 58
            ]

            // Instantiator for all parts
            Instantiator {
                model: 59
                delegate: Model {
                    id: part
                    objectName: "part_" + index
                    source: "qrc:/meshes/Generated/QtQuick3D/Node2012_lamborghini_aventador/meshes/object_" + index + "_mesh.mesh"
                    materials: [ carRoot.partMaterials[index] ]

                    property bool isWheelPart: (index >= 0 && index <= 3) ||
                                               (index >= 4 && index <= 7) ||
                                               (index >= 8 && index <= 11) ||
                                               (index >= 17 && index <= 20)

                    Component.onCompleted: {
                        if (isWheelPart) {
                            var wheelNode;
                            if (index >= 0 && index <= 3) wheelNode = wheelFL;
                            else if (index >= 4 && index <= 7) wheelNode = wheelRL;
                            else if (index >= 8 && index <= 11) wheelNode = wheelRR;
                            else if (index >= 17 && index <= 20) wheelNode = wheelFR;

                            parent = wheelNode;
                            eulerRotation.x = 0;
                        } else {
                            parent = carRoot;
                            eulerRotation.x = 90;
                            startupRotation.start()
                        }
                    }
                }
            }

            // --- WHEEL ANIMATION ---
            NumberAnimation {
                id: wheelAnimation
                target: carRoot
                property: "wheelRotation"
                from: 0
                to: 360
                duration: 1000
                loops: Animation.Infinite
                running: carRoot.wheelsSpinning
            }

            // Tekerlek dönüş bağlamaları
            Binding {
                target: wheelFL
                property: "eulerRotation.x"
                value: carRoot.wheelRotation
                when: carRoot.wheelsSpinning
            }

            Binding {
                target: wheelRL
                property: "eulerRotation.x"
                value: carRoot.wheelRotation
                when: carRoot.wheelsSpinning
            }

            Binding {
                target: wheelRR
                property: "eulerRotation.x"
                value: carRoot.wheelRotation
                when: carRoot.wheelsSpinning
            }

            Binding {
                target: wheelFR
                property: "eulerRotation.x"
                value: carRoot.wheelRotation
                when: carRoot.wheelsSpinning
            }
        }

        Model {
            id: road
            source: "#Cube"
            y: -5
            scale: Qt.vector3d(20, 1, 500)
            materials: roadMaterial
            eulerRotation: Qt.vector3d(
                carRoot.eulerRotation.x,
                carRoot.eulerRotation.y,
                carRoot.eulerRotation.z
            )
        }
    }

    RowLayout {
        id: controlButtonsRow
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 10
        }
        width: Math.min(parent.width * 0.9, 600)
        height: 50
        spacing: 5

        Button {
            text: leftDoorOpen ? "Close Left Door" : "Open Left Door"
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            onClicked: {
                leftDoorOpen = !leftDoorOpen
                console.log("Left door button clicked, open state:", leftDoorOpen)
                leftDoorAnimation.start()
            }
        }

        Button {
            text: rightDoorOpen ? "Close Right Door" : "Open Right Door"
            antialiasing: false
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            onClicked: {
                rightDoorOpen = !rightDoorOpen
                console.log("Right door button clicked, open state:", rightDoorOpen)
                rightDoorAnimation.start()
            }

        }

        Button {
            text: carRoot.wheelsSpinning ? "Stop Wheels" : "Spin Wheels"
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            onClicked: {
                carRoot.wheelsSpinning = !carRoot.wheelsSpinning
                wheelAnimation.running = carRoot.wheelsSpinning
            }
        }
    }

    MouseArea {
        id: mouseArea
          anchors {
              top: rectangle1.bottom  //map alan dışı
              left: parent.left
              right: parent.right
              bottom: controlButtonsRow.top   // butonların alanını hariç tutuyoruz
          }

              property real velX: 0
              property real velY: 0
              property real lastX: 0
              property real lastY: 0

              onPressed: {
                  lastX = mouse.x
                  lastY = mouse.y
                  velX = 0
                  velY = 0
              }

              onPositionChanged: {
                  velX = mouse.x - lastX
                  velY = mouse.y - lastY
                  carRoot.eulerRotation.y += velX
                  carRoot.eulerRotation.x += velY
                  lastX = mouse.x
                  lastY = mouse.y
              }
    }

    Rectangle {
        id: rectangle1
        height: rectangle.height*0.2
        color: "#ffffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Plugin {
            id: mapPlugin
            name: "osm" // OpenStreetMap
        }

        PositionSource {
            id: positionSource
            active: true
            updateInterval: 1000
            onPositionChanged: {
                map.center = position.coordinate
                marker.coordinate = position.coordinate
            }
        }

        property real zoomStep: 0.5
        property real panStartX: 0
        property real panStartY: 0

        Map {
            id: map
            anchors.fill: parent
            plugin: mapPlugin
            zoomLevel: 15
            center: QtPositioning.coordinate(39.9208, 32.8541)
            focus: true

            // 🔴 Konum işaretçisi
            MapQuickItem {
                id: marker
                anchorPoint.x: 10
                anchorPoint.y: 10
                coordinate: QtPositioning.coordinate(0, 0)
                sourceItem: Rectangle {
                    width: 20
                    height: 20
                    color: "red"
                    radius: 10
                    border.color: "white"
                    border.width: 2
                }
            }

            // 🖱 MouseArea: Sürükleme ve Scroll Zoom
            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag.target: null
                property bool panning: false

                onPressed: {
                    panning = true
                    rectangle1.panStartX = mouse.x
                    rectangle1.panStartY = mouse.y
                }

                onReleased: {
                    panning = false
                }

                onPositionChanged: {
                    if (panning) {
                        var dx = mouse.x - rectangle1.panStartX
                        var dy = mouse.y - rectangle1.panStartY
                        map.pan(-dx, -dy)
                        rectangle1.panStartX = mouse.x
                        rectangle1.panStartY = mouse.y
                    }
                }

                onWheel: {
                    if (wheel.angleDelta.y > 0)
                        map.zoomLevel += rectangle1.zoomStep
                    else
                        map.zoomLevel -= rectangle1.zoomStep
                }
            }
        }

        // 🔽 Zoom seviyesi göstergesi
        Label {
            text: "Zoom: " + map.zoomLevel.toFixed(1)
            anchors.top: parent.top
            anchors.left: parent.left
            padding: 6
            background: Rectangle {
                color: "black"
                opacity: 0.6
                radius: 6
            }
            color: "white"
        }

    }


    WheelHandler {
        id: wheelHandler
        target: sceneCamera
        property real zoomSpeed: 10
        onWheel: function(event) {
            sceneCamera.z += event.angleDelta.y > 0 ? zoomSpeed : -zoomSpeed
            event.accepted = true
        }
    }

    Timer {
        id: inertiaTimer
        interval: 16
        running: true
        repeat: true
        onTriggered: {
            if (!mouseArea.dragging) {
                mouseArea.velX *= 0.55
                mouseArea.velY *= 0.55
                carRoot.eulerRotation.y += mouseArea.velX
                carRoot.eulerRotation.x += mouseArea.velY
                if (Math.abs(mouseArea.velX) < 0.01 &&
                    Math.abs(mouseArea.velY) < 0.01) {
                    mouseArea.velX = 0
                    mouseArea.velY = 0
                }
            }
        }
    }

    Item {
        id: __materialLibrary__

        DefaultMaterial {
            id: vehicle_Exterior_mm_wheel_material; diffuseMap: _0_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_tyre_material; diffuseMap: _1_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_misc_material; diffuseMap: _2_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_rotor_material; diffuseMap: _3_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_ext_material; diffuseMap: _4_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_cab_material; diffuseMap: _5_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_windows_material
            diffuseColor: Qt.rgba(0.1, 0.1, 0.1, 0.7)
            specularAmount: 0.5
            specularRoughness: 0.1
            opacity: 0.7
            lighting: DefaultMaterial.FragmentLighting
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_lights_material; diffuseMap: _7_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_badges_material; diffuseMap: _8_texture
        }
        DefaultMaterial {
            id: vehicle_Exterior_mm_chassis_material; diffuseColor: "silver"
        }
        DefaultMaterial {
            id: roadMaterial
            diffuseColor: "#222222"
            specularAmount: 0.1
            specularRoughness: 0.9
        }
        DefaultMaterial {
            id: yellowMaterial
            diffuseColor: "#FFFF00"
            specularAmount: 0.3
            specularRoughness: 0.7
        }
    }
}

/*##^##
Designer {
    D{i:0}D{i:12;cameraSpeed3d:25;cameraSpeed3dMultiplier:1}D{i:33;cameraSpeed3d:25;cameraSpeed3dMultiplier:1}
}
##^##*/
