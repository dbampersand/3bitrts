#pragma once

#include "allegro5/allegro.h"
#include "rect.h"
#include "point.h"
#define DIST_DELTA 1

#define _MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define _MAX(X,Y) ((X) > (Y) ? (X) : (Y))

typedef struct GameObject GameObject;

#define MOUSECURSORSIZE 5

bool IsNear(float f, float f2, float epsilon);
int GetIndex(int h, int x, int y);
Point IndexToPoint(int h, int index);
ALLEGRO_MOUSE_STATE GetMouseClamped();
char* readFile(const char* filename);
bool MoveTo(float* x, float* y, float toX, float toY, float speed, float delta);
void MoveAngle(float* x, float* y, float dx, float dy, float speed, float delta);   
bool CircleInRect(float cx, float cy, float rad, Rect r);
void Normalize(float* x, float* y);
double RandRange(double min, double max);
int RandRangeI(int min, int max);
float dot(float x1, float y1, float x2, float y2);
float dist(float x1, float y1, float x2, float y2);
bool LineIntersectsObj(GameObject* g, float xLine, float yLine, float x2Line, float y2Line);
bool GetLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float* out_x, float* out_y);
void ClampToRadius(float* x, float* y, float cx, float cy, float radius);
bool PointInCircle(float x, float y, float cx, float cy, float radius);
void RotatePoint(int* x, int* y, int cx, int cy, float radius);
float cotan(float i);   
float Normalise(float v, float start, float end);
int sign(float j);
float DegToRad(float deg);
float RadToDeg(float rad);
float magnitude(float* x, float* y);
void RotatePointF(float* x, float* y, float cx, float cy, float angle);
int NumDigits(int i);
float clamp(float f, float min, float max);
float Towards(float f, float to, float maxDist);
float PointsToAngleRad(float x1, float y1, float x2, float y2);
float EaseInOutCubic(float f);
