CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = main.o Config.o Printer.o Bank.o WATCard.o WATCardOffice.o \
		Parent.o Groupoff.o BottlingPlant.o NameServer.o Truck.o VendingMachine.o \
		Student.o
EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean

all : ${EXEC}					# build all executables

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

main.o: main.cc
	${CXX} ${CXXFLAGS} main.cc -c

Config.o: Config.cp
	${CXX} ${CXXFLAGS} Config.cp -c

Printer.o: Printer.cp
	${CXX} ${CXXFLAGS} Printer.cp -c

Bank.o: Bank.cp
	${CXX} ${CXXFLAGS} Bank.cp -c

WATCard.o: WATCard.cp
	${CXX} ${CXXFLAGS} WATCard.cp -c

WATCardOffice.o: WATCardOffice.cp
	${CXX} ${CXXFLAGS} WATCardOffice.cp -c

Parent.o: Parent.cp
	${CXX} ${CXXFLAGS} Parent.cp -c

Student.o: Student.cp
	${CXX} ${CXXFLAGS} Student.cp -c				

Groupoff.o: Groupoff.cp
	${CXX} ${CXXFLAGS} Groupoff.cp -c

BottlingPlant.o: BottlingPlant.cp
	${CXX} ${CXXFLAGS} BottlingPlant.cp -c

NameServer.o: NameServer.cp
	${CXX} ${CXXFLAGS} NameServer.cp -c

Truck.o: Truck.cp
	${CXX} ${CXXFLAGS} Truck.cp -c

VendingMachine.o: VendingMachine.cp
	${CXX} ${CXXFLAGS} VendingMachine.cp -c				
#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC}

