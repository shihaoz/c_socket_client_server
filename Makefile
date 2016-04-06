
all: clean test
test:
	g++ server.cpp -o test

clean:
	rm test