help:
	@echo "make help - this message"
	@echo "make run - build and run the project and clean up afterwards"
	@echo "make build - build the project"
	@echo "make play - play the project"
	@echo "make clean - remove all generated files"

run:
	@echo "Compiling game ..."
	@gcc -o main main.c -lncurses
	@echo "Done."
	@./main
	@echo "Cleaning up ..."
	@rm main
	@echo "Done."

build:
	@echo "Compiling game ..."
	@gcc -o main main.c -lncurses
	@echo "Done."
	@echo "Run with 'make play'"

play:
	@echo "Running game ..."
	@./main
	@echo "Hope you enjoyed it!"
	@echo "clean up with 'make clean'"

clean:
	@echo "Cleaning up ..."
	@rm -f main
	@echo "Done."
