.PHONY: build
.DEFAULT: build

PROBLEM := RoadNetwork
CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2 -g -DLOCAL

build: a.out tester.jar

run: a.out tester.jar
	java -jar tester.jar -exec ./a.out

a.out: main.cpp ${PROBLEM}.cpp
	${CXX} ${CXXFLAGS} $<

${PROBLEM}Vis.class: ${PROBLEM}Vis.java
	javac $<

tester.jar: ${PROBLEM}Vis.class
	jar cvfe tester.jar ${PROBLEM}Vis *.class

URL := https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17427&pm=15314
submit:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open
submit/full:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open --full
