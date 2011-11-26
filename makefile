all: Monitor.o HDMonitor.o MonitorDriver.o SSTF.o
	g++ -ggdb Monitor.o HDMonitor.o MonitorDriver.o SSTF.o -lpthread -lrt
Monitor.o:
	g++ -c Monitor.cpp
MonitorDriver.o:
	g++ -c MonitorDriver.cpp
HDMonitor.o:
	g++ -c HDMonitor.cpp
SSTF.o:
	g++ -c SSTF.cpp
clean:
	rm *.o
	rm ./a.out
