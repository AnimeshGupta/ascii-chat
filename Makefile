#!/usr/bin/make -f


EXT_CDEPS   = fmemopen

BIN_D       = bin
OUT_D       = build

CC          = clang
CXX         = clang++

CFLAGS      = -O3 -g -std=c99
CXXFLAGS    = -O3 -g -std=c++11 -stdlib=libc++
CFLAGS     += -DUSE_CLANG_COMPLETER
CXXFLAGS   += -DUSE_CLANG_COMPLETER
CFLAGS     += -isystem /usr/include -I . -x c
CXXFLAGS   += -isystem /usr/include -I . -x c++

CFLAGS_W   = -Wextra -Wno-unused-parameter
CXXFLAGS_W = -Wextra -Wno-unused-parameter

LDFLAGS     = -lstdc++ -ljpeg
LDFLAGS    += `pkg-config --libs opencv`

TARGETS     = $(addprefix $(BIN_D)/, server client)

OBJS_C      = $(patsubst %.c,   $(OUT_D)/%.o, $(wildcard *.c))
OBJS_CPP    = $(patsubst %.cpp, $(OUT_D)/%.o, $(wildcard *.cpp))
OBJS_CEXT   = $(patsubst %.c,   $(OUT_D)/%.o, $(wildcard $(addprefix ext/, $(EXT_CDEPS))/*.c))

OBJS        = $(OBJS_C) $(OBJS_CPP) $(OBJS_CEXT)
OBJS_       = $(filter-out $(patsubst $(BIN_D)/%, $(OUT_D)/%.o, $(TARGETS)), $(OBJS))
# ^ non-targets; files without main methods

HEADERS_C    = $(wildcard *.h)
HEADERS_CPP  = $(wildcard *.hpp)
HEADERS_CEXT = $(wildcard $(addprefix ext/, $(EXT_CDEPS))/*.h)

HEADERS      = $(HEADERS_C) $(HEADERS_CPP) $(HEADERS_CEXT)

# util
RM = /bin/rm


.DEFAULT: default
.PHONY: all clean

.PRECIOUS: $(OBJS_)


default: $(TARGETS)

all: compile_commands.json default


$(TARGETS): $(BIN_D)/%: $(OUT_D)/%.o $(OBJS_)
	$(CXX) -o $@ \
	$(LDFLAGS) \
	$?

$(OBJS_C): $(OUT_D)/%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ \
	$(CFLAGS) \
	$(CFLAGS_W)

$(OBJS_CPP): $(OUT_D)/%.o: %.cpp $(HEADERS)
	$(CXX) -c $< -o $@ \
	$(CXXFLAGS) \
	$(CXXFLAGS_W)

$(OBJS_CEXT): $(OUT_D)/%.o: %.c $(HEADERS_CEXT)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ \
	$(CFLAGS) \
	$(CFLAGS_W)

compile_commands.json: Makefile
	@tmp="`mktemp -d "$$TMPDIR"'ascii-chat.XXXXX' || exit 1`"; \
	cdb="$$tmp/compile_commands.json"; \
	hdb="$$tmp/compile_commands.hdb.json"; \
	dbs=("$$cdb" "$$hdb"); \
	[[ -f './compile_commands.json' ]] && make clean; \
	bear --cdb "$$cdb" make              || exit 2; \
	compdb headerdb -p "$$tmp" > "$$hdb" || exit 3; \
	cat "$${dbs[@]}" | jq -s 'add | sort_by(.file)' > \
		'./compile_commands.json'        || exit 4; \
	$(RM) -f "$${dbs[@]}"                || exit 5;

clean:
	@echo 'cleaning...'
	@printf '\t'
	@find $(OUT_D) -mindepth 1 \
		-type f -not -iname '.gitkeep' \
		-printf '%P ' -delete
	@find $(BIN_D) -mindepth 1 \
		-type f -not -iname '.gitkeep' \
		-printf '%P ' -delete
	@printf '\n'
	@$(RM) -f compile_commands.json
	@echo 'done!'
	@echo
