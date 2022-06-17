compile: banker.c
	gcc banker.c -o banker

run:
	./banker 10 5 7

clean:
	rm -rf *.o banker result.txt
