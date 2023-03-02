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
PROGS=$(patsubst %, $(BUILD_DIR)/%, build_abr filtrage)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
CONTENU_ZIP=$(SRC_DIR) $(INC_DIR) .clang-format .clang-tidy Makefile rapport.pdf
# $(info $(PROGS))

all: $(BUILD_DIR)/build_abr $(BUILD_DIR)/filtrage 

$(BUILD_DIR)/build_abr: $(patsubst %, ./$(BUILD_DIR)/%, build_abr.o ABR.o)
	$(CC) $^ -o $@ $(LIBS)
$(BUILD_DIR)/filtrage: $(patsubst %, ./$(BUILD_DIR)/%, filtrage.o ABR.o)
	$(CC) $^ -o $@ $(LIBS)

filtrage.o: filtrage.c ABR.h
build_abr.o: build_abr.c ABR.h
ABR.o: ABR.c ABR.h

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
