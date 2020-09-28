#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include <functional>
#include <future>
#include <cstdio>
#include <iostream>

class Timer
{
    public:

        Timer(){};

        template <class callable>        
        void connect(callable&& f)//objeto callable
        {
			std::thread([=]() //hilo
            {
                while(true)//bucle infinito
                {
					if(go.load())
						std::invoke(f);//invoca al objeto f
                    std::this_thread::sleep_for(std::chrono::milliseconds(period.load()));//manda a dormir durante el periodo
                }
            }).detach();//separa el hilo
        };
        
        void start(int p)//comienzo del timer
        {
			period.store(p);//almacena periodo
			go.store(true);
        };
        
        void stop() { go.store(!go); };//parar el timer(invertimos boolean go)
		void setPeriod(int p) { period.store(p) ;};//poner periodo
        
    private:
        std::atomic_bool go = false;//booleano atomico para sincronización
		std::atomic_int period = 0;
        
    
};

#endif // TIMER_H
