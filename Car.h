//---------------------------------------------------------------------------

#ifndef CarH
#define CarH

#include <vcl.h>
#include <math.h>
#include "net.h"

#define MAX_EXAMPLES    10000

// angles in degree
#define NORMAL_ANGLE    30
#define STRONG_ANGLE    60
#define SPEED_MAX       10


#define DIR_SL  0
#define DIR_LL  1
#define DIR_FW  2
#define DIR_RR  3
#define DIR_SR  4

class Car
{
public:   
      Car(double dirX, double dirY, double posX, double posY) :
        _dirX(dirX), _dirY(dirY),
        _posX(posX), _posY(posY),
        _trained(false)
      {
        normalize();
        _speed=1;
        net=new Net(20, 3, 5);
        examples_count=0;
      }

      ~Car()
      {
        resetMind();
        delete net;
      }

      void accellerate()
      {
        if (_speed<SPEED_MAX) _speed++;
      }

      void decrease()
      {
        if (_speed>1) _speed--;
      }

      void move()
      {
        _posX+=_dirX*_speed;
        _posY+=_dirY*_speed;
      }

      void left()
      {
        rotate(NORMAL_ANGLE);
      }

      void right()
      {
        rotate(-NORMAL_ANGLE);
      }

      void strong_left()
      {
        rotate(STRONG_ANGLE);
      }

      void strong_right()
      {
        rotate(-STRONG_ANGLE);
      }

      void camera(double *data, int size)
      {
        if (_trained)
        {
          DVector *input,
                  *output;// not used
          DVector result;
          calc_road_example( data, size, &input, &output);
          net->query(*input, result);
          delete input;
          delete output;
          double max=0;
          int index=0;
          for (unsigned int i=0; i<result.size(); i++)
          {
                  if (max<result[i])
                  {
                          max=result[i];
                          index=i;
                  }
          }
          switch (index)
          {
                  case 4: /*strong left */
                          strong_left();
                  break;
                  case 3: /* left */
                          left();
                  break;
                  case 2: /* forward */ break;
                  case 1: /* right */
                          right();
                  break;
                  case 0: /*strong right */
                          strong_right();
                  break;
          }
        }
      }

      void add_example(double *data, int size, int dir)
      {
        bool isUsed=false;
        for (int i=0; i<examples_count; i++)
        {
                bool isEqual=true;
                for (int j=0; j<20; j++)
                {
                        if (data[j] != used_numbers[i][j])
                                isEqual=false;
                }
                if (isEqual)
                {
                        return ;
                }
        }
        calc_road_example(data, size, &examples_input[examples_count], &examples_output[examples_count], dir);
        net->add_example(*examples_input[examples_count], *examples_output[examples_count]);
        for (int j=0; j<20; j++)
                used_numbers[examples_count][j]=data[j];
        examples_count++;
      }

      void save(TFileStream &stream)
      {
        stream.Write(&_posX, sizeof(double));
        stream.Write(&_posY, sizeof(double));
        stream.Write(&_dirX, sizeof(double));
        stream.Write(&_dirY, sizeof(double));
        stream.Write(&_speed, sizeof(double));
      }

      void load(TFileStream &stream)
      {
        stream.Read(&_posX, sizeof(double));
        stream.Read(&_posY, sizeof(double));
        stream.Read(&_dirX, sizeof(double));
        stream.Read(&_dirY, sizeof(double));
        stream.Read(&_speed, sizeof(double));
      }
      
      void load_examples(TFileStream &stream)
      {
        net->init();
        for (int i=0; i<examples_count; i++)
        {
                delete examples_input[i];
                delete examples_output[i];
        }
        stream.Read(&examples_count, sizeof(int));
        for (int i=0; i<examples_count; i++)
        {
              unsigned int size;
              stream.Read(&size, sizeof(unsigned int));
              examples_input[i] = new DVector();
              for (unsigned int j=0; j<size; j++)
              {
                double value;
                stream.Read(&value, sizeof(double));
                examples_input[i]->push_back(value);
              }
              stream.Read(&size, sizeof(unsigned int));
              examples_output[i] = new DVector();
              for (unsigned int j=0; j<size; j++)
              {
                double value;
                stream.Read(&value, sizeof(double));
                examples_output[i]->push_back(value);
              }
              net->add_example(*(examples_input[i]), *(examples_output[i]));
        }
        for (int i=0; i<examples_count; i++)
        {
              stream.Read(&used_numbers[i], sizeof(unsigned int));
        }
      }

      void save_examples(TFileStream &stream)
      {
        stream.Write(&examples_count, sizeof(int));
        for (int i=0; i<examples_count; i++)
        {
              unsigned int size=examples_input[i]->size();
              stream.Write(&size, sizeof(unsigned int));
              for (unsigned int j=0; j<size; j++)
              {
                      double value = (*examples_input[i])[j];
                      stream.Write(&value, sizeof(double));
              }
              size=examples_output[i]->size();
              stream.Write(&size, sizeof(unsigned int));
              for (unsigned int j=0; j<size; j++)
              {
                      double value = (*examples_output[i])[j];
                      stream.Write(&value, sizeof(double));
              }
        }
        for (int i=0; i<examples_count; i++)
        {
              stream.Write(&used_numbers[i], sizeof(unsigned int));
        }
      }

      void train()
      {
        net->init();
        for (int i=0; i<examples_count; i++)
                net->add_example(*examples_input[i], *examples_output[i]);

 
        DVector* input, *output;
        double *data=(double*)malloc(20*sizeof(double));
        for (int i=0; i<20; i++) data[i]=0.0f;
        calc_road_example(data, 20, &input, &output);
        net->add_example(*input, *output);

        net->process_training(1000, 0.01, 0.1);
        delete input;
        delete output;
        _trained=true;
      }

      void autotrain()
      { /*
              static char* directions[]={
                      "strong left", "left", "forward", "right", "strong right"
              };
              net->init();

              DVector *inputs[21], *outputs[21];
              unsigned int bit=1;

              for (int i=0; i<20; i++)
              {
                      calc_road_example(bit, &inputs[i], &outputs[i]);
                      net->add_example(*inputs[i], *outputs[i]);
                      bit<<=1;
              }
              calc_road_example(0, &inputs[20], &outputs[20]);
              net->add_example(*inputs[20], *outputs[20]);

              net->process_training(1000, 0.01, 0.1);
              _trained=true;
              for (int i=0; i<21; i++)
              {
                      delete inputs[i];
                      delete outputs[i];
              }
      */
      }

      void resetMind()
      {
        net->init();
        net->randomize();
        for (int i=0; i<examples_count; i++)
        {
                delete examples_input[i];
                delete examples_output[i];
        }
        examples_count=0; 
        _trained=false;
      }

      double _dirX, _dirY;
      double _posX, _posY;
      double _speed;
      bool   _trained;
      DVector* examples_input[MAX_EXAMPLES];
      DVector* examples_output[MAX_EXAMPLES];
      double   used_numbers[MAX_EXAMPLES][20];
      int      examples_count;
private:
        void normalize()
        {
            double newX=_dirX/sqrt(_dirX*_dirX+_dirY*_dirY);
            double newY=_dirY/sqrt(_dirX*_dirX+_dirY*_dirY);
            _dirX=newX;
            _dirY=newY;
        }

        void rotate(int rot)
        {
          double angle=atan2(_dirY, _dirX);
          angle+=(rot*M_PI/(double)180);
          _dirY=sin(angle);
          _dirX=cos(angle);
        }

        void calc_road_example(double *road, int size, DVector** input, DVector** output, int dir=-1)
        {
                /*
                        00 0000 00000000 0000 00
                        SR   R     FW    L    SL
                */
                unsigned int bit=1;
                *input=new DVector();
                *output=new DVector();
                for (int i=0; i<20; i++)
                {
                    (*input)->push_back(road[i]);
                }

                if (dir == -1)
                {
                    double total_avg=0;
                    for (int i=0; i<size; i++)
                    {
                        total_avg+=road[i];
                    }

                    double avg;
                    // strong left
                    avg=0;
                    for (int i=0; i<2; i++)
                        avg+=road[i];
                    (*output)->push_back(avg/2);

                    // left
                    avg=0;
                    for (int i=2; i<=5; i++)
                        avg+=road[i];
                    (*output)->push_back(avg/4);

                    // forward
                    avg=0;
                    for (int i=6; i<=13; i++)
                        avg+=road[i];

                    if (total_avg>0)
                        (*output)->push_back(avg/8);
                    else
                        (*output)->push_back(1);

                    // right
                    avg=0;
                    for (int i=14; i<=17; i++)
                        avg+=road[i];
                    (*output)->push_back(avg/4);

                    // strong right
                    avg=0;
                    for (int i=18; i<20; i++)
                        avg+=road[i];
                    (*output)->push_back(avg/2);
                }
                else
                {
                    // strong left
                    if (dir==DIR_SL)
                                    (*output)->push_back(1);
                    else
                                    (*output)->push_back(0);

                    // left
                    if (dir==DIR_LL)
                                    (*output)->push_back(1);
                    else
                                    (*output)->push_back(0);

                    // forward
                    if (dir==DIR_FW)
                                    (*output)->push_back(1);
                    else
                                    (*output)->push_back(0);

                    // right
                    if (dir==DIR_RR)
                                    (*output)->push_back(1);
                    else
                                    (*output)->push_back(0);

                    // strong right
                    if (dir==DIR_SR)
                                    (*output)->push_back(1);
                    else
                                    (*output)->push_back(0);
                }
        }

        Net *net;
};

//---------------------------------------------------------------------------
#endif
