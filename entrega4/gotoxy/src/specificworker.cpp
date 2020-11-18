/*
 *    Copyright (C) 2020 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(TuplePrx tprx, bool startup_check) : GenericWorker(tprx) {
    this->startup_check_flag = startup_check;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker() {
    std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params) {
//	THE FOLLOWING IS JUST AN EXAMPLE
//	To use innerModelPath parameter you should uncomment specificmonitor.cpp readConfig method content
//	try
//	{
//		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
//		std::string innermodel_path = par.value;
//		innerModel = std::make_shared(innermodel_path);
//	}
//	catch(const std::exception &e) { qFatal("Error reading config params"); }
    return true;
}

void SpecificWorker::initialize(int period) {
    std::cout << "Initialize worker" << std::endl;
    this->Period = period;
    if (this->startup_check_flag) {
        this->startup_check();
    } else {
        timer.start(Period);
    }
}

void SpecificWorker::compute() {
    Eigen::Vector2f d;
    differentialrobot_proxy->getBaseState(estado);
    std::cout << "estadoMaq" << estadoMaq << endl;

//    if(auto data = target.get() ; data.has_value()) {
//        auto d = data.value();
//        std::cout << d.x() << std::endl;
//        std::cout << d.y() << std::endl;
//
//        Eigen::Vector2f actual(estado.x, estado.z);
//        Eigen::Matrix2f matriz;
//
//        matriz << cos(estado.alpha), sin(estado.alpha), -sin(estado.alpha), cos(estado.alpha);//matriz traspuesta
//
//        auto tr = matriz * (d - actual);
//        auto beta = atan2(tr.x(), tr.y());
//        c
//
//        qDebug() << "Distancia al objetivo" << distancia << endl << "Angulo beta: " << beta;
//    }

    switch (estadoMaq) {
        case IDLE:
            idle();
            break;
        case TURN:
            turn(estado);
            break;
        case GO:
            go(estado);
            break;
        case AROUND:
            around(estado);
            break;
    }

}

void SpecificWorker::idle() {
  //  qDebug() << "IDLE" << endl;
    differentialrobot_proxy->setSpeedBase(0, 0);
}

void SpecificWorker::turn(RoboCompGenericBase::TBaseState estado) {
    auto data = target.get();
    Eigen::Vector2f actual(estado.x, estado.z);
    Eigen::Matrix2f matriz;

    if (data.has_value()) {
        d = data.value();
    }
    matriz << cos(estado.alpha), -sin(estado.alpha), sin(estado.alpha), cos(estado.alpha);//matriz traspuesta
    auto tr = matriz * (d - actual);
    std::cout << "tr.x" << tr.x() << endl;
    std::cout << "tr.y" << tr.y() << endl;
    std::cout << "actual.x" << actual.x() << endl;
    std::cout << "actual.y" << actual.y() << endl;
    auto beta = atan2(tr.x(), tr.y());
    auto distancia = tr.norm();//norm nos da la dist entre el robot y el target(transformacion matriz)

    qDebug() << fabs(beta) << endl;
    if (fabs(beta) < 0.05) {
        estadoMaq = GO;
        differentialrobot_proxy->setSpeedBase(0, 0);
        return;
    } else {
        differentialrobot_proxy->setSpeedBase(0, beta);
    }

}

void SpecificWorker::around(RoboCompGenericBase::TBaseState estado) {
    Eigen::Vector2f actual(estado.x, estado.z);
    Eigen::Matrix2f matriz;
    matriz << cos(estado.alpha), -sin(estado.alpha), sin(estado.alpha), cos(estado.alpha);//matriz traspuesta
    auto tr = matriz * (d - actual);
    auto distancia = tr.norm();//norm nos da la dist entre el robot y el target(transformacion matriz)

    RoboCompLaser::TLaserData datos = laser_proxy->getLaserData();
    std::sort( datos.begin(), datos.end(), [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; });

    int aux=0;
    aux=fabs((A*estado.x)+(B*estado.z)+C);
    std::cout << "recta" <<A << "|" << B <<"|"<< C <<"|"<<aux <<endl;
    differentialrobot_proxy->setSpeedBase(0, 1);

    if(aux<15000){
        differentialrobot_proxy->setSpeedBase(0, 0);
        estadoMaq = TURN;
    }

    if(datos.front().dist < 300) {
        differentialrobot_proxy->setSpeedBase(0, -1);
    }else{
       differentialrobot_proxy->setSpeedBase(100, 0.3);
       return;
    }
}

void SpecificWorker::go(RoboCompGenericBase::TBaseState estado) {
    Eigen::Vector2f actual(estado.x, estado.z);
    Eigen::Matrix2f matriz;
    matriz << cos(estado.alpha), -sin(estado.alpha), sin(estado.alpha), cos(estado.alpha);//matriz traspuesta
    auto tr = matriz * (d - actual);
    auto distancia = tr.norm();//norm nos da la dist entre el robot y el target(transformacion matriz)
    std::cout << "distancia" << distancia << endl;

    RoboCompLaser::TLaserData datos = laser_proxy->getLaserData();
    std::sort( datos.begin(), datos.end(), [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; });
    std::cout << "laser" <<datos.front().dist<<endl;

    int velocidad=distancia;
    //if (datos.front().dist < distancia)
      //  velocidad=datos.front().dist;

    if(datos.front().dist < 300) {
        estadoMaq = AROUND;
        std::cout << "Obstaculo encontrado" <<datos.front().dist<<endl;
        return;
    }else {
        differentialrobot_proxy->setSpeedBase(500, 0);
    }

    if (distancia < 100) {
        estadoMaq = IDLE;
        target.set_task_finished();
        return;
    }
}

int SpecificWorker::startup_check() {
    std::cout << "Startup check" << std::endl;
    QTimer::singleShot(200, qApp, SLOT(quit()));
    return 0;
}


//SUBSCRIPTION to setPick method from RCISMousePicker interface
void SpecificWorker::RCISMousePicker_setPick(RoboCompRCISMousePicker::Pick myPick) {
    // std::cout << __FUNCTION__ << myPick.x << std::endl <<myPick.z << std::endl;

    target.put(Eigen::Vector2f(myPick.x, myPick.z));//coord del pick al struct target, copia segura     //X1, Y1


    differentialrobot_proxy->getBaseState(estado);                                                         //X2, Y2

    A=myPick.z-estado.z;
    B=estado.x-myPick.x;
    C=(myPick.x-estado.x)*myPick.z + (estado.z-myPick.z)*myPick.x;

    estadoMaq = TURN;

}

/**************************************/
// From the RoboCompDifferentialRobot you can call this methods:
// this->differentialrobot_proxy->correctOdometer(...)
// this->differentialrobot_proxy->getBasePose(...)
// this->differentialrobot_proxy->getBaseState(...)
// this->differentialrobot_proxy->resetOdometer(...)
// this->differentialrobot_proxy->setOdometer(...)
// this->differentialrobot_proxy->setOdometerPose(...)
// this->differentialrobot_proxy->setSpeedBase(...)
// this->differentialrobot_proxy->stopBase(...)

/**************************************/
// From the RoboCompDifferentialRobot you can use this types:
// RoboCompDifferentialRobot::TMechParams

/**************************************/
// From the RoboCompLaser you can call this methods:
// this->laser_proxy->getLaserAndBStateData(...)
// this->laser_proxy->getLaserConfData(...)
// this->laser_proxy->getLaserData(...)

/**************************************/
// From the RoboCompLaser you can use this types:
// RoboCompLaser::LaserConfData
// RoboCompLaser::TData

/**************************************/
// From the RoboCompRCISMousePicker you can use this types:
// RoboCompRCISMousePicker::Pick

