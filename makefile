all:clean txtSR
.POINY:textSR
txtSR:TextSimilarityRatio.cpp
	g++ $^ -o $@ -std=c++11 -I/home/zzz/openlib/cppjieba/include
.POINY:clean
clean:
	rm -rf txtSR
