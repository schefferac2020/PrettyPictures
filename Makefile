
clean:
	rm -rf fractal.exe
	rm -rf fractal.exe.dSYM 
fractal:
	g++ $$(pkg-config --cflags --libs /usr/local/lib/pkgconfig/opencv4.pc) -std=c++1z -g3 -DDEBUG main.cpp -o fractal.exe
