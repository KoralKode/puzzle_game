#include "gameboard.h"
#include <random>
#include <algorithm>
using namespace std;

namespace{
bool isAdjacent(GameBoard::Position f, GameBoard::Position s){//функция проверяет находятся ли рядом элементы
    if(s==f){//позиции не должны быть равны
        return false;
    }
    const auto calcDistance=[](const size_t pos1,size_t pos2){//лямбда-функции считает расстояние между двумя элементами
        int distance=static_cast<int>(pos1);
        distance-=static_cast<int>(pos2);
        distance=abs(distance);
        return distance;
    };

    bool result=false;
    if(f.first==s.first){//если находятся на одной строке смотрим дистанцию
        int distance=calcDistance(f.second,s.second);
        if(distance==1){//если дистанция=1, то элементы находятся рядом
            result=true;
        }
    }else if(f.second==s.second){//если находятся в одном столбце смотрим дистанцию
        int distance=calcDistance(f.first,s.first);
        if(distance==1){//если дистанция=1, то элементы находятся рядом
            result=true;
        }
    }
    return result;
}
}

size_t GameBoard::dimension() const
{
    return m_dimension;
}

size_t GameBoard::boardSize() const
{
    return m_boardSize;
}

GameBoard::Position GameBoard::getRowCol(size_t index) const
{
    Q_ASSERT(m_dimension>0);//бросает исключение если не так
    size_t row=index/m_dimension;//строка-это индекс делёная на длин строки
    size_t col=index%m_dimension;//колонка-это что останется если из индекса вычесть кратное размеру двумерной доски число
    return make_pair(row,col);
}

bool GameBoard::move(int index)
{
    if(!isPositionValid(static_cast<size_t> (index))){//проверяем валидна ли позиция
        return false;
    }
    Position elementPosition{getRowCol(index)};//находим позицию в двумерном представлении

    auto hiddenElementIterator=find(m_rowBoard.begin(),m_rowBoard.end(),boardSize());//ищем скрытый элемент, значение которого =16
    Q_ASSERT(hiddenElementIterator!=m_rowBoard.end());//бросает исключение если не так
    Position hiddenElementPosition{getRowCol(distance(m_rowBoard.begin(),hiddenElementIterator))};//двумерная позиция скрытого элемента посредством метода для преобразования одномерного индекса в двмерный и встроенной функции нахождения расстояния между элементами массива
    if(!isAdjacent(elementPosition,hiddenElementPosition)){//если элемент и скрытый элемент не рядом
        return false;
    }
    swap(hiddenElementIterator->val,m_rowBoard[index].val);//меняем местами скрытый элемент с нажатым
    emit dataChanged(createIndex(0,0),createIndex(m_boardSize,0));//вызываем сигнал для изменения всех плиток, хотя можно сообщить QMLю об изменении конкретных плиток
    return true;
}



void GameBoard::shuffle()
{
    auto seed=chrono::system_clock::now().time_since_epoch().count();//генерируем seed
    static mt19937 generator(seed);//создаём статический генератор
    do{
        std::shuffle(m_rowBoard.begin(),m_rowBoard.end(),generator);//перемешиваем элементы доски
    }while(!isBoardValid());//пока доска не правильная(непригодна для игры)
}

bool GameBoard::isPositionValid(const size_t position) const
{
    return position < m_boardSize;//позиция верная если она не превышает размер доски
}

GameBoard::GameBoard(const size_t boardDimension, QObject *parent):QAbstractListModel{parent},m_dimension{boardDimension},m_boardSize{m_dimension*m_dimension}{//тут надо было для делегирования конструктору QAbstractListModel дать объект базового класса
    m_rowBoard.resize(m_boardSize);//инициализируем таким образом вектор игровой доски
    iota(m_rowBoard.begin(),m_rowBoard.end(),1);//генерирует последовательность номеров для нашей доски
    shuffle();//перемешиваем в рандомном порядке элементы доски
}

int GameBoard::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)//чтобы Qt не писал, что parent не используется
    return m_rowBoard.size();
}

bool GameBoard::isBoardValid() const
{
    int inv {0};
    for (size_t i {0}; i < m_boardSize; ++i) {
        for (size_t j = i; j < m_boardSize; ++j) {
            if (m_rowBoard[j].val < m_rowBoard[i].val && m_rowBoard[i].val!=m_boardSize){
                ++inv;
            }
        }
    }
    const size_t start_point = 1;

    for (size_t i = 0; i < m_boardSize; ++i) {
        if (m_rowBoard[i].val == m_boardSize){
            inv += start_point + i / m_dimension;
        }
    }

    return (inv % 2) == 0;
}

QVariant GameBoard::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role!=Qt::DisplayRole){//проверяем корректный ли индекс и соответствет ли роль
        return {};
    }
    const int rowIndex{index.row()};//получаем индекс элемента в виде цифры
    if(!isPositionValid(rowIndex)){//проверяем верна ли позиция под данным индексом
        return {};
    }
    return QVariant::fromValue(m_rowBoard[rowIndex].val);//возвращаем через функцию, которая сама определяет тип для QVariant, значение, хранщееся под полченым индексом
}
