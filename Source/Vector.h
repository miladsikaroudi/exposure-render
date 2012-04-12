/*
	Copyright (c) 2011, T. Kroes <t.kroes@tudelft.nl>
	All rights reserved.

	Redistribution and use in source and binary forms, with or witDEVut modification, are permitted provided that the following conditions are met:

	- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	- Neither the name of the TU Delft nor the names of its contributors may be used to endorse or promote products derived from this software witDEVut specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT DEVLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT DEVLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) DEVWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "Defines.h"
#include "Enums.h"

namespace ExposureRender
{

template <class T>
HOST_DEVICE inline float Clamp(const T& Value, const T& Min, const T& Max)
{
    return Value < Min ? Min : (Value > Max ? Max : Value);
}

template<class T, int Size>
class EXPOSURE_RENDER_DLL Vec
{
public:
	HOST_DEVICE Vec()
	{
		for (int i = 0; i < Size; i++)
			this->D[i] = T();
	}

	HOST_DEVICE Vec(const T& V)
	{
		for (int i = 0; i < Size; i++)
			this->D[i] = V;
	}

	HOST_DEVICE Vec(const Vec<T, Size>& Other)
	{
		for (int i = 0; i < Size; i++)
			this->D[i] = Other[i];
	}

	HOST_DEVICE Vec<T, Size>& operator = (const Vec<T, Size>& Other)
	{
		for (int i = 0; i < Size; i++)
			this->D[i] = Other[i];

		return *this;
	}

	HOST_DEVICE T operator[](const int& i) const
	{
		return this->D[i];
	}

	HOST_DEVICE T& operator[](const int& i)
	{
		return this->D[i];
	}

	HOST_DEVICE Vec<T, Size> operator + (const Vec<T, Size>& V) const
	{
		Vec<T, Size> Result;

		for (int i = 0; i < Size; i++)
			Result[i] = this->D[i] + V[i];

		return Result;
	}

	HOST_DEVICE Vec<T, Size>& operator += (const Vec<T, Size>& V)
	{
		for (int i = 0; i < Size; i++)
			this->D[i] += V[i];

		return *this;
	}

	HOST_DEVICE Vec<T, Size> operator - (const Vec<T, Size>& V) const
	{
		Vec<T, Size> Result;

		for (int i = 0; i < Size; i++)
			Result[i] = this->D[i] - V[i];

		return Result;
	}

	HOST_DEVICE Vec<T, Size>& operator -= (const Vec<T, Size>& V)
	{
		for (int i = 0; i < Size; i++)
			this->D[i] -= V[i];

		return *this;
	}

	HOST_DEVICE Vec<T, Size> operator * (const Vec<T, Size>& V)
	{
		Vec<T, Size> Result;

		for (int i = 0; i < Size; i++)
			Result[i] = this->D[i] * V[i];

		return Result;
	}

	HOST_DEVICE Vec<T, Size>& operator *= (const Vec<T, Size>& V)
	{
		for (int i = 0; i < Size; i++)
			this->D[i] *= V[i];

		return *this;
	}

	HOST_DEVICE Vec<T, Size> operator / (const Vec<T, Size>& V)
	{
		Vec<T, Size> Result;

		for (int i = 0; i < Size; i++)
			Result[i] = this->D[i] / V[i];

		return Result;
	}

	HOST_DEVICE Vec<T, Size>& operator /= (const Vec<T, Size>& V)
	{
		for (int i = 0; i < Size; i++)
		{
			const float Inv = V[i] == 0.0f ? 0.0f : 1.0f / V[i];
			this->D[i] *= Inv;
		}

		return *this;
	}

	HOST_DEVICE bool operator < (const Vec<T, Size>& V) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (this->D[i] >= V[i])
				return false;
		}

		return true;
	}

	HOST_DEVICE bool operator <= (const Vec<T, Size>& V) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (this->D[i] > V[i])
				return false;
		}

		return true;
	}

	HOST_DEVICE bool operator > (const Vec<T, Size>& V) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (this->D[i] <= V[i])
				return false;
		}

		return true;
	}

	HOST_DEVICE bool operator >= (const Vec<T, Size>& V) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (this->D[i] < V[i])
				return false;
		}

		return true;
	}

	HOST_DEVICE bool operator == (const Vec<T, Size>& V) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (this->D[i] != V[i])
				return false;
		}

		return true;
	}

	HOST_DEVICE bool operator != (const Vec<T, Size>& V) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (this->D[i] != V[i])
				return true;
		}

		return false;
	}

	HOST_DEVICE T Max(void)
	{
		T Max;

		for (int i = 1; i < Size; i++)
		{
			if (D[i] > this->D[i - 1])
				Max = this->D[i];
		}

		return Max;
	}

	HOST_DEVICE T Min(void)
	{
		T Min;

		for (int i = 1; i < Size; i++)
		{
			if (this->D[i] < D[i - 1])
				Min = this->D[i];
		}

		return Min;
	}

	HOST_DEVICE Vec<T, Size> Min(const Vec<T, Size>& Other) const
	{
		Vec<T, Size> Result;
		
		for (int i = 0; i < Size; i++)
			Result[i] = min(this->D[i], Other[i]);

		return Result;
	}

	HOST_DEVICE Vec<T, Size> Max(const Vec<T, Size>& Other) const
	{
		Vec<T, Size> Result;
		
		for (int i = 0; i < Size; i++)
			Result[i] = max(this->D[i], Other[i]);

		return Result;
	}

	HOST_DEVICE void Clamp(const T& Min, const T& Max)
	{
		for (int i = 0; i < Size; ++i)
			this->D[i] = max(Min, min(this->D[i], Max));
	}

	HOST_DEVICE void Clamp(const Vec<T, Size>& Min, const Vec<T, Size>& Max)
	{
		for (int i = 0; i < Size; ++i)
			this->D[i] = max(Min[i], min(this->D[i], Max[i]));
	}

protected:
	T	D[Size];
};

class EXPOSURE_RENDER_DLL Vec2i : public Vec<int, 2>
{
public:
	HOST_DEVICE Vec2i() :
		Vec<int, 2>()
	{
	}
	
	HOST_DEVICE Vec2i(const Vec<int, 2>& V) :
		Vec<int, 2>(V)
	{
	}

	HOST_DEVICE Vec2i(const int& V)
	{
		this->D[0] = V;
		this->D[1] = V;
	}

	HOST_DEVICE Vec2i(const int& V0, const int& V1)
	{
		this->D[0] = V0;
		this->D[1] = V1;
	}

	HOST_DEVICE Vec2i& operator = (const Vec2i& Other)
	{
		for (int i = 0; i < 2; i++)
			this->D[i] = Other[i];

		return *this;
	}
};

class EXPOSURE_RENDER_DLL Vec2f : public Vec<float, 2>
{
public:
	HOST_DEVICE Vec2f() :
		Vec<float, 2>()
	{
	}

	HOST_DEVICE Vec2f(const Vec<float, 2>& V) :
		Vec<float, 2>(V)
	{
	}
		
	HOST_DEVICE Vec2f(const float& V)
	{
		this->D[0] = V;
		this->D[1] = V;
	}

	HOST_DEVICE Vec2f(const float& V0, const float& V1)
	{
		this->D[0] = V0;
		this->D[1] = V1;
	}

	HOST_DEVICE Vec2f& operator = (const Vec2f& Other)
	{
		for (int i = 0; i < 2; i++)
			this->D[i] = Other[i];

		return *this;
	}

	HOST_DEVICE float LengthSquared(void) const
	{
		return this->D[0] * this->D[0] + this->D[1] * this->D[1];
	}

	HOST_DEVICE float Length(void) const
	{
		return sqrtf(this->LengthSquared());
	}

	HOST_DEVICE void Normalize(void)
	{
		const float L = this->Length();
		this->D[0] /= L;
		this->D[1] /= L;
	}
};

class EXPOSURE_RENDER_DLL Vec3i : public Vec<int, 3>
{
public:
	HOST_DEVICE Vec3i() :
		Vec<int, 3>()
	{
	}
	
	HOST_DEVICE Vec3i(const Vec<int, 3>& V) :
		Vec<int, 3>(V)
	{
	}

	HOST_DEVICE Vec3i(const int& V)
	{
		this->D[0] = V;
		this->D[1] = V;
		this->D[2] = V;
	}

	HOST_DEVICE Vec3i(const int& V0, const int& V1, const int& V2)
	{
		this->D[0] = V0;
		this->D[1] = V1;
		this->D[2] = V2;
	}

	HOST_DEVICE Vec3i& operator = (const Vec3i& Other)
	{
		for (int i = 0; i < 3; i++)
			this->D[i] = Other[i];

		return *this;
	}
};

class EXPOSURE_RENDER_DLL Vec3f : public Vec<float, 3>
{
public:
	HOST_DEVICE Vec3f() :
		Vec<float, 3>()
	{
	}

	HOST_DEVICE Vec3f(const Vec<float, 3>& V) :
		Vec<float, 3>(V)
	{
	}

	HOST_DEVICE Vec3f(const Vec3f& Other)
	{
		*this = Other;
	}

	HOST_DEVICE Vec3f(const float& V)
	{
		this->D[0] = V;
		this->D[1] = V;
		this->D[2] = V;
	}

	HOST_DEVICE Vec3f(const float& V0, const float& V1, const float& V2)
	{
		this->D[0] = V0;
		this->D[1] = V1;
		this->D[2] = V2;
	}

	HOST_DEVICE Vec3f& operator = (const Vec3f& Other)
	{
		for (int i = 0; i < 3; i++)
			this->D[i] = Other[i];

		return *this;
	}

	HOST_DEVICE float LengthSquared(void) const
	{
		return this->D[0] * this->D[0] + this->D[1] * this->D[1] + this->D[2] * this->D[2];
	}

	HOST_DEVICE float Length(void) const
	{
		return sqrtf(this->LengthSquared());
	}

	HOST_DEVICE void Normalize(void)
	{
		const float L = this->Length();
		this->D[0] /= L;
		this->D[1] /= L;
		this->D[2] /= L;
	}

	HOST_DEVICE float Dot(const Vec3f& V) const
	{
		return (this->D[0] * V[0] + this->D[1] * V[1] + this->D[2] * V[2]);
	}

	HOST_DEVICE Vec3f Cross(const Vec3f& V) const
	{
		return Vec3f( (this->D[1] * V[2]) - (this->D[2] * V[1]), (this->D[2] * V[0]) - (this->D[0] * V[2]), (this->D[0] * V[1]) - (this->D[1] * V[0]) );
	}

	HOST_DEVICE void ScaleBy(float F)
	{
		this->D[0] *= F;
		this->D[1] *= F;
		this->D[2] *= F;
	}
};

static inline HOST_DEVICE Vec2i operator + (const Vec2i& A, const Vec2i& B)					{ return Vec2i(A[0] + B[0], A[1] + B[1]);						};
static inline HOST_DEVICE Vec2i operator - (const Vec2i& A, const Vec2i& B)					{ return Vec2i(A[0] - B[0], A[1] - B[1]);						};
static inline HOST_DEVICE Vec2i operator * (const Vec2i& V, const int& I)					{ return Vec2i(V[0] * I, V[1] * I);								};
static inline HOST_DEVICE Vec2i operator * (const int& I, const Vec2i& V)					{ return Vec2i(V[0] * I, V[1] * I);								};
static inline HOST_DEVICE Vec2i operator * (const Vec2i& A, const Vec2i& B)					{ return Vec2i(A[0] * B[0], A[1] * B[1]);						};
static inline HOST_DEVICE Vec2i operator / (const Vec2i& V, const int& I)					{ return Vec2i(V[0] / I, V[1] / I);								};
static inline HOST_DEVICE Vec2i operator / (const Vec2i& A, const Vec2i& B)					{ return Vec2i(A[0] / B[0], A[1] / B[1]);						};

static inline HOST_DEVICE Vec2f operator + (const Vec2f& A, const Vec2f& B)					{ return Vec2f(A[0] + B[0], A[1] + B[1]);						};
static inline HOST_DEVICE Vec2f operator - (const Vec2f& A, const Vec2f& B)					{ return Vec2f(A[0] - B[0], A[1] - B[1]);						};
static inline HOST_DEVICE Vec2f operator * (const Vec2f& V, const float& F)					{ return Vec2f(V[0] * F, V[1] * F);								};
static inline HOST_DEVICE Vec2f operator * (const float& F, const Vec2f& V)					{ return Vec2f(V[0] * F, V[1] * F);								};
static inline HOST_DEVICE Vec2f operator * (const Vec2f& A, const Vec2f& B)					{ return Vec2f(A[0] * B[0], A[1] * B[1]);						};
static inline HOST_DEVICE Vec2f operator / (const Vec2f& V, const float& F)					{ return Vec2f(V[0] / F, V[1] / F);								};
static inline HOST_DEVICE Vec2f operator / (const Vec2f& A, const Vec2f& B)					{ return Vec2f(A[0] / B[0], A[1] / B[1]);						};

static inline HOST_DEVICE Vec2f Normalize(const Vec2f& V)									{ Vec2f R = V; R.Normalize(); return R; 						};
static inline HOST_DEVICE float Length(const Vec2f& V)										{ return V.Length();											};

/*
static inline HOST_DEVICE Vec2f Cross(const Vec2f& A, const Vec2f& B)						{ return A.Cross(B);											};
static inline HOST_DEVICE float Dot(const Vec2f& A, const Vec2f& B)							{ return A.Dot(B);												};
static inline HOST_DEVICE float AbsDot(const Vec2f& A, const Vec2f& B)						{ return fabs(A.Dot(B));										};
static inline HOST_DEVICE float ClampedAbsDot(const Vec2f& A, const Vec2f& B)				{ return Clamp(fabs(A.Dot(B)), 0.0f, 1.0f);						};
static inline HOST_DEVICE float ClampedDot(const Vec2f& A, const Vec2f& B)					{ return Clamp(Dot(A, B), 0.0f, 1.0f);							};
*/

static inline HOST_DEVICE float Distance(const Vec2f& A, const Vec2f& B)					{ return (A - B).Length();										};
static inline HOST_DEVICE float DistanceSquared(const Vec2f& A, const Vec2f& B)				{ return (A - B).LengthSquared();								};
static inline HOST_DEVICE Vec2f Lerp(const Vec2f& A, const Vec2f& B, const float& LerpC)	{ return A + LerpC * (B - A);									};

static inline HOST_DEVICE Vec3i operator + (const Vec3i& A, const Vec3i& B)					{ return Vec3i(A[0] + B[0], A[1] + B[1], A[2] + B[2]);			};
static inline HOST_DEVICE Vec3i operator - (const Vec3i& A, const Vec3i& B)					{ return Vec3i(A[0] - B[0], A[1] - B[1], A[2] - B[2]);			};
static inline HOST_DEVICE Vec3i operator * (const Vec3i& V, const int& I)					{ return Vec3i(V[0] * I, V[1] * I, V[2] * I);					};
static inline HOST_DEVICE Vec3i operator * (const int& I, const Vec3i& V)					{ return Vec3i(V[0] * I, V[1] * I, V[2] * I);					};
static inline HOST_DEVICE Vec3i operator * (const Vec3i& A, const Vec3i& B)					{ return Vec3i(A[0] * B[0], A[1] * B[1], A[2] * B[2]);			};
static inline HOST_DEVICE Vec3i operator / (const Vec3i& V, const int& I)					{ return Vec3i(V[0] / I, V[1] / I, V[2] / I);					};
static inline HOST_DEVICE Vec3i operator / (const Vec3i& A, const Vec3i& B)					{ return Vec3i(A[0] / B[0], A[1] / B[1], A[2] / B[2]);			};

static inline HOST_DEVICE Vec3f operator + (const Vec3f& A, const Vec3f& B)					{ return Vec3f(A[0] + B[0], A[1] + B[1], A[2] + B[2]);			};
static inline HOST_DEVICE Vec3f operator - (const Vec3f& A, const Vec3f& B)					{ return Vec3f(A[0] - B[0], A[1] - B[1], A[2] - B[2]);			};
static inline HOST_DEVICE Vec3f operator * (const Vec3f& V, const float& F)					{ return Vec3f(V[0] * F, V[1] * F, V[2] * F);					};
static inline HOST_DEVICE Vec3f operator * (const float& F, const Vec3f& V)					{ return Vec3f(V[0] * F, V[1] * F, V[2] * F);					};
static inline HOST_DEVICE Vec3f operator * (const Vec3f& A, const Vec3f& B)					{ return Vec3f(A[0] * B[0], A[1] * B[1], A[2] * B[2]);			};
static inline HOST_DEVICE Vec3f operator / (const Vec3f& V, const float& F)					{ return Vec3f(V[0] / F, V[1] / F, V[2] / F);					};
static inline HOST_DEVICE Vec3f operator / (const Vec3f& A, const Vec3f& B)					{ return Vec3f(A[0] / B[0], A[1] / B[1], A[2] / B[2]);			};

static inline HOST_DEVICE Vec3f Normalize(const Vec3f& V)									{ Vec3f R = V; R.Normalize(); return R; 						};
static inline HOST_DEVICE float Length(const Vec3f& V)										{ return V.Length();											};
static inline HOST_DEVICE Vec3f Cross(const Vec3f& A, const Vec3f& B)						{ return A.Cross(B);											};
static inline HOST_DEVICE float Dot(const Vec3f& A, const Vec3f& B)							{ return A.Dot(B);												};
static inline HOST_DEVICE float AbsDot(const Vec3f& A, const Vec3f& B)						{ return fabs(A.Dot(B));										};
static inline HOST_DEVICE float ClampedAbsDot(const Vec3f& A, const Vec3f& B)				{ return Clamp(fabs(A.Dot(B)), 0.0f, 1.0f);						};
static inline HOST_DEVICE float ClampedDot(const Vec3f& A, const Vec3f& B)					{ return Clamp(Dot(A, B), 0.0f, 1.0f);							};
static inline HOST_DEVICE float Distance(const Vec3f& A, const Vec3f& B)					{ return (A - B).Length();										};
static inline HOST_DEVICE float DistanceSquared(const Vec3f& A, const Vec3f& B)				{ return (A - B).LengthSquared();								};
static inline HOST_DEVICE Vec3f Lerp(const Vec3f& A, const Vec3f& B, const float& LerpC)	{ return A + LerpC * (B - A);									};

}