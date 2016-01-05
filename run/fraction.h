
#ifndef _FRACTION_H_
#define _FRACTION_H_

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

class fraction {
public:
	fraction();
	fraction(int n);
	explicit fraction(int n, int d);
//	explicit fraction(const fraction&);

	fraction& operator=(const fraction&);

	float value() const;
	void output() const;
	string strval() const;

	bool isInt() const;
	int intval() const;

	int floor() const;
	int ceil() const;

	fraction& operator+=(const fraction&);
	fraction& operator-=(const fraction&);

	friend bool operator==(const fraction&, const fraction&);
	friend bool operator>(const fraction&, const fraction&);
	friend bool operator<(const fraction&, const fraction&);

	friend fraction operator+(const fraction&, const fraction&);
	friend fraction operator-(const fraction&, const fraction&);
	friend fraction operator*(const fraction&, const fraction&);
	friend fraction operator/(const fraction&, const fraction&);

private:
	int nu_, de_;
	void simplify_();
};


#endif
