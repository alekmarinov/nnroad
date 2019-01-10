#ifndef __NET_H
#define __NET_H

#include <vector>
#include <iostream>

using namespace std;

typedef vector<double> DVector;
typedef vector<DVector> VectorVector;

#define RND	((double)rand()/(double)RAND_MAX)
#define SMALL_RND	((double)RND*0.1f-0.05f)
#define DFORMAT(d)	((int)(d * 1000) / (double)1000)

class Net
{
public:
	Net(int input_count, int hidden_count, int output_count, double learn_rate=0.3f, double momentum=0.7f, double momentum_rate=0.0f) : 
	_input_count(input_count), 
	_hidden_count(hidden_count), 
	_output_count(output_count), 
	_learn_rate(learn_rate), 
	_momentum(momentum),
	_momentum_rate(momentum_rate)
	{
		init();
	}
	~Net() {}

	void init()
	{
		weights_ih.clear();
		weights_ho.clear();
		wdelta_ih.clear();
		wdelta_ho.clear();
		unsigned int i, j;
		for (i=0; i<_input_count; i++)  
		{ 
			weights_ih.push_back(DVector()); 
			wdelta_ih.push_back(DVector());
			for (j=0; j<_hidden_count; j++)  
			{
				wdelta_ih[i].push_back(0.0f);
			}
		}
		for (i=0; i<_hidden_count; i++) 
		{ 
			weights_ho.push_back(DVector()); 
			wdelta_ho.push_back(DVector());
			for (j=0; j<_output_count; j++)  
			{
				wdelta_ho[i].push_back(0.0f);
			}
		}

		_current_learn_rate=_learn_rate;
		example_inputs.clear();
		example_outputs.clear();
	}

	void randomize()
	{
		unsigned int i, j;

		for (i=0; i<_input_count; i++)
		{
			weights_ih[i].clear();
			for (j=0; j<_hidden_count; j++) weights_ih[i].push_back(SMALL_RND);
		}

		for (i=0; i<_hidden_count; i++)
		{
			weights_ho[i].clear();
			for (j=0; j<_output_count; j++) weights_ho[i].push_back(SMALL_RND);
		}
	}

	void draw()
	{
		unsigned int i, j;
		cout << "NET..." << endl;

		for (i=0; i<_input_count; i++)
		{
			cout << "(";
			for (j=0; j<_hidden_count; j++) 
				cout << (j==0?"":",") << DFORMAT(weights_ih[i][j]);
			cout << ")";
		}
		cout << endl;
		for (i=0; i<_hidden_count; i++)
		{
			cout << "(";
			for (j=0; j<_output_count; j++) 
				cout << (j==0?"":",") << DFORMAT(weights_ho[i][j]);
			cout << ")";
		}
		cout << endl;
	}

	void forward_prop(DVector &x, DVector &h, DVector &o)
	{
		unsigned int i, j;
		double sum;
		h.clear();
		for (j=0; j<_hidden_count; j++)
		{
			sum=0;
			for (i=0; i<_input_count; i++)
				sum+=x[i]*weights_ih[i][j];
			h.push_back(sigmoid(sum));
		}
		o.clear();
		for (j=0; j<_output_count; j++)
		{
			sum=0;
			for (i=0; i<_hidden_count; i++)
				sum+=h[i]*weights_ho[i][j];
			o.push_back(sigmoid(sum));
		}
	}

	void train(DVector &inputs, DVector &t, double error_threshold=0.009f)
	{
		unsigned int k,j;
		double sum;
		DVector h, o, delta_o, delta_h;
		forward_prop(inputs, h, o);

		if (output_error(o, t)>error_threshold)
		{
			// BACKPROPAGATION
			for (k=0; k<_output_count; k++)
			{
				delta_o.push_back(o[k]*(1-o[k])*(t[k]-o[k]));
			}

			for (k=0; k<_hidden_count; k++)
			{
				sum=0;
				for (j=0; j<_output_count; j++)
					sum+=delta_o[j]*weights_ho[k][j];
				delta_h.push_back(h[k]*(1-h[k])*sum);
			}

			for (k=0; k<_input_count; k++)
			{
				for (j=0; j<_hidden_count; j++)
				{
					wdelta_ih[k][j]=_current_learn_rate*delta_h[j]*inputs[k]+_momentum*wdelta_ih[k][j];
					weights_ih[k][j]+=wdelta_ih[k][j];
				}
			}

			for (k=0; k<_hidden_count; k++)
			{
				for (j=0; j<_output_count; j++)
				{
					wdelta_ho[k][j]=_current_learn_rate*delta_o[j]*h[k]+_momentum*wdelta_ho[k][j];
					weights_ho[k][j]+=wdelta_ho[k][j];
				}
			}
		}
	}

	void add_example(DVector &inputs, DVector &outputs)
	{
		example_inputs.push_back(inputs);
		example_outputs.push_back(outputs);
	}

	double sigmoid(double x)
	{
		return 1.0f/(1.0f + exp(-x));
	}

	double process_training(int max_epochs, double error_threshold, double min_avg_error)
	{
		randomize();
		double error=2*min_avg_error, total_avg_error=0;
		for (int epoch=0; epoch<max_epochs && error>min_avg_error; epoch++)
		{
			error=0;
			for (unsigned int i=0; i<example_inputs.size(); i++)
			{
				DVector test_input=example_inputs[i];
				DVector test_output=example_outputs[i];
				train(test_input, test_output, error_threshold);
				error+=output_error(test_input, test_output);
			}
			total_avg_error+=error;
		}
		total_avg_error/=(double)max_epochs;
		return total_avg_error;
	}

	void query(DVector &input, DVector &output)
	{
		DVector hidden;
		forward_prop(input, hidden, output);
	}

private:
	VectorVector weights_ih;
	VectorVector weights_ho;
	VectorVector wdelta_ih;
	VectorVector wdelta_ho;
	VectorVector example_inputs;
	VectorVector example_outputs;
	unsigned int _input_count, _hidden_count, _output_count;
	double _learn_rate;
	double _current_learn_rate;
	double _momentum_rate;
	double _momentum;
private:
	double output_error(DVector outputs, DVector t)
	{
		double error=0;
		for (unsigned int i=0; i<outputs.size(); i++)
		{
			error+=(float)((float)outputs[i] - (float)t[i])*((float)outputs[i] - (float)t[i])/2;
		}
		return error;
	}
};

#endif
