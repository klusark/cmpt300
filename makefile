all: Monitor.o MonitorDriver.o
	g++ -ggdb Monitor.o MonitorDriver.o -lpthread
Monitor.o:
	g++ -c Monitor.cpp
MonitorDriver.o:
	g++ -c MonitorDriver.cpp

clean:
	rm ./a.out
	rm *.o
