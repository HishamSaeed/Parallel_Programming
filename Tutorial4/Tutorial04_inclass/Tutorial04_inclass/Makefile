all:
	gcc -fopenmp -o ex1 ex1.c
	gcc -fopenmp -o ex2 ex2.c
	gcc -fopenmp -o ex3 ex3.c
	gcc -fopenmp -o ex4 ex4.c
	gcc -fopenmp -o ex5 ex5.c
	gcc -fopenmp -o ex6 ex6.c
	gcc -fopenmp -o fibo_seq fibonacci_seq.c
	gcc -fopenmp -o fibo_par fibonacci.c
	gcc -fopenmp -o fibo_final fibonacci_final.c
	gcc -fopenmp -o fibo_hybrid fibonacci_hybrid.c
	g++ -std=c++11 -fopenmp -o tree_seq tree_seq.c
	g++ -std=c++11 -fopenmp -g -o tree_par tree_par.c
run:
	@echo "running example 1:" && ./ex1
	@echo "running example 2:" && ./ex2
	@echo "running example 3:" && ./ex3
	@echo "running example 4:" && ./ex4
	@echo "running example 5:" && ./ex5
	@echo "running example 6:" && ./ex6
	@echo "running example fibonacci sequential:" && ./fibo_seq
	@echo "running example fibonacci parallel :" && ./fibo_par
	@echo "running example fibonacci final :" && ./fibo_final 	
	@echo "running example fibonacci hybrid :" && ./fibo_hybrid 
clean:
	rm -rf ex1 ex2 ex3 ex4 ex5 ex6 fibo_seq fibo_par fibo_final fibo_hybrid tree_par tree_seq
