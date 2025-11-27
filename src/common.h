#ifndef COMMON_H
#define COMMON_H

#define UNUSED(x) (void)(x)

#include <iostream>
#include <vector>
#include <fstream>
#include <complex>
#include <thread>
#include <stdexcept>
#include <string>
#include <cstring>
#include <atomic>
#include <unordered_map>
#include "H5Cpp.h"

using std::thread;
using std::vector;
using std::string;

typedef uint8_t   u8;
typedef int32_t   b32;
typedef int32_t   i32;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef float     f32;
typedef double    f64;
typedef uintptr_t uptr;
typedef std::complex<f32> c32;

constexpr f32 c = 2.997924e8f;
constexpr f32 pi = 3.14159f;
constexpr f32 c2 = c * c;
constexpr f32 eps0 = 8.85418782e-12f;
constexpr f32 mu0 = 4.0f * pi * 1e-7f;
constexpr f32 h = 6.6261e-34f;
constexpr f32 C_electron = 1.602176634e-19f;
constexpr f32 m_electron = 9.1093837e-31f;
constexpr f32 c2Pi = 2.0f * pi * c;
constexpr f32 e = 2.7182818284590f;
constexpr f32 boltzman = 1.380649e-23f;
constexpr c32 Im(0.0f, 1.0f);

inline std::atomic<bool> writing = false;

inline void linspace(vector<f32>* A, u64 N, f32 dx)
{
	A->resize(N);
	for (u64 n = 0; n < N; n++)
	{
		A->at(n) = dx * (f32)n;
	}
}

inline void linspace(vector<f32>* A, u64 N, f32 start, f32 stop)
{
	A->resize(N);
	for (u64 n = 0; n < N; n++)
	{
		A->at(n) = start + ((f32)n) * ((stop - start) / (N - 1));
	}
}

inline void linspace(f32* A, u64 N, f32 start, f32 stop)
{
	for (u64 n = 0; n < N; n++)
	{
		A[n] = start + ((f32)n) * ((stop - start) / (N - 1));
	}
}

inline void print_dtype_info(const H5::DataType& dt) 
{
	H5T_class_t cls = dt.getClass();
	std::cout << "Datatype class: ";
	if (cls == H5T_STRING) std::cout << "STRING\n";
	else if (cls == H5T_INTEGER) std::cout << "INTEGER\n";
	else if (cls == H5T_FLOAT) std::cout << "FLOAT\n";
	else std::cout << "OTHER\n";

	hid_t id = dt.getId();
	if (cls == H5T_STRING) {
		size_t size = H5Tget_size(id);
		std::cout << " HDF5 size: " << size << "\n";
		htri_t isvar = H5Tis_variable_str(id);
		std::cout << " variable-length? " << (isvar > 0 ? "YES" : "NO") << "\n";
		// charset info (C API)
		H5T_cset_t cset = H5Tget_cset(id);
		std::cout << " charset: " << (cset == H5T_CSET_UTF8 ? "UTF-8" : "ASCII/OTHER") << "\n";
	}
}

template <typename T>
inline T readAttribute(H5::Group& grp, const char* name);

template <>
inline f32 readAttribute(H5::Group& grp, const char* name)
{
	H5::Attribute attr = grp.openAttribute(name);
	H5::DataType type = attr.getDataType();
	f32 val;
	attr.read(type, &val);
	return val;
}

template <>
inline u32 readAttribute(H5::Group& grp, const char* name)
{
	H5::Attribute attr = grp.openAttribute(name);
	H5::DataType type = attr.getDataType();
	f32 val;
	attr.read(type, &val);
	return (u32)val;
}

template <>
inline bool readAttribute(H5::Group& grp, const char* name)
{
	H5::Attribute attr = grp.openAttribute(name);
	H5::DataType type = attr.getDataType();
	f32 val;
	attr.read(type, &val);
	return (bool)val;
}

template <>
inline std::string readAttribute(H5::Group& grp, const char* name)
{
	// open attribute
	hid_t aid = H5Aopen(grp.getId(), name, H5P_DEFAULT);

	// make a string datatype: variable-length C string with UTF-8 charset
	hid_t strtype = H5Tcopy(H5T_C_S1);
	H5Tset_size(strtype, H5T_VARIABLE);
	H5Tset_cset(strtype, H5T_CSET_UTF8);

	// read into char* (for scalar attribute, we still pass address of char*)
	char* rdata = nullptr;
	herr_t err = H5Aread(aid, strtype, &rdata);
	if (err < 0) {
		std::cerr << "H5Aread failed\n";
	}
	else {
		if (rdata) {
			std::string s(rdata);
			return s;
		}
		else {
			std::cout << "Attribute returned NULL pointer (empty?)\n";
		}
	}
    throw;
    return "";
}

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

inline void printProgress(double percentage)
{
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush(stdout);
}

#undef PBSTR
#undef PBWIDTH

#endif