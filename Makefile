main:MyCamshift.cpp RotateImage.cpp AdaBoostClassifier.cpp CascadeClassifier.cpp IntImage.cpp MyStruct.cpp SimpleClassifier.cpp MatrixRotate.cpp main.cpp 
	g++ `pkg-config opencv --libs --cflags opencv`  MyCamshift.cpp RotateImage.cpp AdaBoostClassifier.cpp CascadeClassifier.cpp IntImage.cpp MatrixRotate.cpp MyStruct.cpp SimpleClassifier.cpp main.cpp -o main
