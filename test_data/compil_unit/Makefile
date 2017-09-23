main: foo.o bar.o main.o
	$(CXX) $(CXXFLAGS) foo.o bar.o main.o -o main

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o
	rm main
	rm *.ast
	rm *.ldmap
	rm *.crmap
