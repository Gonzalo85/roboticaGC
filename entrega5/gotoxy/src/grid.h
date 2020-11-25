//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template<typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid
{
    public:

//    this.width = width;
//    this.tile = tile;
        Grid()
        {
            array.resize((int)(width/tile));
            for (auto &row : array)
                row.resize((int)(width/tile));//creada matriz de 50x50
            int k=0;
            for (int i = hmin; i < width/2; i += tile, k++)
            {
                int l=0;
                for (int j = hmin; j < width/2; j += tile, l++)//recorriendo coordenadas de la celda, indexada por tile(100 en 100)
                {
                    array[k][l] = Value{false, nullptr, i, j};//coordenadas del centro
                }
            }
        };

        struct Value
        {
            bool occupied = false;
            QGraphicsRectItem * paint_cell = nullptr;
            int cx, cy;//coordenadas del centro en el mundo
            int dist = 0; //dist vecinos
        };

        std::vector<std::vector<Value>> array;

        void create_graphic_items(QGraphicsScene &scene)
        {
            for (auto &row : array)
                for (auto &elem : row)
                {
                    elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")),
                                                    QBrush(QColor("LightGreen")));
                    elem.paint_cell->setPos(elem.cx, elem.cy);//mover el elemento a sus coordenadas, al centro de la baldosa
                }
        }
    /*
 * Inicializamos el array a false, osea, no ocupadas.
 */
    void inicializate()//poner a ocupadas la superficie que caiga debajo de las cajas que sean obstaculos
    {
        for (int i = 0; i < this->tam; ++i) {
            for (int j = 0; j < this->tam; ++j) {
                this->array[i][j] = false;
            }
        }
    }

public:
    /**
     * modificamos en funcion de v la coordenada x,z
     * @param x
     * @param z
     * @param v
     */
    void set_Value(int x, int z, bool v)
    {
        this->array[x][z].occupied = v;
       auto [i, j] = transformar(x,z);//metodo que pases x,z y devuelva i,j, acceder a valores del mundo
       array[i][j].occupied = v;
       if(v)
            array[i][j].paint_cell->setBrush(QColor("Red"));
       else
           array[i][j].paint_cell->setBrush(QColor("Green"));

    }
    /**
     * devolvemos el valor de la coordenada x,z
     * @param x
     * @param z
     * @return
     */
    bool get_value(int x, int z)
    {
        auto [i, j] = transformar(x,z);
        return  this->array[x][z];
    }

    std::tuple<int,int> transformar(int i, int j){
        int k = ((width/tile)/5000 * i) + (width/tile)/2;
        int l = ((width/tile)/5000 * j) + (width/tile)/2;

        return std::make_tuple(k,l);
    }

};


#endif //GOTOXY_GRID_H
