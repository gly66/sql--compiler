VPATH = code
# vpath outcome.txt code
lexical: 
	g++ -o lexical.out -std=c++11 ./code/main.cpp
# main.o: 
# 	g++ -c $< -std=c++11 -o main.o
run:
	./lexical.out ./test.txt > outcome.tsv
test:
	./lexical.out ./test0A.sql > Alex.tsv
	./lexical.out ./test0B.sql > Blex.tsv
.PHONY: clean
clean:
	rm *.out
	rm *.tsv