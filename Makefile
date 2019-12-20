test:
	$(MAKE) -C tst
	$(MAKE) -C examples/hello_world

clean:
	$(MAKE) -C tst clean
	$(MAKE) -C examples/hello_world clean
