main:
	g++ `pkg-config opencv --libs --cflags opencv` MyCamshift.cpp AdaBoostClassifier.cpp CascadeClassifier.cpp IntImage.cpp MyStruct.cpp SimpleClassifier.cpp main.cpp -o main
