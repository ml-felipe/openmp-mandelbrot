CXX ?= g++
CXXFLAGS ?= -O2 -g
.PHONY: all test clean
all: build/block build/cyclic build/block_cyclic

build/%: src/%.cpp run.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -std=c++17 -fopenmp -DROWS=512 -DCOLS=512 -DSOURCE='"src/$*.cpp"' run.cpp -o $@

test:
	@mkdir -p build
	@set -e; for variant in block cyclic block_cyclic; do \
	  for dims in '7 11' '17 35' '16 64'; do \
	    set -- $$dims; \
	    $(CXX) $(CXXFLAGS) -std=c++17 -fopenmp -DROWS=$$1 -DCOLS=$$2 \
	      -DSOURCE='"../src/'$$variant'.cpp"' tests/check.cpp -o build/check; \
	    echo "$$variant"; ./build/check; \
	  done; \
	done
clean:
	rm -rf build
