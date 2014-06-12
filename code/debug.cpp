#include "max.h"
#include "max_mem.h"
#include "debug.h"

#define FNAME "c:\\debug\\uvwframe.txt"

#ifdef DODEBUG

void debug(int a, int b){
	FILE *file = fopen(FNAME,"w");
	fprintf(file, "\n");
	fclose(file);
}

void debug(TSTR str){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%s",str);
	fclose(file);
}

void debug(int kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d\n",kprint);
	fclose(file); 
} 

void debugnl(int kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d,",kprint);
	fclose(file); 
} 


void debug(unsigned int kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%u\n",kprint);
	fclose(file);
} 

void debug(float kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\n",kprint);
	fclose(file);
} 

void debug(double kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\n",kprint);
	fclose(file);
} 

void debug(Point3 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\t%f\t%f\n",kprint.x,kprint.y,kprint.z);
	fclose(file);
} 

void debug(IPoint3 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d\t%d\t%d\n",kprint.x,kprint.y,kprint.z);
	fclose(file);
} 

void debug(Point2 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\t%f\n",kprint.x,kprint.y);
	fclose(file);
} 

void debug(IPoint2 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d\t%d\n",kprint.x,kprint.y);
	fclose(file);
} 

void debug(Matrix3 kprint){
	FILE *file = fopen(FNAME,"a+");
	Point3 aux;
	aux = kprint.GetRow(0);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	aux = kprint.GetRow(1);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	aux = kprint.GetRow(2);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	aux = kprint.GetRow(3);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	fclose(file);
} 

void debug(Matrix2 kprint){
	FILE *file = fopen(FNAME,"a+");
	Point2 aux;
	aux = kprint.GetRow(0);
	fprintf(file, "%f\t%f\n",aux.x,aux.y);
	aux = kprint.GetRow(1);
	fprintf(file, "%f\t%f\n",aux.x,aux.y);
	aux = kprint.GetRow(2);
	fprintf(file, "%f\t%f\n",aux.x,aux.y);
	fclose(file);
} 

void debug(Quat kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "x : %f\ty : %f\tz : %f\tw : %f\n",kprint.x,kprint.y, kprint.z, kprint.w);
	fclose(file);
} 

void debug(AngAxis kprint){
	FILE *file = fopen(FNAME,"a+");
	debug( kprint.axis );
	debug( kprint.angle);
	fclose(file);
} 

void debug(Color kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\t%f\t%f\n",kprint.r,kprint.g,kprint.b);
	fclose(file);
} 

void debug(AColor kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\t%f\t%f\t%f\n",kprint.r,kprint.g,kprint.b,kprint.a);
	fclose(file);
} 

void debugtime() {
	SYSTEMTIME date_time;
	GetSystemTime(&date_time);
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d:%d:%d:%d\n",int(date_time.wHour),int(date_time.wMinute),int(date_time.wSecond),int(date_time.wMilliseconds) );
	fclose(file);
}

#endif
