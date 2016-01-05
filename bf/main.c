#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX(a, b) (a>b)?a:b

#define MAX_TASK_NUM		101
#define MAX_PROCESSOR_NUM	100
#define MAX_DURATION		2000000

int **alpha;
int *b;
float *w;
int **UF;
int **alloc;

int task_num, processor_num, lcm_period, true_task_num;
int row_num, *rows;

int *period, *execute;
FILE *fin, *fout, *flog;

int *preemption, *migration, *event;

int getSign(int x)
{
	if(x>0)
		return 1;
	else if(x<0)
		return -1;
	return 0;
}

void calc_bound()
{
	int time[MAX_DURATION];
	int i, j, pre;
	for(i=0; i<lcm_period; i++)
		time[i]=0;
	for(i=0; i<task_num; i++)
		for(j=0; j<lcm_period/period[i]; j++)
			time[j*period[i]]=1;
	row_num=0;
	for(i=0; i<lcm_period; i++)
		row_num+=time[i];
	rows=(int *)malloc(sizeof(int)*row_num);
	b=(int *)malloc(sizeof(int)*(row_num+1));
	j=row_num-1;
	pre=lcm_period;
	for(i=lcm_period-1; i>=0; i--)
	{
		if(time[i]==1)
		{
			rows[j]=pre-i;
			j--;
			pre=i;
		}
	}
	b[0]=0;
	for(i=0; i<row_num; i++)
		b[i+1]=b[i]+rows[i];
}

void calc_UF()
{
	int i, j, k;
	int tmp;
	for(i=0; i<task_num; i++)
	{
		for(j=0; j<row_num+1; j++)
		{
			UF[i][j]=0;
			for(k=1; k<row_num-j; k++)
			{
				if(alpha[i][j+k]==-1)
				{
					tmp=(b[j+k]*execute[i])/period[i];
					UF[i][j]=(1+tmp)*period[i]-execute[i]*b[j+k];
		//			UF[i][j]=(1.0-(((float)b[j+k])*w[i]-(float)((int)(b[j+k]*w[i]))))/w[i];
					break;
				}
			}
		}
	}
}

void calc_alpha()
{
	int i, j, tmp;
	for(i=0; i<row_num; i++)
		for(j=0; j<task_num; j++)
		{
			tmp=b[i]*execute[j]/period[j]+(b[i+1]-b[i]);
			alpha[j][i]=getSign(b[i+1]*execute[j]-tmp*period[j]);
//			alpha[j][i]=getSign(b[i+1]*w[j]-(float)((int)(b[i]*w[j]))-(b[i+1]-b[i]));
		}
}


int Compare(int i, int j, int k)
{
	int s=1;
	int xi, xj;
	while((alpha[i][k+s]==1)&&(alpha[j][k+s]==1))
		s++;
	if(alpha[i][k+s]>alpha[j][k+s])
		return 1;
	else if(alpha[i][k+s]<alpha[j][k+s])
		return 0;
	else if((alpha[i][k+s]==0)&&(alpha[j][k+s]==0))
		return 1;
	else
	{
		xi=UF[i][k]*execute[j];
		xj=UF[j][k]*execute[i];
		if(xi>xj)
			return 0;
		else
			return 1;
	}
}

void TaskSelection(int *selected, int ru, int k, int el)
{
	int i, j, tmp;
	for(i=0; i<ru; i++)
		for(j=i+1; j<el; j++)
		{
			if(Compare(selected[i], selected[j], k)==0)
			{
				tmp=selected[i];
				selected[i]=selected[j];
				selected[j]=tmp;
			}
		}
}

void Bfair(int *RWmP, int k)
{
	int m[MAX_TASK_NUM];
	int PWmP[MAX_TASK_NUM];
	int i, j, RU, summ=0, el=0;
	int selected[MAX_TASK_NUM], o[MAX_TASK_NUM];
	int x, y, count, tmp;
	for(i=0; i<task_num; i++)
	{
		tmp=RWmP[i]+(rows[k]*execute[i]);
		tmp=tmp/period[i];
		m[i]=MAX(0, tmp);
		PWmP[i]=RWmP[i]+rows[k]*execute[i]-m[i]*period[i];
		o[i]=0;
		summ+=m[i];
	}
	RU=processor_num*rows[k]-summ;
	if(RU>0)
	{
		for(i=0; i<task_num; i++)
		{
			if((PWmP[i]>0)&&(m[i]<rows[k]))
			{
				selected[el]=i;
				el++;
			}
		}
		TaskSelection(selected, RU, k, el);
		for(i=0; i<RU; i++)
			o[selected[i]]=1;
	}
	for(i=0; i<task_num; i++)
		m[i]=m[i]+o[i];
	count=0;
	for(i=0; i<task_num; i++)
	{
		for(j=0; j<m[i]; j++)
		{
			x=count%rows[k]+b[k];
			y=count/rows[k];
			alloc[y][x]=i;
			count++;
		}
	}
	for(i=0; i<task_num; i++)
		RWmP[i]=PWmP[i]-o[i]*period[i];
}

int checkSchedule()
{
	int i, j, k, m, count;
	for(i=0; i<task_num; i++)
	{
		for(j=0; j<lcm_period/period[i]; j++)
		{
			count=0;
			for(k=0; k<period[i]; k++)
			{
				for(m=0; m<processor_num; m++)
				{
					if(alloc[m][j*period[i]+k]==i)
						count++;
				}
			}
			if(count!=execute[i])
			{
				printf("task%d, job%d, from %d to %d.\n", i, j, j*period[i], (j+1)*period[i]-1);
				return -1;
			}
		}
	}
	for(i=0; i<lcm_period; i++)
	{
		for(j=0; j<processor_num-1; j++)
			for(k=j+1; k<processor_num; k++)
				if(alloc[j][i]==alloc[k][i])
				{
					printf("time%d----->%d and %d.\n", i, alloc[j][i], alloc[k][i]);
					return -1;
				}
	}
	return 1;
}

void initialize()
{
	int i;
	alpha=(int **)malloc(sizeof(int *)*task_num);
	for(i=0; i<task_num; i++)
		alpha[i]=(int *)malloc(sizeof(int)*row_num);
	UF=(int **)malloc(sizeof(int *)*task_num);
	for(i=0; i<task_num; i++)
		UF[i]=(int *)malloc(sizeof(int)*(row_num+1));

	alloc=(int **)malloc(sizeof(int *)*processor_num);
	for(i=0; i<processor_num; i++)
		alloc[i]=(int *)malloc(sizeof(int)*lcm_period);

	preemption=(int*)malloc(sizeof(int)*true_task_num);
	migration=(int*)malloc(sizeof(int)*true_task_num);
	event=(int*)malloc(sizeof(int)*true_task_num);
}

void remains()
{
	int i;

	free(period);
	free(execute);
	free(b);
	free(rows);

	for(i=0; i<task_num; i++)
	{
		free(alpha[i]);
		free(UF[i]);
	}

	for(i=0; i<processor_num; i++)
		free(alloc[i]);

	free(alpha);
	free(UF);
	free(alloc);

	free(preemption);
	free(migration);
	free(event);
}

void countPreemption();
void countMigration();
void countEvent();

int algo()
{
	int i, j;
	int RWmP[MAX_TASK_NUM];
	fscanf(fin, "%d%d%d%d", &true_task_num, &task_num, &processor_num, &lcm_period);

	period=(int *)malloc(sizeof(int)*task_num);
	execute=(int *)malloc(sizeof(int)*task_num);
	w=(float *)malloc(sizeof(float)*task_num);
	for(i=0; i<task_num; i++)
		fscanf(fin, "%d", &period[i]);
	for(i=0; i<task_num; i++)
		fscanf(fin, "%d", &execute[i]);

	for(i=0; i<task_num; i++)
	{
		w[i]=((float)execute[i])/((float)period[i]);
		RWmP[i]=0;
	}

	calc_bound();
	initialize();
	calc_alpha();
	calc_UF();
	
	for(i=0; i<processor_num; i++)
		for(j=0; j<lcm_period; j++)
			alloc[i][j]=-1;
	
	for(i=0; i<row_num; i++)
		Bfair(RWmP, i);

	for(i=0; i<processor_num; i++)
	{
		for(j=0; j<lcm_period; j++)
		{
			fprintf(fout, "%d ", alloc[i][j]);
		}
		fprintf(fout, "\n");
	}
	if(checkSchedule()==-1)
	{
		printf("!!!Schedule error.\n");
	}
	else
		printf("successfully scheduling.\n");

	countPreemption();
	countMigration();
	countEvent();

	fprintf(flog, "preemption:\n");
	for(i=0; i<true_task_num; i++)
	{
		double ppj=((double)preemption[i])/((double)(lcm_period/period[i]));
		fprintf(flog, "%lf ", ppj);
	}
	fprintf(flog, "\n");
	fprintf(flog, "migration:\n");
	for(i=0; i<true_task_num; i++)
	{
		double mpj=((double)migration[i])/((double)(lcm_period/period[i]));
		fprintf(flog, "%lf ", mpj);
	}
	fprintf(flog, "\n");
	fprintf(flog, "event:\n");
	for(i=0; i<true_task_num; i++)
	{
		double epj=((double)event[i])/((double)(lcm_period/period[i]));
		fprintf(flog, "%lf ", epj);
	}
	fprintf(flog, "\n");

	remains();
	return 0;
}

void countPreemption() {
	int i, j;
	int k, m;
	int tnum=true_task_num;
	int pnum=processor_num;
	for(i=0; i<tnum; i++)
	{
		int count=0;
		for(j=0; j<lcm_period/period[i]; j++) 
		{
			int executed=0;
			for(m=0; m<period[i]; m++)
				for(k=0; k<pnum; k++)
				{
					if(alloc[k][j*period[i]+m]==i) {
						executed++;
						if(executed==execute[i])
							continue;
						if(alloc[k][j*period[i]+m+1]!=i)
							count++;
					}
				}
		}
		preemption[i]=count;
	}
}

void countMigration() {
	int i, j;
	int k, m;
	int tnum=true_task_num;
	int pnum=processor_num;
	for(i=0; i<tnum; i++)
	{
		int count=0;
		int loc=-1;
		for(j=0; j<lcm_period; j++)
		{
			for(k=0; k<pnum; k++)
			{
				if(alloc[k][j]==i) {
					if(loc==-1)
						loc=k;
					else {
						if(loc!=k)
							count++;
						loc=k;
					}
				}
			}
		}
		migration[i]=count;
	}
}

void countEvent() {
	int i, j;
	int k, m;

	int jstart, loc;
	int tnum=true_task_num;
	int pnum=processor_num;

	for(i=0; i<tnum; i++) {
		int count=0;
		int loc=-1;
		for(j=0; j<lcm_period/period[i]; j++) {
			jstart=j*period[i];
			loc=-1;
			for(k=0; k<period[i]; k++) 
				for(m=0; m<pnum; m++) {
					if(alloc[m][jstart+k]==i) {
						if(loc==-1)
							loc=m;
						else {
							if(loc!=m)
								count++;
							loc=m;
						}
					}
				}
		}
		event[i]=preemption[i]+migration[i]-count;
	}
}

int main(int argc, char *argv[])
{
	int i, taskset_num;

	if(argc<4) {
		printf("usage: ./bfair <taskset_filename> <output_filename> <logfile_name>\n");
		return 0;
	}

	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	flog=fopen(argv[3], "w");

	printf("%s\t", argv[1]);

	algo();

	fflush(fout);
	fflush(flog);
	fclose(fin);
	fclose(fout);
	fclose(flog);
	return 0;
}
