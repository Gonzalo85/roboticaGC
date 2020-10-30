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

/**
	\brief
	@author authorname
*/



#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>
#include "Eigen/Dense"

class SpecificWorker : public GenericWorker
{
Q_OBJECT
    template <typename T>
    struct Target {
        T data;
        std::mutex mutex;
        bool active = false;
        bool empty = true;

        void put(const T &Data) {
            std::lock_guard<std::mutex> guard(mutex);
            data = Data;
            active = true;
            empty = false;
        }

        std::optional<T> get() {
            std::lock_guard<std::mutex> guard(mutex);
            if (active && !empty) {
                empty = true;
                return data;
            } else
                return {};
        }

        void set_task_finished() {
            std::lock_guard<std::mutex> guard(mutex);
            active = false;
        }
    };
    enum state{
        IDLE,
        TURN,
        GO
    };
    state estadoMaq = state::IDLE;
    int estado=0; //0 IDLE || 1 TURN || 2 GO

public:
	SpecificWorker(TuplePrx tprx, bool startup_check);
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);
	void RCISMousePicker_setPick(RoboCompRCISMousePicker::Pick myPick);
    void idle();
    void turn(RoboCompGenericBase::TBaseState estado);
    void go(float distancia);

public slots:
	void compute();
	int startup_check();
	void initialize(int period);
private:
	std::shared_ptr < InnerModel > innerModel;
	Target<Eigen::Vector2f> target;
	bool startup_check_flag;


};

#endif
