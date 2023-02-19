CC=gcc
BUILD_DIR=build
SRC_DIR=src
INC_DIR=include
INCLUDE=-I$(INC_DIR)
LIBS=-lm
CFLAGS=-fdiagnostics-color=always -Wall -pedantic -std=gnu17 -g -O0
DM_N=1
NOM_ZIP=TP$(DM_N)_SEBAN_POUYANFAR.zip

SOURCES=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(INC_DIR)/*.h)
PROGS=$(patsubst %, $(BUILD_DIR)/%, main build_abr)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
CONTENU_ZIP=$(SRC_DIR) $(INC_DIR) .clang-format .clang-tidy Makefile rapport.pdf
# $(info $(PROGS))

all: $(BUILD_DIR)/main $(BUILD_DIR)/build_abr

$(BUILD_DIR)/main: $(patsubst %, ./$(BUILD_DIR)/%, main.o ArbreBinaire.o ArbreBinaireRecherche.o Graph.o)
	$(CC) $^ -o $@ $(LIBS)
$(BUILD_DIR)/build_abr: $(patsubst %, ./$(BUILD_DIR)/%, build_abr.o ArbreBinaire.o ArbreBinaireRecherche.o Graph.o)
	$(CC) $^ -o $@ $(LIBS)

main.o: main.c ArbreBinaireRecherche.h Graph.h
build_abr.o: build_abr.c ArbreBinaireRecherche.h Graph.h
ArbreBinaire.o: ArbreBinaire.c ArbreBinaire.h
ArbreBinaireRecherche.o: ArbreBinaireRecherche.c ArbreBinaireRecherche.h ArbreBinaire.h
Graph.o: Graph.c Graph.h

# Création des fichiers objets à partir des fichiers sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

format: $(SOURCES) $(HEADERS)
	clang-format -i --style=file $?
	clang-tidy --fix $?

clean:
	rm -f $(PROGS) $(OBJS)
	rm -f $(NOM_ZIP)

zip:
	zip -FSr $(NOM_ZIP) $(CONTENU_ZIP)

rendu:
	@$(MAKE) --no-print-directory zip

.PHONY: clean all
