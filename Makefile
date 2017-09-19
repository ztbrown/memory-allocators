export BUILDDIR=$(CURDIR)
export CFLAGS+= -Wall -Werror -std=c99

all: clean
	$(MAKE) -C src

check: clean
	docker run -it --rm -P -v $(CURDIR):/code -w="/code" lib-check /bin/sh -c "make -s -C tests && ./tests/test_suite"

clean:
	rm -rf **/*.o
