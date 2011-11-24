all: Monitor.o MonitorDriver.o
	g++ -ggdb Monitor.o MonitorDriver.o -lpthread -lrt
Monitor.o:
	g++ -c Monitor.cpp -lrt
MonitorDriver.o:
	g++ -c MonitorDriver.cpp

clean:
	rm ./a.out
	rm *.o
