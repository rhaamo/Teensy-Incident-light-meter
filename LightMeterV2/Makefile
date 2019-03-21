.PHONY: all clean upload monitor lint format

PIO := /home/dashie/.platformio/penv/bin/pio

all:
	${PIO} run

lint:
	cpplint --extensions=cpp,h,ino $(shell find src  \( ! -regex '.*/\..*' \) \
				-type f -a \( -name "*\.cpp" -o -name "*\.h" -o -name "*\.ino" \) )

clean:
	${PIO} run --target clean

upload:
	${PIO} run --target upload

monitor:
	${PIO} run --target monitor

format:
	clang-format -i src/*.*
