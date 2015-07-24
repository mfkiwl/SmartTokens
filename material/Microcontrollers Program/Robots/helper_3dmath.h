// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class, 3D math helper
// 6/5/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-06-05 - add 3D math helper file to DMP6 example sketch

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _HELPER_3DMATH_H_
#define _HELPER_3DMATH_H_

#include <jendefs.h>
#include <AppHardwareApi.h>
#include "config.h"
#include <Math.h>

typedef struct{
        float w;
        float x;
        float y;
        float z;
}Quaternion;

//Quaternion() {
//	w = 1.0f;
//	x = 0.0f;
//	y = 0.0f;
//	z = 0.0f;
//}

//Quaternion(float nw, float nx, float ny, float nz) {
//	w = nw;
//	x = nx;
//	y = ny;
//	z = nz;
//}

Quaternion getProduct(Quaternion* q1, Quaternion* q2)
{
	// Quaternion multiplication is defined by:
	//     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
	//     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
	//     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
	//     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
	Quaternion q;
	q.w = q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
	q.x = q1->w*q2->x + q1->x*q2->w + q1->y*q2->z - q1->z*q2->y;
	q.y = q1->w*q2->y - q1->x*q2->z + q1->y*q2->w + q1->z*q2->x;
	q.z = q1->w*q2->z + q1->x*q2->y - q1->y*q2->x + q1->z*q2->w;

	return q;
}

Quaternion getConjugate(Quaternion* quat)
{
	Quaternion q;
	q.w = quat->w;
	q.x = -quat->x;
	q.y = -quat->y;
	q.z = -quat->z;

	return q;
}

float getQuaternionMagnitude(Quaternion* quat) {
	return sqrt(quat->w*quat->w + quat->x*quat->x + quat->y*quat->y + quat->z*quat->z);
}

void normalizeQuaternion(Quaternion* quat) {
	float m = getQuaternionMagnitude(quat);
	quat->w /= m;
	quat->x /= m;
	quat->y /= m;
	quat->z /= m;
}

Quaternion getNormalizedQuaternion(Quaternion* quat) {
	Quaternion q;
	q.w = quat->w;
	q.x = quat->x;
	q.y = quat->y;
	q.z = quat->z;
	normalizeQuaternion(&q);
	return q;
}

//
typedef struct{
        int16 x;
        int16 y;
        int16 z;
}VectorInt16;

//VectorInt16() {
//	x = 0;
//	y = 0;
//	z = 0;
//}
//
//VectorInt16(int16_t nx, int16_t ny, int16_t nz) {
//	x = nx;
//	y = ny;
//	z = nz;
//}

float getVectorInt16Magnitude(VectorInt16* vect) {
	return sqrt(vect->x*vect->x + vect->y*vect->y + vect->z*vect->z);
}

void normalizeVectorInt16(VectorInt16* vect) {
	float m = getVectorInt16Magnitude(vect);
	vect->x /= m;
	vect->y /= m;
	vect->z /= m;
}

VectorInt16 getNormalizedVectorInt16(VectorInt16* vect) {
	VectorInt16 v;
	v.x = vect->x;
	v.y = vect->y;
	v.z = vect->z;
	normalizeVectorInt16(&v);
	return v;
}

void rotateVectorInt16(Quaternion *q) {
	// http://www.cprogramming.com/tutorial/3d/quaternions.html
	// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
	// http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
	// ^ or: http://webcache.googleusercontent.com/search?q=cache:xgJAp3bDNhQJ:content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation&hl=en&gl=us&strip=1

	// P_out = q * P_in * conj(q)
	// - P_out is the output vector
	// - q is the orientation quaternion
	// - P_in is the input vector (a*aReal)
	// - conj(q) is the conjugate of the orientation quaternion (q=[w,x,y,z], q*=[w,-x,-y,-z])
	Quaternion p(0, x, y, z);

	// quaternion multiplication: q * p, stored back in p
	p = q -> getProduct(p);

	// quaternion multiplication: p * conj(q), stored back in p
	p = p.getProduct(q -> getConjugate());

	// p quaternion is now [0, x', y', z']
	x = p.x;
	y = p.y;
	z = p.z;
}

VectorInt16 getRotatedVectorInt16(Quaternion *q) {

	VectorInt16 r(x, y, z);
	r.rotate(q);
	return r;
}


typedef struct{
        float x;
        float y;
        float z;
}VectorFloat;

//        VectorFloat() {
//            x = 0;
//            y = 0;
//            z = 0;
//        }

//        VectorFloat(float nx, float ny, float nz) {
//            x = nx;
//            y = ny;
//            z = nz;
//        }

float getVectorFloatMagnitude(VectorFloat* vect) {
	return sqrt(vect->x*vect->x + vect->y*vect->y + vect->z*vect->z);
}

void normalizeVectorFloat(VectorFloat* vect) {
	float m = getVectorFloatMagnitude(vect);
	vect->x /= m;
	vect->y /= m;
	vect->z /= m;
}

VectorFloat getNormalizedVectorFloat(VectorFloat* vect) {
	VectorFloat v;
	v.x = vect->x;
	v.y = vect->y;
	v.z = vect->z;

	normalizeVectorFloat(&v);
	return v;
}

void rotateVectorFloat(Quaternion *q) {
	Quaternion p;
	p.w=0.0f;
	p.x //(0, x, y, z);

	// quaternion multiplication: q * p, stored back in p
	p = q -> getProduct(p);

	// quaternion multiplication: p * conj(q), stored back in p
	p = p.getProduct(q -> getConjugate());

	// p quaternion is now [0, x', y', z']
	x = p.x;
	y = p.y;
	z = p.z;
}

VectorFloat getRotatedVectorFloat(Quaternion *q) {
	VectorFloat r(x, y, z);
	r.rotate(q);
	return r;
}

#endif /* _HELPER_3DMATH_H_ */
