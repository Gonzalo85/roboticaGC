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

void SpecificWorker::calcular(float &beta, float &distancia){
    auto data = target.get();
    differentialrobot_proxy->getBaseState(estado);
    if (data.has_value()) {
        d = data.value();
    }
    Eigen::Vector2f actual(estado.x, estado.z);
    Eigen::Matrix2f matriz;
    matriz << cos(estado.alpha), -sin(estado.alpha), sin(estado.alpha), cos(estado.alpha);//matriz traspuesta
    auto tr = matriz * (d - actual);
    distancia = tr.norm();//norm nos da la dist entre el robot y el target(transformacion matriz)
    beta = atan2(tr.x(), tr.y());
}

void SpecificWorker::compute() {
    float beta, distancia;
    calcular(beta, distancia);

    switch (estadoMaq) {
        case IDLE:
            idle();
            break;
        case TURN:
            turn(estado,beta);
            break;
        case GO:
            go(estado,distancia);
            break;
        case AROUND:
            around(estado);
            break;
    }
}

void SpecificWorker::idle() {
    differentialrobot_proxy->setSpeedBase(0, 0);
}

void SpecificWorker::turn(RoboCompGenericBase::TBaseState estado, float beta) {
    qDebug() << "Ángulo: "<< fabs(beta) << endl;
    if (fabs(beta) < 0.05) {
        estadoMaq = GO;
        differentialrobot_proxy->setSpeedBase(0, 0);
        return;
    } else {
        differentialrobot_proxy->setSpeedBase(0, beta);
    }
}

void SpecificWorker::go(RoboCompGenericBase::TBaseState estado,float distancia) {

    RoboCompLaser::TLaserData datos = laser_proxy->getLaserData();
    std::sort( datos.begin(), datos.end(), [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; });
    std::cout << "laser" <<datos.front().dist<<endl;

    if (distancia < 230) {
        estadoMaq = IDLE;
        target.set_task_finished();
        return;
    }

    if(datos.front().dist < 200) {
        estadoMaq = AROUND;
        std::cout << "Obstaculo encontrado" <<datos.front().dist<<endl;
        return;
    }else {
        differentialrobot_proxy->setSpeedBase(datos.front().dist, 0);
    }
}

void SpecificWorker::around(RoboCompGenericBase::TBaseState estado) {

    RoboCompLaser::TLaserData datos = laser_proxy->getLaserData();
    std::sort( datos.begin(), datos.end(), [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; });
    int aux=fabs((A*estado.x)+(B*estado.z)+C);
    std::cout<<aux <<endl;

    differentialrobot_proxy->setSpeedBase(0, 1);

    if(aux<16000){
        differentialrobot_proxy->setSpeedBase(0, 0);
        estadoMaq = TURN;
    }

    if(datos.front().dist < 300) {
        differentialrobot_proxy->setSpeedBase(0, -1);
    }else{
       differentialrobot_proxy->setSpeedBase(150, 0.4);
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

