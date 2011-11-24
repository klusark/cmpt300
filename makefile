all: Monitor.o HDMonitor.o MonitorDriver.o
	g++ -ggdb Monitor.o HDMonitor.o MonitorDriver.o -lpthread -lrt
Monitor.o:
	g++ -c Monitor.cpp Monitor.h -lrt
MonitorDriver.o:
	g++ -c MonitorDriver.cpp
HDMonitor.o:
	g++ -c HDMonitor.cpp HDMonitor.h

clean:
	rm *.o
	rm ./a.out
