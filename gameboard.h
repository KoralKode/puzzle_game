#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <QAbstractListModel>//подключаем модель в виде списка, потому что GridView может работать только со списками
#include <vector>
using namespace std;

class GameBoard: public QAbstractListModel
{
    struct Tile{//структура, реалезующая плитк, конкретно номер и операторы
        size_t val;//значение на плитке
        Tile &operator=(size_t a){
            val=a;
            return *this;
        }
        bool operator ==(size_t a){
            return val==a;
        }
    };

    Q_OBJECT//указываем что Gameboard является Q_OBJECTом и moc компилятор видит наш класс
    Q_PROPERTY(int dimension READ dimension CONSTANT)//свойство для передачи длинны строк
    Q_PROPERTY(int hiddenElementValue READ boardSize CONSTANT)//свойство для передачи значения скрытого элемента, он всегда равен размеру доски
    //Q_PROPERTY это макрос, который работает только в Q_OBJECT, нужен чтобы поля класса типа Q_OBJECT могли передаваться в QML
    vector<Tile> m_rowBoard;//хранит нашу игровую доску, начинается с m_ чтобы qt мог савм делать функции с этими полями, типа getter и setter
    size_t m_dimension;//количество строк и столбцов
    size_t m_boardSize;//количество элементов

    void shuffle();//метод перемешивания элементов нашей доски
    bool isPositionValid(const size_t position)const;//метод проверяет допустимая ли позиция
    bool isBoardValid() const;//метод проверки правильности игровой доски

public:

    static constexpr size_t defaultPuzzlDimnsion {4};//значение количества строк и столбцов по-умолчанию
    using Position=pair<size_t,size_t>;//alias позиции, чтобы мы могли просто написать Position подразумевая pair<size_t,size_t>
    GameBoard(const size_t boardDimension=defaultPuzzlDimnsion, QObject* parent=nullptr);//тут необходимо указать QObject базовый класс, потому что в модели это надо прописывать, хотя бы nullptr
    int rowCount(const QModelIndex& parent=QModelIndex{}) const override;//метод возвращает количество элементов, для того, чтобы его можно было переопределить указываем QModelIndex, мы переопределяем метод QAbstractListModel
    QVariant data(const QModelIndex& index,int role=Qt::DisplayRole) const override;//метод возвращает данные от кода c++ к qml, для этого используем тип QVfriant, потом что с помощью него можно передавать разные типы данных
    size_t dimension() const;//getter возвращающий размер строки
    size_t boardSize() const;//getter возвращающий размер доски
    Q_INVOKABLE bool move(int index);//метод для передвижения плитки
    Position getRowCol(size_t index) const;//переодит индекс из одномерного в двумерный
};

#endif // GAMEBOARD_H
