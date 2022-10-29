main:
	@echo "Compiling game ..."
	@gcc -o main main.c -lncurses
	@echo "Done."
	@echo "Run with ./main"

clean:
	@echo "Cleaning up ..."
	@rm -f main
	@echo "Done."
