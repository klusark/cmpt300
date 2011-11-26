all: Monitor.o HDMonitor.o MonitorDriver.o SSTF.o Elevator.o
	g++ -ggdb Monitor.o HDMonitor.o MonitorDriver.o SSTF.o Elevator.o -lpthread -lrt
Monitor.o: Monitor.cpp Monitor.h
	g++ -c Monitor.cpp
MonitorDriver.o: MonitorDriver.cpp
	g++ -c MonitorDriver.cpp
HDMonitor.o: HDMonitor.cpp HDMonitor.h
	g++ -c HDMonitor.cpp
SSTF.o: SSTF.cpp SSTF.h
	g++ -c SSTF.cpp
Elevator.o: Elevator.cpp Elevator.h
	g++ -c Elevator.cpp
clean:
	rm *.o
	rm ./a.out
