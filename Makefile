#Makefile

all:
	$(MAKE) -C ui/display-image
	$(MAKE) -C neural-network
	$(MAKE) -C picture-processing/pre-processing/grayscale-image
	$(MAKE) -C picture-processing/pre-processing/rotate-image
	$(MAKE) -C picture-processing/pre-processing/split-grid
	$(MAKE) -C picture-processing/post-processing/solver


.PHONY: clean

clean:
	$(MAKE) clean -C ui/display-image
	$(MAKE) clean -C neural-network
	$(MAKE) clean -C picture-processing/pre-processing/grayscale-image
	$(MAKE) clean -C picture-processing/pre-processing/rotate-image
	$(MAKE) clean -C picture-processing/pre-processing/split-grid
	$(MAKE) clean -C picture-processing/post-processing/solver

#END
