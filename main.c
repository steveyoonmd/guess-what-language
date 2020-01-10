/*
Copyright (c) 2020 Steve Yoon at Market Designers

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
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define _if_not_allocated_then_exit(ptr)\
	if (ptr == NULL)\
	{\
		fprintf(stderr, "memory allocation failed\n");\
		exit(EXIT_FAILURE);\
	}

#define _free(ptr)\
	if (ptr != NULL)\
	{\
		free(ptr);\
		ptr = NULL;\
	}

typedef unsigned char *String;

#define String_init(self, iLength)\
	if (self == NULL)\
	{\
		self = calloc(iLength + 1, sizeof(unsigned char));\
		_if_not_allocated_then_exit(self);\
	}

#define String_free(self) _free(self)

String String_sDuplicate(String self)
{
	String sOutput = NULL;
	String_init(sOutput, strlen(self));

	strncpy(sOutput, self, strlen(self));
	return sOutput;
}

#define Array(type) type*

#define Array_init(self)\
	if (self == NULL)\
	{\
		size_t *p = calloc(2, sizeof(size_t));\
		_if_not_allocated_then_exit(p);\
		p[0] = 0;\
		p[1] = 0;\
		self = (void *)&p[2];\
	}

#define Array_free(self)\
	if (self != NULL)\
	{\
		size_t *p = (size_t *)self - 2;\
		free(p);\
		self = NULL;\
	}

#define Array_iSize(self) *((size_t *)self - 1)

#define Array_push(self, value)\
	if (self != NULL)\
	{\
		size_t *p = (size_t *)self - 2;\
		if (p[0] == 0)\
		{\
			p[0] = 1;\
			p = realloc(p, 2 * sizeof(size_t) + p[0] * sizeof(value));\
			_if_not_allocated_then_exit(p);\
			self = (void *)&p[2];\
		}\
		if (p[0] == p[1])\
		{\
			p[0] *= 2;\
			p = realloc(p, 2 * sizeof(size_t) + p[0] * sizeof(value));\
			_if_not_allocated_then_exit(p);\
			self = (void *)&p[2];\
		}\
		self[p[1]] = value;\
		p[1] += 1;\
	}

#define AutoFree_initialize(type, value)\
	type _value = value

#define _return(value)\
	_value = value;\
	goto AutoFree_finalize

typedef struct Point
{
	int x;
	int y;

} Point;

void Point_init(Point *self)
{
	self->x = 0;
	self->y = 0;
}

int main(int iArgc, char* aArgv[])
{
	String sNew = NULL;
	Array(int64_t) aInt64s = NULL;
	Array(String) aStrings = NULL;
	Array(Point) aPoints = NULL;

	AutoFree_initialize(int, EXIT_SUCCESS);

	String sSrc = "abcd";
	sNew = String_sDuplicate(sSrc);
	printf("%s\n", sNew);

	Point pt;
	Point_init(&pt);
	printf("%d\n", pt.x);

	int64_t iFirst = 9223372036854775807;
	int64_t iSecond = 9223372036854775807;

	Array_init(aInt64s);
	Array_push(aInt64s, iFirst);
	Array_push(aInt64s, iSecond);

	size_t iSize = 0;
	size_t i = 0;

	iSize = Array_iSize(aInt64s);
	for (i = 0; i < iSize; ++i)
	{
		printf("%" PRId64 "\n", aInt64s[i]);
	}

	String sFirst = "aa";
	String sSecond = "bb";

	Array_init(aStrings);
	Array_push(aStrings, sFirst);
	Array_push(aStrings, sSecond);

	iSize = Array_iSize(aStrings);
	for (i = 0; i < iSize; ++i)
	{
		printf("%s\n", aStrings[i]);
	}

	Array_init(aPoints);
	Array_push(aPoints, ((Point) { 1, 2 }));
	
	iSize = Array_iSize(aPoints);
	for (i = 0; i < iSize; ++i)
	{
		printf("%d\n", aPoints[i].y);
	}

	_return(EXIT_SUCCESS);

AutoFree_finalize:

	String_free(sNew);
	Array_free(aInt64s);
	Array_free(aStrings);
	Array_free(aPoints);

	return _value;
}
