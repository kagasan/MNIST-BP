/*
Mnistのデータを読み込み・管理する
*/

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

const std::string TrainImage = "train-images.idx3-ubyte";
const std::string TrainLabel = "train-labels.idx1-ubyte";
const std::string TestImage = "t10k-images.idx3-ubyte";
const std::string TestLabel = "t10k-labels.idx1-ubyte";

class Images{
private:
	std::vector<double>vec;
	int MagicNumber;
	int NumberOfImages;
	int Rows;
	int Cols;

	void Reverse(int *i){
		unsigned char c1, c2, c3, c4;
		c1 = (*i) & 255;
		c2 = ((*i) >> 8) & 255;
		c3 = ((*i) >> 16) & 255;
		c4 = ((*i) >> 24) & 255;
		(*i) = ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
	}

public:
	void Read(std::string filename){
		std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
		ifs.read((char*)&MagicNumber, sizeof(MagicNumber));
		Reverse(&MagicNumber);
		ifs.read((char*)&NumberOfImages, sizeof(NumberOfImages));
		Reverse(&NumberOfImages);
		ifs.read((char*)&Rows, sizeof(Rows));
		Reverse(&Rows);
		ifs.read((char*)&Cols, sizeof(Cols));
		Reverse(&Cols);
		vec.resize(NumberOfImages*Rows*Cols);
		for (int i = 0; i < NumberOfImages;i++){
			for (int row = 0; row < Rows;row++){
				for (int col = 0; col < Cols;col++){
					unsigned char tmp;
					ifs.read((char*)&tmp, sizeof(tmp));
					vec[i*Rows*Cols + row*Cols + col] = tmp;
					vec[i*Rows*Cols + row*Cols + col] /= 255;
				}
			}
		}
	}

	double at(int id, int row, int col){
		return vec[id*Rows*Cols + row*Cols + col];
	}

	void View(int id){
		for (int row = 0; row < Rows;row++){
			for (int col = 0; col < Cols;col++){
				if (at(id, row, col)>0.5)std::cout << "#";
				else std::cout << " ";
			}
			std::cout << std::endl;
		}
	}
};

class Labels{
private:
	std::vector<int>vec;
	int MagicNumber;
	int NumberOfItems;

	void Reverse(int *i){
		unsigned char c1, c2, c3, c4;
		c1 = (*i) & 255;
		c2 = ((*i) >> 8) & 255;
		c3 = ((*i) >> 16) & 255;
		c4 = ((*i) >> 24) & 255;
		(*i) = ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
	}

public:
	void Read(std::string filename){
		std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
		ifs.read((char*)&MagicNumber, sizeof(MagicNumber));
		Reverse(&MagicNumber);
		ifs.read((char*)&NumberOfItems, sizeof(NumberOfItems));
		Reverse(&NumberOfItems);
		vec.resize(NumberOfItems);
		for (int i = 0; i < NumberOfItems;i++){
			unsigned char tmp;
			ifs.read((char*)&tmp, sizeof(tmp));
			vec[i] = tmp;
		}
	}

	int at(int id){
		return vec[id];
	}

	void View(int id){
		std::cout << vec[id] << std::endl;
	}
};

class DataSet{
private:
	Images imgs;
	Labels lbls;

public:
	DataSet(int Test = 0){
		if (Test){
			imgs.Read(TestImage);
			lbls.Read(TestLabel);
		}
		else{
			imgs.Read(TrainImage);
			lbls.Read(TrainLabel);
		}
	}

	void View(int id){
		lbls.View(id);
		imgs.View(id);
	}

};

void test(){
	DataSet ds;
	for (int i = 0; i < 10;i++)ds.View(i);
}
