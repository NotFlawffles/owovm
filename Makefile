CPP = c++
CPPFLAGS = -Wall -Wextra -o
OBJECT = owovm
SRC = $(wildcard src/*)
INSTALLATION_PATH = $(PREFIX)/bin


default: $(SRC)
	$(CPP) $(CPPFLAGS) $(OBJECT) $(SRC)


clean: $(OBJECT)
	rm $(OBJECT)


install: $(INSTALLATION_PATH)
	make default
	mv $(OBJECT) $(INSTALLATION_PATH)


uninstall: $(INSTALLATION_PATH)/$(OBJECT)
	rm $(INSTALLATION_PATH)/$(OBJECT)
