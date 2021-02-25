ALL = polymorphism
CXXFLAGS = -Wall -O2 -std=c++17 `pkg-config --cflags openssl`
LDFLAGS = -lboost_system -lpthread `pkg-config --libs openssl`

all: $(ALL)

polymorphism: polymorphism.o sync_stream_wrapper.o
	$(CXX) -o $@ $^ $(LDFLAGS)


clean:
	rm -f *.o
	rm -f *.d
	rm -f $(ALL)
