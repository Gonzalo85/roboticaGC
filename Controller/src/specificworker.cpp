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
SpecificWorker::SpecificWorker(MapPrx& mprx, bool startup_check) : GenericWorker(mprx)
{
	this->startup_check_flag = startup_check;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
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

void SpecificWorker::recto(RoboCompLaser::TLaserData datos, int distancia,int &state)
{
    static int random = 0;
    if(datos.front().dist<distancia){
        random=rand()%2;
        state = random+1;
        qDebug() << state;

    }else{
        differentialrobot_proxy->setSpeedBase(700,0);
        qDebug() << "recto";
        random=rand()%2;
        state = random+1;
    };

}

void SpecificWorker::girar(RoboCompLaser::TLaserData datos,int distancia, int &state)
{
    int random = 0;
    static int aux=1;
    if(datos.front().dist>distancia){
        state = 0;

    }else {
        qDebug() << "girar";
        qDebug() << datos.front().angle;
        random=rand()%2;
        if(random==0)
            aux = -1;
        else
            aux=1;
        differentialrobot_proxy->setSpeedBase(0, 0.7)*aux;
    }

}
void SpecificWorker::espiral(RoboCompLaser::TLaserData datos, int distancia, int &state) {
    int random = 0;
    if(datos.front().dist<distancia){
        state = 1;

    }else{
        differentialrobot_proxy->setSpeedBase(700, 0.5);
        random=rand()%2;
        if(random==0)
         state = 0;
        qDebug() << "espiral";
    }
}


void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;
	this->Period = period;
	if(this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}

}

//  camera_proxy->getYImage(0,img, cState, bState);
//  memcpy(image_gray.data, &img[0], m_width*m_height*sizeof(uchar));
//  searchTags(image_gray);
void SpecificWorker::compute()
{
    const int distancia=200;
    vector<int> recorrido;
    static RoboCompGenericBase::TBaseState estado;


    static int i=0;
    static int state=0;           //0:recto 1:girar 2:espiral
    differentialrobot_proxy->getBaseState(estado);
    differentialrobot_proxy->setSpeedBase(500,0);
    RoboCompLaser::TLaserData datos = laser_proxy->getLaserData();
    std::sort( datos.begin(), datos.end(), [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; });


	try
	{
        switch (state) {
            case 0:                 //0:recto
                recto(datos, distancia, state);
                break;
            case 1:
                girar(datos,distancia, state);
                break;
            case 2:
                espiral(datos,distancia, state);
                break;
        }
	}
	catch(const Ice::Exception &e)
	{
	     std::cout << "Laser error" << e << std::endl;
	}
	
	
}

int SpecificWorker::startup_check()
{
	std::cout << "Startup check" << std::endl;
	QTimer::singleShot(200, qApp, SLOT(quit()));
	return 0;
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

