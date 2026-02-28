# Gemini

(from: https://downloads.reactivemicro.com/Apple%20II%20Items/ROM_and_JEDEC/II_&_II+/)


2. How to "Build" the 12 KB File
If you download the individual files from ReActiveMicro, you need to join them in a specific order to create the single 12 KB image ($D000 to $FFFF) that many emulators expect:

341-0011.bin ($D000 - $D7FF)

341-0012.bin ($D800 - $DFFF)

341-0013.bin ($E000 - $E7FF)

341-0014.bin ($E800 - $EFFF)

341-0015.bin ($F000 - $F7FF)

341-0020.bin ($F800 - $FFFF) — This is your Monitor ROM



cat 341-0011.bin 341-0012.bin 341-0013.bin 341-0014.bin 341-0015.bin 341-0020.bin > apple2plus_12k.rom


3. Verification
Once you have the merged file, run md5sum. If it is the standard Apple II+ set, it should return: 69477e361280053916962f928a6f4e24

