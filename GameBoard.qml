import QtQuick 2.15
import Game 1.0;//как бы подключаем файлы c++

GridView {
    id:root
    model:GameBoardModel{}//указываем нашу модель из файла c++
    cellHeight:height/root.model.dimension
    cellWidth:width/root.model.dimension
    delegate: Item{
        id: _backgroundDelegate
        width: root.cellWidth
        height:root.cellHeight
        visible: display!==root.model.hiddenElementValue
        Tile{
            anchors.margins:5
            anchors.fill:_backgroundDelegate
            displayText: display
            width:root.cellWidth
            height:root.cellHeight
            MouseArea{//компонент, который ловит любые касания мышки
                anchors.fill:parent//прикрепляем к родителю
                onClicked:{//типа лямбда фнкция, которая указываем, что действия будт происходить через нажатия на плитки
                    root.model.move(index);//вызываем метод сдвига плитки, на которую нажали
                }
            }
        }
    }

}
