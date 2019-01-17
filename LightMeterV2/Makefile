.PHONY: all clean upload monitor lint format

all:
	pio run

lint:
	cpplint --extensions=cpp,h,ino $(shell find src  \( ! -regex '.*/\..*' \) \
				-type f -a \( -name "*\.cpp" -o -name "*\.h" -o -name "*\.ino" \) )

clean:
	pio run --target clean

upload:
	pio run --target upload

monitor:
	pio run --target monitor

format:
	clang-format -i src/*.*
