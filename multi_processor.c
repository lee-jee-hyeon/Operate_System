#pragma warning(disable: 4996)
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>

#define ROW 2048
#define COL 2048

float a[ROW][COL];
float b[ROW][COL];
float c[ROW][COL];

void change(float res[ROW][COL], float temp[ROW][COL])
{
	int i, j;

	for (i = 0; i < ROW; ++i)
		for (j = 0; j < COL; ++j)
			res[j][i] = temp[i][j];
}

int file_load(void *buffer, size_t size, const char* file)
{

	size_t res;
	FILE* fin = fopen(file, "rb");
	res = fread(buffer, size, 1, fin);
	fclose(fin);
	return res;

}
int file_save(const void* buffer, size_t size, const char* file)
{

	size_t res = (ROW * COL * sizeof(float));
	FILE* fout = fopen(file, "wb");
	res = fwrite(buffer, size, 1, fout);
	fclose(fout);


	return res;
}

void multiply(float a[ROW][COL], float b[ROW][COL], float res[ROW][COL])
{

	int a_row, b_col, i;
	float sum;

	for (a_row = 0; a_row<ROW; ++a_row)
	{
		for (b_col = 0; b_col<COL; ++b_col)
		{
			sum = 0;
			for (i = 0; i<ROW; ++i)
			{
				sum += a[a_row][i] * b[b_col][i];
			}
			res[a_row][b_col] = sum;
		}
	}

	
}


int main(int argc, char* argv[]) {



	int TOTALFORK;
	int status;
	int a_row=0;
	int runProcess2 = 1;
	//int runProcess2 = 0;
	int b_col, i;
	float sum;
	int runProcess = 0; //\BB\FD\BC\BA\C7\D1 \C7\C1\B7μ\BC\BD\BA \BC\F6

	TOTALFORK = atoi(argv[4]);
	clock_t t, second;

	pid_t pids[TOTALFORK], pid, pid_child;

	if (!file_load(c, sizeof(c), argv[2])) return -1; // file B Load
	if (!file_load(a, sizeof(a), argv[1])) return -1; // file A load

	t = clock();
	change(b, c);
	second = clock()-t;
	printf("load time : %g sec\n", (double)second/CLOCKS_PER_SEC);

	while(runProcess<TOTALFORK){

		pids[runProcess] = fork(); // do fork
		t = clock();

		
		if(pids[runProcess] < 0){
			printf("error \n");
			exit(0);
		}
		if(pids[runProcess] == 0){
			int j;
			int row_hal, row_hal2, r;
			
			if(ROW%TOTALFORK==0){
				row_hal = ROW/TOTALFORK;
				printf("child  %ld\n", (long)getpid());
				for (a_row = 0; a_row<row_hal; ++a_row) // a[a_row]\B0\A1 \B9ٲ\F1. 
				{
					//	if(runProcess2==a_row){

					for (b_col = 0; b_col<COL; ++b_col)
						{
							sum = 0;
								for (i = 0; i<ROW; ++i)
								{
									sum += a[a_row][i] * b[b_col][i];
								}	
								c[a_row][b_col] = sum;
							}

					}
			}
			if(ROW%TOTALFORK!=0){
				r = ROW%TOTALFORK;
				row_hal2 = ROW/TOTALFORK+1; // 0 ~ r -1
				row_hal = ROW/TOTALFORK; // r ~ TOTALFORK - 1
				
				printf("child  %ld\n", (long)getpid());
				if(runProcess <= (r-1)){
					for (a_row = 0; a_row<row_hal2; ++a_row) // a[a_row]\B0\A1 \B9ٲ\F1. 
					{
					//	if(runProcess2==a_row){

						for (b_col = 0; b_col<COL; ++b_col)
						{
							sum = 0;
								for (i = 0; i<ROW; ++i)
								{
									sum += a[a_row][i] * b[b_col][i];
								}	
								c[a_row][b_col] = sum;
							}

					}
				}
				if((runProcess >= r) && (runProcess<=(TOTALFORK-1))){
					for (a_row = 0; a_row<row_hal; ++a_row) // a[a_row]\B0\A1 \B9ٲ\F1. 
					{
					//	if(runProcess2==a_row){

						for (b_col = 0; b_col<COL; ++b_col)
							{
							sum = 0;
								for (i = 0; i<ROW; ++i)
								{
									sum += a[a_row][i] * b[b_col][i];
								}	
								c[a_row][b_col] = sum;
							}

					}
				}

			}
		second = clock()-t;
					printf("multiply succeed!\n");
					printf("%ld multiply time : %g sec\n",(long)getpid(), (double)second/CLOCKS_PER_SEC);

				exit(0);
			// child process to do
			
		}else{

		}
		runProcess++;
//		a_row++;
	}
	
			pid_child = wait(&status);
			printf("pid : %d exit", pid_child);
			if(0==(status & 0xff)){
				printf("child process is closed\n");
			}else{
				printf("child process is non-stationary closed\n");
			
			}
		

	sleep(5);
	t = clock();
	if (!file_save(c, sizeof(c), argv[3])) { // file C Save
		return -1;
	}
	second = clock()-t;
	printf("save succeed!\n");
	printf("save time : %g sec\n", (double)second/CLOCKS_PER_SEC);
	
	return 0;
}




