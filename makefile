all: Monitor.o HDMonitor.o MonitorDriver.o SSTF.o Elevator.o
	g++ -ggdb Monitor.o HDMonitor.o MonitorDriver.o SSTF.o Elevator.o -lpthread -lrt
Monitor.o: Monitor.cpp Monitor.h
	g++ -ggdb -c Monitor.cpp
MonitorDriver.o: MonitorDriver.cpp
	g++ -ggdb -c MonitorDriver.cpp
HDMonitor.o: HDMonitor.cpp HDMonitor.h
	g++ -ggdb -c HDMonitor.cpp
SSTF.o: SSTF.cpp SSTF.h
	g++ -ggdb -c SSTF.cpp
Elevator.o: Elevator.cpp Elevator.h
	g++ -ggdb -c Elevator.cpp
clean:
	rm *.o
	rm ./a.out
