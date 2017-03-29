# include <stdio.h>
# include <math.h>
/* define constants used throughout functions */

# define MXN_INAT    16   /* max no. of input features   */
# define MXN_HDLR    1   /* max no. of hidden layers    */
# define MXN_UNIT   16   /* max no. of units in a layer */
# define MXN_OUAT    16   /* max no. of output features  */

/* Database : declarations of variables */

float InPut[MXN_INAT];        /* input pattern storage */
float Target[MXN_OUAT];       /* target output storage */

float activation[MXN_HDLR+1][MXN_UNIT];    /* activation storage */   

float weight[MXN_HDLR+1][MXN_UNIT][MXN_INAT];  /* current weight */

float bias[MXN_HDLR+1][MXN_UNIT];                /* current bias */
float error[MXN_HDLR+1][MXN_UNIT];              /* current error */
float delta[MXN_HDLR+1][MXN_UNIT];                /* delta error */

int  epochno, no_input, no_layer, no_unit[MXN_HDLR+2];

recognition(task_name)
char task_name[];
{   
    read_input_pattern(task_name)
    read_neural_weight(task_name);
}

/*--------------------------------------------------------*
 *  Parameter, Input Pattern and Weight File I/O Routine  *
 *--------------------------------------------------------*/

param_define(task_name)
char task_name[];
/*
 * network define parameter read
 */
{   
    no_layer = 3;
    no_unit[0] = 8;
    no_unit[1] = 4;
    no_unit[2] = 2;
}

read_input_pattern(task_name)
char task_name[];
/* 
 * read input pattern from data file 
 */
{
    fscanf(fpl, "%s", pt_name[j]);
    fscanf(fpl, "%f", &InPut[j][k]);
    compute_output(pe)
}

read_neural_weight(task_name)
char task_name[];
/*
 * read  learning weight
 */
{
    FILE *fpr;
    int  i, j, k;
    char fname[20];

    /* strcpy(fname, task_name);
    strcat(fname, hiddenNode[no_unit[1]-4]);
    strcat(fname, epochData[epochno/1000 - 3]); */

    strcat(fname, "face.weg");
    if ((fpr = fopen(fname, "r")) == NULL) {
	printf("Can't Open Learning Weight File %s\n", fname);
        exit(0);
    }
    for (i = 0; i < (no_layer-1); i++) {
        for (j = 0; j < no_unit[i+1]; j++) {
            for (k = 0; k < no_unit[i]; k++)
                fscanf(fpr, "%f", &weight[i][j][k]);
            fscanf(fpr, "%f", &bias[i][j]);
        }
    }
    fscanf(fpr, "%d", epochno);
    fclose(fpr);

}


float sigmoid(x)
float x;
/*
 * logistic activation function
 */
{
    double exp();

    if (x > 11.5129) return(0.99999);
    else
        if (x < -11.5129) return(0.00001);
    return(1.0 / (1.0 + (float) exp( (double) ((-1.0) * x))));
}

compute_output(pe)
int pe;
/*
 * net input, hidden data summation and net output generation
 */
{
    int i, j, k;
    float sum_of_input;
    
    for (i = 0; i < (no_layer-1); i++) {
        for (j = 0; j < no_unit[i+1]; j++) {
            sum_of_input = bias[i][j];
            for (k = 0; k < no_unit[i]; k++) { /* input summation */
                if (i == 0)
                    sum_of_input += InPut[k] * weight[i][j][k];
                else
                    sum_of_input += activation[i-1][k] * weight[i][j][k];
            }
            activation[i][j] = sigmoid(sum_of_input);
        }
    }
}

/*-------------------------- End of Program ----------------------------*/

