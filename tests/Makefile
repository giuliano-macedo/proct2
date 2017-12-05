CC=g++ -Wall -Wno-unused-function -I../include/
F=../src/fft.c ../src/lodepng.cpp -lm
canny:canny.cpp
	 $(CC) canny.cpp $(F) -o canny
kmeans:kmeans.c
	 $(CC) kmeans.c $(F) -o kmeans
fftRings2:fftRings2.cpp
	 $(CC) fftRings2.cpp $(F) -o fftRings2
fftRings:fftRings.cpp
	$(CC) fftRings.cpp $(F) -o fftRings
fftTestImage:fftTestImage.c
	$(CC) fftTestImage.c $(F) -o fftTestImage
saveTest:saveTest.c
	$(CC) saveTest.c $(F) -o saveTest
clean:
	rm -rf fftTestImage saveTest fftRings fftRings2 canny kmeans
