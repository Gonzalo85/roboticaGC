//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template<typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid {
public:
    std::vector<std::tuple<int, int>> lista_coor_de_vecinos{{-1, -1},
                                                            {0,  -1},
                                                            {1,  -1},
                                                            {-1, 0},
                                                            {1,  0},
                                                            {-1, 1},
                                                            {0,  1},
                                                            {-1, 1}};

    Grid() {
        array.resize((int) (width / tile));
        for (auto &row : array)
            row.resize((int) (width / tile));//creada matriz de 50x50
        int k = 0;
        for (int i = hmin; i < width / 2; i += tile, k++) {
            int l = 0;
            for (int j = hmin;
                 j < width / 2; j += tile, l++)//recorriendo coordenadas de la celda, indexada por tile(100 en 100)
            {
                array[k][l] = Value{false, nullptr, nullptr, i, j, k, l, -1};//coordenadas del centro
            }
        }
    };

    struct Value {
        bool occupied = false;
        QGraphicsRectItem *paint_cell = nullptr;
        QGraphicsTextItem *text_cell = nullptr;
        int cx, cy;//coordenadas del centro en el mundo
        int k, l;
        int dist = 0; //dist vecinos
    };

    std::vector<std::vector<Value>> array;

    void create_graphic_items(QGraphicsScene &scene, QGraphicsView *view) {
        auto fondo = QColor("LightGreen");
        fondo.setAlpha(40);
        QFont font("Bavaria");
        font.setPointSize(40);
        font.setWeight(QFont::TypeWriter);
        for (auto &row : array)
            for (auto &elem : row) {
                elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")),
                                                QBrush(fondo));
                elem.paint_cell->setPos(elem.cx, elem.cy);
                elem.text_cell = scene.addText("-1", font);
                elem.text_cell->setPos(elem.cx - tile / 2, elem.cy - tile / 2);
                // Get the current transform
                QTransform transform(elem.text_cell->transform());
                qreal m11 = transform.m11();    // Horizontal scaling
                qreal m12 = transform.m12();    // Vertical shearing
                qreal m13 = transform.m13();    // Horizontal Projection
                qreal m21 = transform.m21();    // Horizontal shearing
                qreal m22 = transform.m22();    // vertical scaling
                qreal m23 = transform.m23();    // Vertical Projection
                qreal m31 = transform.m31();    // Horizontal Position (DX)
                qreal m32 = transform.m32();    // Vertical Position (DY)
                qreal m33 = transform.m33();    // Addtional Projection Factor
                // Vertical flip
                m22 = -m22;
                // Write back to the matrix
                transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                // Set the items transformation
                elem.text_cell->setTransform(transform);
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
    void set_Value(int x, int z, bool v) {

        if (auto r = transformar(x, z); r.has_value())//metodo que pases x,z y devuelva i,j, acceder a valores del mundo
        {
            auto[i, j] = r.value();
            array[i][j].occupied = v;
            if (v)
                array[i][j].paint_cell->setBrush(QColor("Red"));
            else
                array[i][j].paint_cell->setBrush(QColor("Green"));
        }
    }
    /**
     * devolvemos el valor de la coordenada x,z
     * @param x
     * @param z
     * @return
     */
    std::optional<Value> get_value(int x, int z) {
        if (auto v = transformar(x, z); v.has_value()) {
            auto[x, y] = v.value();
            return array[x][y];
        } else {
            return {};
        }
    }
    float getDist(float x, float y){

    }

    bool get_occupied(int x, int z) {
        return (this->array[x][z].occupied);
    }

    void set_dist(int x, int y, int dist2) {
        auto[x2, y2] = this->transformar(x, y);
        this->array[x2, y2].dist = dist2;
    }

    std::optional<std::tuple<int, int>> transformar(int i, int j) {

        int k = i / tile + (width / tile) / 2;
        int l = j / tile + (width / tile) / 2;

        return std::make_tuple(k, l);
    }

    int transformar_mapa(int i) {
        return i / tile + (width / tile) / 2;
    }

    void reset() {
        for (auto &row : array)
            for (auto &elem : row)
                elem.dist = -1;
    }


    std::vector<Value> vecinos(Value &v, int dist) {
        std::vector<Value> lista;
        for (auto[dk, dl] : lista_coor_de_vecinos) {
            int k = v.k + dk;        // OJO hay que añadir al struct Value las coordenadas de array
            int l = v.l + dl;
            //           if (k >= -2500 && k < 2500 && l >= -2500 && l < 2500) {   //Limites
            if (k > 0 and l > 0 and l < array.size() and k < array.size() and !array[k][l].occupied and
                array[k][l].dist == -1) {
                this->array[k][l].dist = dist;
                this->array[k][l].text_cell->setPlainText(QString::number(dist));
                lista.push_back(this->array[k][l]);
            }
            //}
        }

        return lista;
    }

    void navigation(Value &v) {
        reset();
        int dist = 0;
        auto listaCasillas1 = vecinos(v, dist);
        std::vector<Value> listaCasillas2 = {};

        bool fin = false;
        while (!fin) {
            for (auto &current_cell : listaCasillas1) {
                auto selected = vecinos(current_cell, dist);
                listaCasillas2.insert(listaCasillas2.end(), selected.begin(), selected.end());
            }
            dist++;
            fin = listaCasillas2.empty();
            listaCasillas1.swap(listaCasillas2);
            listaCasillas2.clear();
        }
    }
};


#endif //GOTOXY_GRID_H
