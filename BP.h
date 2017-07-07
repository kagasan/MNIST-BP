#include "MnistData.h"

#include <random>
#include <cmath>

class BP{
private:

	const int Hnum = 100;

	DataSet train;
	DataSet test;

	std::vector<double>w1;//(from, to)->[from*Hnum+to]
	std::vector<double>w2;//(from, to)->[from*10+to]

	std::vector<double>sigma1;
	std::vector<double>sigma2;
	std::vector<double>sigma3;

	std::vector<double>o1;
	std::vector<double>o2;
	std::vector<double>o3;
	
	std::vector<double>d2;
	std::vector<double>d3;


	void sigmoid(std::vector<double>*sigmavec, std::vector<double>*ovec){
		for (int i = 0; i < (*sigmavec).size();i++){
			(*ovec)[i] = 1.0 / (1.0 + std::exp(-(*sigmavec)[i]));
		}
	}

	//sigmoid'()
	double fd(double x){
		return 1.0 / (1.0 + std::exp(-x)) * (1.0 - 1.0 / (1.0 + std::exp(-x)));
	}

	void Read(){
		train.Read(0);
		test.Read(1);
	}

	void Propagation(int id){

		train.Copy(id, &sigma1);
		for (int i = 0; i < sigma1.size();i++){
			o1[i] = sigma1[i];
		}

		for (int j = 0; j < Hnum; j++){
			sigma2[j] = 0.0;
			for (int i = 0; i < 28 * 28; i++){
				sigma2[j] += w1[i*Hnum + j] * o1[i];
			}
			sigma2[j] /= Hnum;
		}
		sigmoid(&sigma2, &o2);

		for (int j = 0; j < 10; j++){
			sigma3[j] = 0.0;
			for (int i = 0; i < Hnum; i++){
				sigma3[j] += w2[i * 10 + j] * o2[i];
			}
			sigma3[j] /= 10;
		}
		for (int i = 0; i < 10; i++)o3[i] = sigma3[i];

		int ans = 0;
		for (int i = 0; i < 10; i++){
			if (o3[ans] < o3[i])ans = i;
		}

		int correct = train.label(id);


		for (int j = 0; j < 10; j++){
			double y = (j == correct ? 1.0 : 0.0);
			d3[j] = o3[j] - y;
		}

		for (int i = 0; i < Hnum; i++){
			for (int j = 0; j < 10; j++){
				w2[i * 10 + j] -= 0.5 * d3[j] * fd(sigma3[j]) * o2[i];
			}
		}

		for (int j = 0; j < Hnum; j++){
			d2[j] = 0;
			for (int l = 0; l < 10; l++){
				d2[j] += w2[j * 10 + l] * d3[l];
			}
			d2[j] *= fd(sigma2[j]);
		}

		for (int i = 0; i < 28 * 28; i++){
			for (int j = 0; j < Hnum; j++){
				w1[i*Hnum + j] -= 0.5*d2[j] * fd(sigma2[j])*o1[i];
			}
		}

	}

	int check(int id){
		test.Copy(id, &sigma1);
	
		for (int i = 0; i < sigma1.size(); i++){
			o1[i] = sigma1[i];
		}
		for (int j = 0; j < Hnum; j++){
			sigma2[j] = 0.0;
			for (int i = 0; i < 28 * 28; i++){
				sigma2[j] += w1[i*Hnum + j] * o1[i];
			}
			sigma2[j] /= Hnum;
		}
		sigmoid(&sigma2, &o2);

		for (int j = 0; j < 10; j++){
			sigma3[j] = 0.0;
			for (int i = 0; i < Hnum; i++){
				sigma3[j] += w2[i * 10 + j] * o2[i];
			}
			sigma3[j] /= 10;
		}

		for (int i = 0; i < 10; i++){
			o3[i] = sigma3[i];
		}
		int ans = 0;
		for (int i = 0; i < 10; i++){
			if (o3[ans] < o3[i])ans = i;
		}

		int correct = test.label(id);
		return ans == correct;
	}

public:

	BP(){
		std::random_device rd;
		std::mt19937 mt(rd());
		Read();
		for (int i = 0; i < 28 * 28 * Hnum; i++)w1.push_back((mt() % 65536) / 65535.0);
		for (int i = 0; i < Hnum * 10; i++)w2.push_back((mt() % 65536) / 65535.0);
		sigma1.resize(28 * 28);
		sigma2.resize(Hnum);
		sigma3.resize(10);
		
		o1.resize(28 * 28);
		o2.resize(Hnum);
		o3.resize(10);

		d2.resize(Hnum);
		d3.resize(10);

	}

	void Test(){
		int cnt = 0;
		for (int i = 0; i < test.Size(); i++){
			cnt += check(i);
		}
		std::cout << cnt << " / " << test.Size() <<" = "<< 1.0 * cnt/test.Size()<< std::endl;
	}

	void Do(){
		std::cout << "[0]:";
		Test();
		for (int i = 1; i <= 50;i++){
			for (int j = 0; j < train.Size();j++){
				Propagation(j);
				if(j%100==0)std::cout <<"["<<i<<"]:"<< j << " / " << train.Size() << "\r";
			}
			std::cout << "[" << i << "]:" << train.Size() << " / " << train.Size() << "\r";
			std::cout << "[" << i << "]:";
			Test();
		}
	}

};
