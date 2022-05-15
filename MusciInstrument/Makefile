# Instrument Makefile by Olivia Stone for Code Clinic: C++
CC		= g++
TARGET	= Instrument
SRC		= Instrument.cpp InstrumentWinMain.cpp
LDLIBS	= -L/cygdrive/c/Users/amustafa/Desktop/MusciInstrument -lao

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)