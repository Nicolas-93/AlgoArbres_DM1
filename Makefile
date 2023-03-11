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
PROGS=$(patsubst %, $(BUILD_DIR)/%, test_ABR filtrage)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
CONTENU_ZIP=$(SRC_DIR) $(INC_DIR) .clang-format .clang-tidy Makefile rapport.pdf texte.txt filtre.txt
#{en_commun, texte, filtrage, filtre}{.pdf, .dot} test_ABR{.pdf, .dot}
PRODUITS=en_commun.pdf texte.pdf filtrage.pdf filtre.pdf en_commun.dot texte.dot filtrage.dot filtre.dot test_ABR.pdf test_ABR.dot
# $(info $(PROGS))

all: $(BUILD_DIR)/test_ABR $(BUILD_DIR)/filtrage 

$(BUILD_DIR)/test_ABR: $(patsubst %, ./$(BUILD_DIR)/%, test_ABR.o ABR.o)
	$(CC) $^ -o $@ $(LIBS)
$(BUILD_DIR)/filtrage: $(patsubst %, ./$(BUILD_DIR)/%, filtrage.o ABR.o)
	$(CC) $^ -o $@ $(LIBS)

filtrage.o: filtrage.c ABR.h
test_ABR.o: test_ABR.c ABR.h
ABR.o: ABR.c ABR.h

# Création des fichiers objets à partir des fichiers sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

format: $(SOURCES) $(HEADERS)
	clang-format -i --style=file $?
	clang-tidy --fix $?

clean_outputs:
	rm -f $(PRODUITS)

clean:
	rm -f $(OBJS)

mrproper: clean clean_outputs 
	rm -f $(PROGS)
	rm -f $(NOM_ZIP)

zip:
	zip -FSr $(NOM_ZIP) $(CONTENU_ZIP)

rendu:
	@$(MAKE) --no-print-directory zip

.PHONY: clean all
