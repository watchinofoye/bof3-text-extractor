all: extractor jpextractor
extractor: src_c/extractor.c src_c/utils.c
	gcc -o extractor src_c/extractor.c src_c/utils.c -O2
jpextractor: src_c/jpextractor.c src_c/jputils.c
	gcc -o jpextractor src_c/jpextractor.c src_c/jputils.c -O2
clean:
	rm -rf extractor jpextractor
install: extractor jpextractor
	cp -t /usr/local/bin/ extractor jpextractor 