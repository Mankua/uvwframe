#ifndef DEBUG_H
#define DEBUG_H

//#define DODEBUG

#ifdef DODEBUG

void debug(int a, int b);
void debug(TSTR str);
void debug(int kprint);
void debugnl(int kprint);
void debug(unsigned int kprint);
void debug(float kprint);
void debug(double kprint);
void debug(Point3 kprint);
void debug(IPoint3 kprint);
void debug(Matrix3 kprint);
void debug(Matrix2 kprint);
void debug(Point2 kprint);
void debug(IPoint2 kprint);
void debug(Quat kpring);
void debug(AngAxis kprint);
void debug(Color kprint);
void debug(AColor kprint);
void debugtime();

#else

#define debug

#endif

#endif
