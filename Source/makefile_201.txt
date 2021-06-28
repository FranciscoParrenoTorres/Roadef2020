
CC      = gcc
CFLAGS  =  -W"switch" -W"no-deprecated-declarations" -W"empty-body" -W"conversion" -W"return-type" -W"parentheses" -W"no-format" -W"uninitialized" -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -Wsign-compare -fpermissive -frtti -fno-omit-frame-pointer -std=c11


CXX          = g++
LDFLAGS      = -O3 -DIL_STD -fthreadsafe-statics -flto -Wswitch -Wno-deprecated-declarations -Wempty-body -Wconversion -Wreturn-type -Wparentheses -Wno-pointer-sign -Wno-format -Wuninitialized -Wunreachable-code -Wunused-function -Wunused-value -Wunused-variable -Wsign-compare -std=c++11 -funroll-all-loops -Wall -fno-strict-aliasing -I /opt/ibm/ILOG/CPLEX_Studio201/concert/include -I /opt/ibm/ILOG/CPLEX_Studio201/cplex/include -g2 -gdwarf-2 
LIBS 	    = -fPIC -lilocplex -lconcert -lcplex -lm -lpthread -ldl -fopenmp -L /opt/ibm/ILOG/CPLEX_Studio201/concert/lib/x86-64_linux/static_pic -L /opt/ibm/ILOG/CPLEX_Studio201/cplex/lib/x86-64_linux/static_pic



$(OUT): $(OBJFILES)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

OBJSC =  MaintenanceCPlex.o Constructive.o Tools.o 
OBJSF  = 
OBJS= $(OBJSC) $(OBJSF)

default: Roadef2020

all: $(CPROGS)

clean:	
	rm -f core  
	rm -f salida* 
	rm -f sal* 
	rm -f *.o  
	rm -f *.exe  

Roadef2020 :   main.o $(OBJS)
	$(CXX) $(LDFLAGS) -o ChallengeRTE.exe main.o $(OBJS) $(LIBS)

.f.o:
	$(F77) $(FFLAGS) -c $<

.cpp.o:
	$(CXX) -c $(LDFLAGS) -c $(LIBS) $<

.c.o:
	$(CC) -c $(CFLAGS) -c $(LIBS) $<


