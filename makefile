all: go.c user.c calculator.c
	gcc go.c -o go
	gcc user.c -o user
	gcc calculator.c -o calculator

clean:
	rm -f go user calculator