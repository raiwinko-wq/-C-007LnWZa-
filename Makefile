# [Universal Makefile] ตรวจสอบระบบปฏิบัติการอัตโนมัติ
ifeq ($(OS),Windows_NT)
    # สำหรับ Windows (เครื่อง Lenovo ของคุณ)
    CXX = g++
    CXXFLAGS = -I"C:\SFML\include" -std=c++17
    LDFLAGS = -L"C:\SFML\lib" -lsfml-graphics -lsfml-window -lsfml-system
    TARGET = game.exe
    CLEAN_CMD = del /f $(TARGET)
else
    # สำหรับ Mac (เครื่อง MacBook ของเพื่อน)
    CXX = g++
    CXXFLAGS = -std=c++17 -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system
    TARGET = game
    CLEAN_CMD = rm -f $(TARGET)
endif

SRCS = main.cpp

all:
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

clean:
	$(CLEAN_CMD)