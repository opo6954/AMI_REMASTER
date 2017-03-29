/*
 * Implementation of Error Back-Propagation Learning Algorithm
 *                 DEC. 5, 1990  Written in  C Language
 */

# include <stdio.h>
# include <math.h>
/*
# include "menu.h"
*/

/* define constants used throughout functions */

# define MXN_INAT    1610   /* max no. of input features   */
# define MXN_HDLR    1    /* max no. of hidden layers    */
# define MXN_UNIT    4   /* max no. of hidden units in a layer */
# define MXN_OUAT    4    /* max no. of output features  */
# define MXN_SAMP    10000   /* max no. of input samples    */

/* Database : declarations of variables */

float InPut[MXN_SAMP][MXN_INAT];        /* input pattern storage */
float Target[MXN_SAMP][MXN_OUAT];       /* target output storage */

float activation[MXN_HDLR+1][MXN_UNIT];    /* activation storage */   

float weight[MXN_HDLR+1][MXN_UNIT][MXN_INAT];  /* current weight */
float dweight[MXN_HDLR+1][MXN_UNIT][MXN_INAT];   /* delta weight */

float bias[MXN_HDLR+1][MXN_UNIT];                /* current bias */
float error[MXN_HDLR+1][MXN_UNIT];
float delta[MXN_HDLR+1][MXN_UNIT];
float dbias[MXN_HDLR+1][MXN_UNIT];                 /* delta bias */

float delta[MXN_HDLR+1][MXN_UNIT];                /* delta error */

float lrate=0.25, momentum=0.8;          /* learning rate and momentum */
float cur_err, pss, tss;        /* current error and total error */

int  epochno, no_input, no_layer, no_unit[MXN_HDLR+2];
char pt_name[MXN_SAMP][12];

main()
{
    char task_name[12];
    int  crow = 6, ccol = 15, choice;
    int  i, j, k = 0;

    printf("BP> Enter the task name : ");
    scanf("%s", task_name);
    
    param_define(task_name);
    define_bp_network();
    read_input_pattern(task_name);
    
    printf("learning order %d\n", k++);
    learning();
    write_neural_weight(task_name);
    
}

learning()
/*
 * learning procedure
 */
{   
    int   i, epoch = 0;
    float max_pss = 0.01, cur_tss = 100.0;

    while ((epoch < epochno) && (max_pss > pss) && (cur_tss > tss)) {
        cur_tss = 0.0;
        for (i = 0; i < no_input; i++) {      /* each pattern learning */
            compute_output(i); /* output compute by current weight */
            compute_error(i);     /* error compute by target value */
            change_weight(i);           /* weight adjust of output */
            if (cur_err > max_pss) max_pss = cur_err;
            cur_tss = cur_tss + cur_err;
        }
        epoch++;
	/* printf("cur_err = %d\n", cur_err); */
	printf("epoch num = %d, tss = %f\r", epoch, cur_tss);
    }
    printf("Epoch =  %d\t\t", epoch + 1);
    printf("PSS   =  %f\t", cur_err);
    printf("TSS   =  %f\t", cur_tss);
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
    no_unit[0] = MXN_INAT;
    no_unit[1] = MXN_UNIT;
    no_unit[2] = MXN_OUAT;
    //no_input   = MXN_SAMP;
    
	//lrate = 0.2;
    //momentum = 0.8;
	
	printf("epoch number = ");
	scanf("%d", &epochno);
    //epochno = 20000;
    pss = 0.001;
    tss = 0.1;
}

read_input_pattern(task_name)
char task_name[];
/* 
 * read input pattern from data file 
 */
{
    FILE *fpl;
    int  i, j, k;
    char fname[100];
   
    strcpy(fname, task_name);
    strcat(fname, ".dat");
    if ((fpl = fopen(fname, "r")) == NULL) {
	printf("Can't Open Input Data File %s\n", fname);
        exit(0);
    }
    fscanf(fpl, "%d", &no_input);
    i = 0;
    for (j = 0; j < no_input; j++) {
        fscanf(fpl, "%s", pt_name[j]);
        /* printf("\n%s", pt_name[j]); */ 
        for (k = 0; k < no_unit[0]; k++) {
            fscanf(fpl, "%f", &InPut[j][k]);
            /* printf(" %6.5f", InPut[j][k]); */ 
        }
        for (k = 0; k < no_unit[no_layer-1]; k++) {
            fscanf(fpl, "%f", &Target[j][k]);
            /* printf(" %1.0f", Target[j][k]); */ 
        }
    }
    fclose(fpl);
}

write_neural_weight(task_name)
char task_name[];
/*
 * learning weight write */
{
    FILE *fpw;
    int  i, j, k;
    char fname[20];

    /* strcat(fname, hiddenNode[no_unit[1]-4]);
    strcat(fname, epochData[epochno/1000 - 3]);
    */
    strcpy(fname, task_name);
    strcat(fname, ".weg");

    fpw = fopen(fname, "w");
    for (i = 0; i < (no_layer-1); i++) {
        for (j = 0; j < no_unit[i+1]; j++) {
            for (k = 0; k < no_unit[i]; k++)
                fprintf(fpw, "%f\n", weight[i][j][k]);
            fprintf(fpw, "%f\n", bias[i][j]);
        }
    }
    fprintf(fpw, "%d\n", epochno);
    //fprintf(fpw, "%d\n", no_unit[1]);
    fclose(fpw);
}



/*--------------------------------------------------------*
 *           Back-Propagation Learning Routine            *
 *--------------------------------------------------------*/

float random() 
/*
 * random number generator
 */
{
    return((float)rand()*3.051850948e-5);
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

define_bp_network()
/*
 * network define and random number assignment 
 */
{
    int  i, j, k;
    long time;   
 
    srand(&time);
    for (i = 0; i < (no_layer-1); i++) {
        for (j = 0; j < no_unit[i+1]; j++) {
            for (k = 0; k < no_unit[i]; k++)
                weight[i][j][k] = random();
            bias[i][j] = random();
        }
    }
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
                    sum_of_input += InPut[pe][k] * weight[i][j][k];
                else
                    sum_of_input += activation[i-1][k] * weight[i][j][k];
            }
            activation[i][j] = sigmoid(sum_of_input);
        }
    }
}

compute_error(l)
int l;
/*
 * error compute between output value and taget value
 */
{
    int i, j, k;
    
    i = 0;
    for (j = 0; j < no_unit[i+1]; j++)
        error[i][j] = 0.0; /* initialize */
    
    i = 1;
    cur_err = 0.0;
    for (j = 0; j < no_unit[i+1]; j++) { /* error between target and output */
        error[i][j] = Target[l][j] - activation[i][j]; /* delta value computing by GDR */
        delta[i][j] = error[i][j] * activation[i][j] * (1.0 - activation[i][j]);
        cur_err = cur_err + fabs(error[i][j]);
	/* 
	printf("error[i][j] = %f, activation[i][j] = %f, Target[i][j] = %f\n", error[i][j], activation[i][j], Target[i][j]);
*/
        for (k = 0; k < no_unit[i]; k++)/* error computing for hidden layer */
            error[i-1][k] += delta[i][j] * weight[i][j][k];
    }

    i = 0;
    for (j = 0; j < no_unit[i+1]; j++)
        delta[i][j] = error[i][j] * activation[i][j] * (1.0 - activation[i][j]);
}



change_weight(pe)
int pe;
{
    int i, j, k;
    
    for (i = 0; i < (no_layer-1); i++) {
        for (j = 0; j < no_unit[i+1]; j++) {
            for (k = 0; k < no_unit[i]; k++) {
                /* compute to change value of connection link */
                if (i == 0)
                    dweight[i][j][k] = lrate * delta[i][j] * InPut[pe][k] + momentum * dweight[i][j][k];
                else
                    dweight[i][j][k] = lrate * delta[i][j] * activation[i-1][k] + momentum * dweight[i][j][k];
                weight[i][j][k] += dweight[i][j][k];
            }
            /* same method to adjust the bias value */
            dbias[i][j] = lrate * delta[i][j] + momentum * dbias[i][j];
            bias[i][j] += dbias[i][j];
        }
    }
}

/*-------------------------- End of Program ----------------------------*/

