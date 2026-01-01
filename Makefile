CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iheaders
LDFLAGS  = -lsqlite3
CRYPT = -lssl -lcrypto

SRC_DIR = src
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TARGET = $(BIN_DIR)/main
DB = data/auth.db

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(CRYPT) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGET) $(DB)
